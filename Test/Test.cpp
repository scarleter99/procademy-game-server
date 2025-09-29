#include <iostream>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

#define SHOW(V)     \
{                   \
    int x = V;      \
    cout << x;      \
}

int AssemblyTest(int a) {
    int b;
    int c;

    b = 2;
    c = a + b;
    c = a * 3;

    return c;
}

void loopTest() {
    int a = 0;
    while (1) {
        a++;
    }

    for (;;) {
        a++;
    }
}

struct Data
{
    int a;
    int b;
    int c;
    char f[33];
};

Data structTest(Data d) {
    d.a += 1;

    return d;
}

void structTest2() {
    Data d;
    d.a = 1;
    d.b = 2;
    d.c = 3;

    Data d2 = structTest(d);
}

int errorTest() {
    int x = 0;
    x++;
    if (x == 1) {
        return x;
    }
}

void RandTest() {
    int r;

    srand(10);

    r = rand();
    cout << r << endl;
    r = rand();
    cout << r << endl;
    r = rand();
    cout << r << endl;
    r = rand();
    cout << r << endl;
    r = rand();
    cout << r << endl;
}

void MemoryCorruptionTest() {
    int a = 0;
    int b = 0;
    int c = 0;
    int* p = &a;

    //p += 1; // 쿠키 오염
    //p+= 2; // old ebp 오염(체크 불가)
    p += 3; // 함수 반환 주소 오염(체크 불가)
    *p = 0x0001ffff;
}

struct DATA2 {
    char a;
    char b;
    char c;
    char d;
};

union udata
{
    DATA2 d;
    unsigned long lp;
};

struct bitfield {
    unsigned int bit_a : 3;
    unsigned int bit_b : 3;
    unsigned int bit_c : 3;
};

void mallocTest() {
    int* p = (int*)malloc(30);
    p++;
    free(p); // UB
}

void mallocTest2() {
    int* p = (int*)malloc(39);
    //int* p = (int*)malloc(1000000); // 페이지 할당
    free(p);
    *p = 0; // UB
}

struct alignas(64) casheHitTestStruct {
    alignas(64) int a;
    int b;
    int c;
    int d;
};

class DATA3 {
public:
    int a;
    int b;
};

class DATA3_2 {
public:
    int c;
    int d;
};

DATA3 g_Player[100];
DATA3_2 g_Player2[100];

void casheHitTest() {
    for (int cnt = 0; cnt < 100; cnt++) {
        if (g_Player[cnt].a == 0 && g_Player[cnt].b == 0) {

        }
    }
}

void getTickCountTest() {
    DWORD start = GetTickCount64(); // 시스템이 시작된 후 경과한 시간(밀리초)을 윈도우가 반환
    for (volatile int i = 0; i < 100000000; ++i);
    DWORD end = GetTickCount64();
    std::cout << "GetTickCount() Elapsed time: " << (end - start) << " ms\n";

    clock_t start2 = clock();  // 프로세스가 측정한 경과한 시간
    for (volatile int i = 0; i < 100000000; ++i);
    clock_t end2 = clock();
    double duration = static_cast<double>(end2 - start2) / CLOCKS_PER_SEC;
    std::cout << "clock() Elapsed time: " << duration << " seconds" << std::endl;
}

void timeGetTimeTest() {
    timeBeginPeriod(1);

    while (1) {
        clock_t t = clock();
        DWORD dwTick = GetTickCount64();
        DWORD dwTick2 = timeGetTime();
        cout << t << " " << dwTick << " " << dwTick2 << endl;
    }

    timeEndPeriod(1);
}

void fflushTest() {
    FILE* pFile = nullptr;
    fopen_s(&pFile, "TEST.txt", "wb");
    if (pFile == nullptr) {
        return;
    }

    char* pBuffer = (char*)malloc(4096 * 2);
    if (pBuffer == nullptr) {
        fclose(pFile);
        return;
    }

    memset(pBuffer, 0xaa, 4096 * 2);
    int iret = fwrite(pBuffer, 1, 512, pFile);
    cout << iret << " bytes write" << endl;
    fflush(pFile);

    Sleep(100000);

    fclose(pFile);
}

void fseekTest() {
    FILE* pFile = nullptr;
    fopen_s(&pFile, "TEST.txt", "wb");
    if (pFile == nullptr) {
        return;
    }

    fseek(pFile, 0, SEEK_END);
    int sizeFile = ftell(pFile);

    char* pBuffer = (char*)malloc(sizeFile);
    if (pBuffer == nullptr) {
        fclose(pFile);
        return;
    }

    fseek(pFile, 0, SEEK_SET);
    fread(pBuffer, 1, sizeFile, pFile);
    rewind(pFile);

    Sleep(100000);

    fclose(pFile);
}

void frameControlTest() {
    timeBeginPeriod(1); // 해상도를 1ms로 낮춘다.

    // fps 확인용 값 
    DWORD fpsTick = timeGetTime();
    unsigned long fpsCount = 0;

    DWORD startTick = timeGetTime();
    DWORD endTick = 0;
    while (1) {
        Sleep(10); // 로직
        endTick = timeGetTime();
        // 프레임제어 대기 코드
        long useTick = (long)(endTick - startTick);
        startTick += 20;
        if (20 - useTick > 0) {
            Sleep(20 - useTick);
            //Render();
        }
        
        // fps 확인 코드
        {
            fpsCount++;
            if (timeGetTime() - fpsTick >= 1000) {
                cout << "FPS: " << fpsCount << endl;
                fpsCount = 0;
                fpsTick = timeGetTime();
            }
        }
    }

    timeEndPeriod(1);
}

void DeleteTest() {
    int* p1 = new int;
    int* p2 = new int[10];

    delete[] p1; // 소멸자가 있을 때만 문제가 발생한다.
    delete p2;
}

class CPlayer {
public:
    int GetHp() {
        return 0;
    }

    void SetHp(int hp) {
        _hp = hp;
    }

    void TestFunc() {
        int a = 0;
        a += 10;
        cout << a << endl;
    }

private:
    int _x;
    int _y;
    int _hp;
};

struct AllocInfo
{
    uintptr_t ptr;
    int size;
    string FileName;
    int LineNo;
    bool isArray;
};

void* operator new(size_t size, string file, int Line) // 매크로 처리
{
    void* adr = malloc(size); //2
    return adr;
}

void* operator new[](size_t size, string file, int Line) // 실제 크기 + 4가 할당된다.
{
    void* adr = malloc(size);
    return adr;
}

// 직접 사용
void operator delete (void* ptr) 
{
    delete ptr;
}

void operator delete[](void* ptr) //  실제 주소-4가 들어온다.
{
    delete[] ptr;
}

// 직접 사용안함
void operator delete (void* ptr, string file, int Line) {}
void operator delete[](void* ptr, string file, int Line) {}

void operatorTest() {
    int* p = new("file.cpp", 30) int;
    char* p2 = new("file.cpp", 30) char[10];
}

class CTest
{
    int _a;
    int _b;
    int _c;
    CTest* i;

public:
    void CTestFunc() {

        int x;
        int* p = &x;
        p -= 2;
        *p = 0; // this 포인터 오염

        x = _b; // 오류 발생
        _b = 0;
        cout << _b << endl;
    }
};

int main()
{
    void* p = malloc(sizeof(CTest));
    CTest* c = new(p) CTest;
    new(p) CTest;


    return 0;
}