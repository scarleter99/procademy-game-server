#include <iostream>
using namespace std;

#define SHOW(V)     \
{                   \
    int x = V;          \
    cout << x;          \
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
    if (x == 1)
        return x;
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

int main()
{
    Data d;
    d.a = 1;
    d.b = 2;
    d.c = 3;

    RandTest();

    return 0;
}