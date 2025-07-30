#include <iostream>
#include <windows.h>

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

void TernaryOperatorTest() {
    int a;
    int b = 20;

    a = b == 30 ? 10 : 20;

    int c = sizeof(int);
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

void getTickCountTest() {
    DWORD start = GetTickCount(); // 시스템이 시작된 후 경과한 시간(밀리초)을 윈도우가 반환
    for (volatile int i = 0; i < 100000000; ++i);
    DWORD end = GetTickCount();
    std::cout << "GetTickCount() Elapsed time: " << (end - start) << " ms\n";

    clock_t start2 = clock();  // 프로세스가 측정한 경과한 시간
    for (volatile int i = 0; i < 100000000; ++i);
    clock_t end2 = clock();
    double duration = static_cast<double>(end2 - start2) / CLOCKS_PER_SEC;
    std::cout << "clock() Elapsed time: " << duration << " seconds" << std::endl;
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

int main()
{
    mallocTest2();

    return 0;
}