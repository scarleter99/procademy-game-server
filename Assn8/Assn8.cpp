#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>   // memcpy
using namespace std;

static uint32_t hashBuf(const unsigned char* buf, size_t bufSize) {
    // FNV-1a (32-bit)
    uint32_t h = 2166136261u;
    for (size_t i = 0; i < bufSize; i++) {
        h ^= buf[i];
        h *= 16777619u;
    }
    return h;
}

bool encryptFile(const string& fileName, unsigned char key) {
    // 읽기
    ifstream fin(fileName, ios::binary);
    if (!fin) {
        cout << "No File" << endl;
        return false;
    }

    // 파일 전체를 벡터로 읽기
    vector<unsigned char> buf(
        (istreambuf_iterator<char>(fin)),
        istreambuf_iterator<char>()
    );
    fin.close();

    const size_t bufSize = buf.size();
    const size_t headerSize = sizeof(uint32_t);

    // out 버퍼(결과)
    vector<unsigned char> outBuf;

    if (bufSize == 0) {
        ofstream fout(fileName, ios::binary | ios::trunc);
        return static_cast<bool>(fout);
    }

    if (bufSize >= headerSize + 1) {
        // 암호문으로 가정하고 헤더 추출
        uint32_t storedHeader = 0;
        memcpy(&storedHeader, buf.data(), headerSize);

        const unsigned char* outData = buf.data() + headerSize;
        const size_t outDataSize = bufSize - headerSize;

        // 암호문으로 가정하고 평문 추출
        vector<unsigned char> tmpPlain(outData, outData + outDataSize);
        for (size_t i = 0; i < outDataSize; i++) {
            tmpPlain[i] ^= key;
        }

        uint32_t calcHeaderOnPlain = hashBuf(tmpPlain.data(), outDataSize);

        if (calcHeaderOnPlain == storedHeader) {
            // 복호화: (평문 해시)암호문 -> 평문
            outBuf = move(tmpPlain);
            cout << "Decrypt File" << endl;
        }
        else {
            // 암호화: 평문 -> (평문 해시)암호문
            uint32_t calcHeader = hashBuf(buf.data(), bufSize);

            outBuf.resize(headerSize + bufSize);
            // 헤더 입력
            memcpy(outBuf.data(), &calcHeader, headerSize);

            // 본문 암호화 후 입력
            vector<unsigned char> tmp(buf.begin(), buf.end());
            for (size_t i = 0; i < bufSize; i++) {
                tmp[i] ^= key;
            }
            memcpy(outBuf.data() + headerSize, tmp.data(), bufSize);

            cout << "Encrypt File" << endl;
        }
    }
    else {
        // 너무 짧아서 평문으로 간주 -> 암호화
        uint32_t calcHeader = hashBuf(buf.data(), bufSize);

        outBuf.resize(headerSize + bufSize);
        // 헤더 입력
        memcpy(outBuf.data(), &calcHeader, headerSize);

        // 본문 암호화 후 입력
        vector<unsigned char> tmp(buf.begin(), buf.end());
        for (size_t i = 0; i < bufSize; i++) {
            tmp[i] ^= key;
        }
        memcpy(outBuf.data() + headerSize, tmp.data(), bufSize);

        cout << "Encrypt File" << endl;
    }

    // 쓰기: trunc로 파일을 깔끔히 덮어써서 길이 조정까지 한 번에 해결
    ofstream fout(fileName, ios::binary | ios::trunc);
    if (!fout) {
        return false;
    }
    if (!outBuf.empty()) {
        fout.write(reinterpret_cast<const char*>(outBuf.data()),
            static_cast<std::streamsize>(outBuf.size()));
        if (!fout) {
            return false;
        }
    }

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
