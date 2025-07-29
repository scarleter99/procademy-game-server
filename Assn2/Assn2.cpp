#include <iostream>
using namespace std;

static int g_seed;

void Test(int seed) {
    int x;
    srand(seed);

    x = rand();
    cout << x << endl;

    x = rand();
    cout << x << endl;

    x = rand();
    cout << x << endl;

    cout << endl;
}

int randEx() {
    g_seed *= 0x343FD;
    g_seed += 0x269EC3;
    return (g_seed >> 16) & 0x7FFF;
}

void assn1() {
    int x;

    x = randEx();
    cout << x << endl;

    x = randEx();
    cout << x << endl;

    x = randEx();
    cout << x << endl;

    cout << endl;
}

int main()
{
    const int seed = 10;
    g_seed = seed;

    Test(seed);
    assn1();

    return 0;
}

