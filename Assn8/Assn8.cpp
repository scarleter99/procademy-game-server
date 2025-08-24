#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <windows.h>
#include <io.h>        // _chsize_s, _fileno
using namespace std;

static uint32_t hashBuf(const unsigned char* buf, size_t bufSize) {
    // FNV-1a (32-bit)
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < bufSize; ++i) {
        h ^= buf[i];
        h *= 16777619u;
    }
    return h;
}

bool encryptFile(const string& fileName, unsigned char key) {
    FILE* pFile = nullptr;
    if (fopen_s(&pFile, fileName.c_str(), "rb+") != 0 || pFile == nullptr) {
        cout << "No File" << endl;
        return false;
    }

    // 파일 크기
    if (fseek(pFile, 0, SEEK_END) != 0) { fclose(pFile); return false; }
    long fSize = ftell(pFile);
    if (fSize < 0) { fclose(pFile); return false; }
    if (fseek(pFile, 0, SEEK_SET) != 0) { fclose(pFile); return false; }
    if (fSize == 0) { fclose(pFile); return true; }

    size_t bufSize = static_cast<size_t>(fSize);
    unsigned char* buf = static_cast<unsigned char*>(malloc(bufSize));
    if (!buf) { fclose(pFile); return false; }

    size_t rSize = fread(buf, 1, bufSize, pFile);
    if (rSize != bufSize) { free(buf); fclose(pFile); return false; }

    const size_t headerSize = sizeof(uint32_t);

    // out 버퍼(결과)
    unsigned char* outBuf = nullptr;
    size_t outBufSize = 0;
    // 
    if (bufSize >= headerSize + 1) {
        // 암호문으로 가정하고 헤더 추출
        uint32_t storedHeader = 0;
        memcpy(&storedHeader, buf, headerSize);

        const unsigned char* outData = buf + headerSize;
        size_t outDataSize = bufSize - headerSize;

        // 암호문으로 가정하고 평문 추출
        unsigned char* tmpPlain = static_cast<unsigned char*>(malloc(outDataSize));
        if (!tmpPlain) { free(buf); fclose(pFile); return false; }
        memcpy(tmpPlain, outData, outDataSize);
        for (size_t i = 0; i < outDataSize; ++i) {
            tmpPlain[i] ^= key;
        }

        uint32_t calcHeaderOnPlain = hashBuf(tmpPlain, outDataSize);

        if (calcHeaderOnPlain == storedHeader) {
            // 복호화: (평문 해시)암호문 -> 평문
            outBufSize = outDataSize;
            outBuf = static_cast<unsigned char*>(malloc(outBufSize));
            if (!outBuf) { memset(tmpPlain, 0, outDataSize); free(tmpPlain); free(buf); fclose(pFile); return false; }

            // 평문 입력
            memcpy(outBuf, tmpPlain, outDataSize);
            free(tmpPlain);
            
            cout << "Decrypt File" << endl;
        }
        else {
            // 암호화: 평문 -> (평문 해시)암호문
            free(tmpPlain);

            uint32_t calcHeader = hashBuf(buf, bufSize);
            outBufSize = headerSize + bufSize;
            outBuf = static_cast<unsigned char*>(malloc(outBufSize));
            if (!outBuf) { free(buf); fclose(pFile); return false; }

            // 헤더입력
            memcpy(outBuf, &calcHeader, headerSize);

            // 암호문 입력
            unsigned char* tmp = static_cast<unsigned char*>(malloc(bufSize));
            if (!tmp) { free(outBuf); free(buf); fclose(pFile); return false; }
            memcpy(tmp, buf, bufSize);
            for (size_t i = 0; i < bufSize; ++i) {
                tmp[i] ^= key;
            }

            memcpy(outBuf + headerSize, tmp, bufSize);
            free(tmp);

            cout << "Encrypt File" << endl;
        }
    }
    else {
        // 너무 짧아서 평문으로 간주 -> 암호화
        uint32_t calcHeader = hashBuf(buf, bufSize);
        outBufSize = headerSize + bufSize;
        outBuf = static_cast<unsigned char*>(malloc(outBufSize));
        if (!outBuf) { free(buf); fclose(pFile); return false; }

        // 헤더입력
        memcpy(outBuf, &calcHeader, headerSize);

        // 암호문 입력
        unsigned char* tmp = static_cast<unsigned char*>(malloc(bufSize));
        if (!tmp) { free(outBuf); free(buf); fclose(pFile); return false; }
        memcpy(tmp, buf, bufSize);
        for (size_t i = 0; i < bufSize; ++i) {
            tmp[i] ^= key;
        }

        memcpy(outBuf + headerSize, tmp, bufSize);
        free(tmp);

        cout << "Encrypt File" << endl;
    }

    // 파일 처음으로 이동
    if (fseek(pFile, 0, SEEK_SET) != 0) {
        free(buf);
        free(outBuf);
        fclose(pFile);
        return false;
    }

    // 쓰기
    size_t wSize = fwrite(outBuf, 1, outBufSize, pFile);
    if (wSize != outBufSize) {
        free(buf);
        free(outBuf);
        fclose(pFile);
        return false;
    }

    // 길이 정확히 맞추기(복호화로 더 짧아졌을 수 있음)
    if (_chsize_s(_fileno(pFile), static_cast<long long>(outBufSize)) != 0) {
        free(buf);
        free(outBuf);
        fclose(pFile);
        return false;
    }

    fflush(pFile);
    free(buf);
    free(outBuf);
    fclose(pFile);
    return true;
}

int main() {
    while (1) {
        string fileName;
        cout << "File Name: ";
        cin >> fileName;
        unsigned char key = 0x5A;
        if (encryptFile(fileName, key)) {
            cout << "Success" << endl << endl;
        }
        else {
            cout << "Fail" << endl << endl;
        }
    }
}
