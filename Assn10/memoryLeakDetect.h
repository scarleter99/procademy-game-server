#pragma once
using namespace std;

#define MAX_ALLOC_INFO 100
#define MAX_ERROR_INFO 100

#ifdef MEMORY_LEAK_DETECT
    #define new new(__FILE__, __LINE__)
#endif

struct AllocInfo {
    void* ptr;
    int size;
    char filename[128];
    int line;
    bool array;
};

struct ErrorInfo {
    enum ErrorType
    {
        NOALLOC,
        ARRAY,
        LEAK,
    };

    ErrorType errorType;
    void* ptr;
    int size;
    char filename[128];
    int line;
    bool array;
};

class memoryLeakDetect
{
public:
    ~memoryLeakDetect();

    void* operator new (size_t size, const char* file, int line);
    void* operator new[](size_t size, const char* file, int line);

    void operator delete (void* p, const char* file, int line) {};
    void operator delete[](void* p, const char* file, int line) {};
    void operator delete (void* p);      // 실제 사용 delete
    void operator delete[](void* p);     // 실제 사용 delete[]

    void ProfileDataOutText(const string& szFileName);

private:
    static string makeLogFileName();
    static string ptrToHex(void* p);

    static void logNoAllocError(void* p);
    static void logArrayError(AllocInfo* info);
    static void logLeakError(AllocInfo* info);

private:
    static AllocInfo s_allocInfos[MAX_ALLOC_INFO];
    static ErrorInfo s_errorInfos[MAX_ERROR_INFO];
    static int       s_errorCount;
};
