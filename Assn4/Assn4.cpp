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
    uint64_t idx = (addr >> offsetSize) & ((1ULL << idxSize) - 1);
    uint64_t tag = addr >> (offsetSize + idxSize);
    
    cout << name << " - addr: " << hex << /*bitset<16>*/(addr) << endl;
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
    struct Data {
        int a = 0;
        int b = 0;
        char arr1[64] = {};
        int c = 0;
        char arr2[4096] = {};
        int d = 0;
    } data;
    

    CheckCacheHit(&data.a, "a", sizeof(data.a));
    data.a = 1;
    CheckCacheHit(&data.b, "b", sizeof(data.b));
    data.b = 1;
    CheckCacheHit(&data.arr1, "arr1", sizeof(data.arr1));
    data.arr1[0] = 1;
    CheckCacheHit(&data.c, "c", sizeof(data.c));
    data.c = 1;
    CheckCacheHit(&data.arr2, "arr2", sizeof(data.arr2));
    data.arr2[0] = 1;
    CheckCacheHit(&data.d, "d", sizeof(data.d));
    data.d = 1;

    CheckCacheHit(&data.a, "a", sizeof(data.a));
    data.a = 1;
}

