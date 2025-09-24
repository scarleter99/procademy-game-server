#include <cstdlib>
#include <new>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdio>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdint>
#include <sstream>

#include "memoryLeakDetect.h"
using namespace std;

AllocInfo memoryLeakDetect::s_allocInfos[MAX_ALLOC_INFO] = {};
ErrorInfo memoryLeakDetect::s_errorInfos[MAX_ERROR_INFO] = {};
int       memoryLeakDetect::s_errorCount = 0;

static int hexWidthForPtr() {
    return static_cast<int>(sizeof(void*) * 2);
}

string memoryLeakDetect::ptrToHex(void* p)
{
    uintptr_t v = reinterpret_cast<uintptr_t>(p);
    stringstream ss;
    ss << "0x" << hex << nouppercase << setw(hexWidthForPtr()) << setfill('0') << v;
    return ss.str();
}

string memoryLeakDetect::makeLogFileName()
{
    time_t now = time(nullptr);
    struct tm lt;
    errno_t err = localtime_s(&lt, &now);
    if (err != 0) {
        return "Alloc_log_fallback.txt";
    }

    char buf[64] = {};
    sprintf_s(buf, sizeof(buf), "Alloc_%04d%02d%02d_%02d%02d%02d.txt",
        lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday,
        lt.tm_hour, lt.tm_min, lt.tm_sec);
    return string(buf);
}

memoryLeakDetect::~memoryLeakDetect()
{
    const string fileName = makeLogFileName();
    ProfileDataOutText(fileName);
}

void* memoryLeakDetect::operator new (size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr == nullptr) {
        throw bad_alloc();
    }

    for (int i = 0; i < MAX_ALLOC_INFO; ++i) {
        if (s_allocInfos[i].ptr == nullptr) {
            s_allocInfos[i].ptr = ptr;
            s_allocInfos[i].size = static_cast<int>(size);
            s_allocInfos[i].line = line;
            s_allocInfos[i].array = false;
            strncpy_s(s_allocInfos[i].filename, file, sizeof(s_allocInfos[i].filename) - 1);
            break;
        }
    }

    return ptr;
}

void* memoryLeakDetect::operator new[](size_t size, const char* file, int line) {
    void* ptr = malloc(size);
    if (ptr == nullptr) {
        throw bad_alloc();
    }

    for (int i = 0; i < MAX_ALLOC_INFO; ++i) {
        if (s_allocInfos[i].ptr == nullptr) {
            s_allocInfos[i].ptr = ptr;
            s_allocInfos[i].size = static_cast<int>(size);
            s_allocInfos[i].line = line;
            s_allocInfos[i].array = true;
            strncpy_s(s_allocInfos[i].filename, file, sizeof(s_allocInfos[i].filename) - 1);
            break;
        }
    }

    return ptr;
}

void memoryLeakDetect::operator delete (void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    int deleteIdx = -1;
    for (int i = 0; i < MAX_ALLOC_INFO; ++i) {
        if (s_allocInfos[i].ptr == ptr) {
            deleteIdx = i;
            break;
        }
    }

    if (deleteIdx < 0) {
        logNoAllocError(ptr);
        free(ptr);
        return;
    }

    if (s_allocInfos[deleteIdx].array == true) {
        logArrayError(&s_allocInfos[deleteIdx]);
    }

    s_allocInfos[deleteIdx].ptr = nullptr;
    free(ptr);
}

void memoryLeakDetect::operator delete[](void* ptr) {
    if (ptr == nullptr) {
        return;
    }

    int deleteIdx = -1;
    for (int i = 0; i < MAX_ALLOC_INFO; ++i) {
        if (s_allocInfos[i].ptr == ptr) {
            deleteIdx = i;
            break;
        }
    }

    if (deleteIdx < 0) {
        logNoAllocError(ptr);
        free(ptr);
        return;
    }

    if (s_allocInfos[deleteIdx].array == false) {
        logArrayError(&s_allocInfos[deleteIdx]);
    }

    s_allocInfos[deleteIdx].ptr = nullptr;
    free(ptr);
}

void memoryLeakDetect::ProfileDataOutText(const string& szFileName)
{
    for (int i = 0; i < MAX_ALLOC_INFO; ++i) {
        if (s_allocInfos[i].ptr != nullptr) {
            logLeakError(&s_allocInfos[i]);
        }
    }

    ofstream fout(szFileName);
    if (!fout.is_open()) {
        return;
    }

    // NOALLOC
    for (int i = 0; i < s_errorCount; i++) {
        const ErrorInfo& e = s_errorInfos[i];
        if (e.errorType == ErrorInfo::NOALLOC) {
            fout << left << setw(7) << "NOALLOC" << " "
                << "[" << ptrToHex(e.ptr) << "]" << "\n";
        }
    }

    // ARRAY
    for (int i = 0; i < s_errorCount; i++) {
        const ErrorInfo& e = s_errorInfos[i];
        if (e.errorType == ErrorInfo::ARRAY) {
            fout << left << setw(7) << "ARRAY" << "   "
                << "[" << ptrToHex(e.ptr) << "] "
                << "[" << setw(10) << right << e.size << "] "
                << e.filename << " : " << e.line << "\n";
        }
    }

    // LEAK
    for (int i = 0; i < s_errorCount; i++) {
        const ErrorInfo& e = s_errorInfos[i];
        if (e.errorType == ErrorInfo::LEAK) {
            fout << left << setw(7) << "LEAK" << "    "
                << "[" << ptrToHex(e.ptr) << "] "
                << "[" << setw(10) << right << e.size << "] "
                << e.filename << " : " << e.line << "\n";
        }
    }

    fout.close();
}

void memoryLeakDetect::logNoAllocError(void* p)
{
    if (s_errorCount >= MAX_ERROR_INFO) {
        return;
    }

    ErrorInfo& e = s_errorInfos[s_errorCount++];
    e.errorType = ErrorInfo::NOALLOC;
    e.ptr = p;
    e.size = 0;
    e.filename[0] = '\0';
    e.line = 0;
    e.array = false;
}

void memoryLeakDetect::logArrayError(AllocInfo* info)
{
    if (info == nullptr || s_errorCount >= MAX_ERROR_INFO) {
        return;
    }

    ErrorInfo& e = s_errorInfos[s_errorCount++];
    e.errorType = ErrorInfo::ARRAY;
    e.ptr = info->ptr;
    e.size = info->size;
    strncpy_s(e.filename, info->filename, sizeof(e.filename) - 1);
    e.line = info->line;
    e.array = info->array;
}

void memoryLeakDetect::logLeakError(AllocInfo* info)
{
    if (info == nullptr || s_errorCount >= MAX_ERROR_INFO) {
        return;
    }

    ErrorInfo& e = s_errorInfos[s_errorCount++];
    e.errorType = ErrorInfo::LEAK;
    e.ptr = info->ptr;
    e.size = info->size;
    strncpy_s(e.filename, info->filename, sizeof(e.filename) - 1);
    e.line = info->line;
    e.array = info->array;
}
