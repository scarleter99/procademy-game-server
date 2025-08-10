#include <iostream>
#include <iomanip>
#include <bitset>
using namespace std;

const int ways = 8;
const int offsetSize = 6;
const int idxSize = 6;

struct Line {
    uint64_t tag = 0;
    bool valid = false;
    int cnt = 0;
};

Line cacheTable[1 << idxSize][ways];

void CheckCacheHit(void* a, string name, int size) {
    uint64_t addr = reinterpret_cast<uint64_t>(a);
    uint64_t idx = (addr >> offsetSize) & ((1 << idxSize) - 1);
    uint64_t tag = addr >> (offsetSize + idxSize);
    
    cout << name << " - addr: " << bitset<16>(addr) << endl;
    cout << name << " - idx: " << hex << idx << ", tag:" <<  tag << endl;

    for (int i = 0; i < ways; i++) {
        if (cacheTable[idx][i].valid && cacheTable[idx][i].tag == tag) {
            cout << name << " - Cache Hit" << endl << endl;
            for (int j = 0; j < ways; j++) {
                cacheTable[idx][j].cnt++;
            }

            cacheTable[idx][i].cnt = 0;
            return;
        }
    }

    cout << name << " - Cache Miss" << endl << endl;
    for (int i = 0; i < ways; i++) {
        if (cacheTable[idx][i].valid == false) {
            cacheTable[idx][i].valid = true;
            cacheTable[idx][i].tag = tag;
            cacheTable[idx][i].cnt = 0;
            return;
        }
    }

    int cntMax = -1;
    int cntMaxWay = 0;
    for (int i = 0; i < ways; i++) {
        if (cacheTable[idx][i].cnt > cntMax) {
            cntMax = cacheTable[idx][i].cnt;
            cntMaxWay = i;
        }
    }

    cacheTable[idx][cntMaxWay].valid = true;
    cacheTable[idx][cntMaxWay].tag = tag;
    cacheTable[idx][cntMaxWay].cnt = 0;
}

int main()
{
    struct data {
        int a = 0;
        int b = 0; //1110110100 111000
        char arr1[64] = {}; //1110110100 111100
        int c = 0;
        char arr2[4096] = {};
        int d = 0;
    } da;
    

    CheckCacheHit(&da.a, "a", sizeof(da.a));
    da.a = 1;
    CheckCacheHit(&da.b, "b", sizeof(da.b));
    da.b = 1;
    CheckCacheHit(&da.arr1, "arr1", sizeof(da.arr1));
    da.arr1[0] = 1;
    CheckCacheHit(&da.c, "c", sizeof(da.c));
    da.c = 1;
    CheckCacheHit(&da.arr2, "arr2", sizeof(da.arr2));
    da.arr2[0] = 1;
    CheckCacheHit(&da.d, "d", sizeof(da.d));
    da.d = 1;

    CheckCacheHit(&da.a, "a", sizeof(da.a));
    da.a = 1;
}

