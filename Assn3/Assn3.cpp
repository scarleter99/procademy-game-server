#include <iostream>
#include <string>
#include <stack>
#include <queue>
#include <tuple>
#include <windows.h>
#include <cstdlib>
using namespace std;

string arr[9] = {
        " 00     000     00",
        " 000000000000  000",
        "00000  00000000000",
        "0000   00000000000",
        "000 0  00000000000",
        "00  0  00000000000",
        "00 00  000000000  ",
        " 0  0 00000000    ",
        "      000000      "
};

const int dy[] = {0, -1, 0, 1};
const int dx[] = {-1, 0, 1, 0};

int n = 9;
int m = 18;

void printArr(int time) {
    system("cls");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << arr[i][j];
        }
        cout << endl;
    }

    if (time > 0) {
        Sleep(time);
    }
}

void dfs1(int y, int x){
    arr[y][x] = '.';
    printArr(0);

    for (int i = 0; i < 4; i++) {
        int ny = y + dy[i];
        int nx = x + dx[i];

        if (ny < 0 || nx < 0 || ny >= n || nx >= m) {
            continue;
        }

        if (arr[ny][nx] == ' ' || arr[ny][nx] == '.') {
            continue;
        }

        dfs1(ny, nx);
    }
}

void dfs2() {
    stack<pair<int, int>> st;
    st.push({ 0, 0 });
    arr[0][0] = '.';
    
    int y, x;
    while (st.size()) {
        tie(y, x) = st.top();
        st.pop();
        printArr(0);

        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (ny < 0 || nx < 0 || ny >= n || nx >= m) {
                continue;
            }

            if (arr[ny][nx] == ' ' || arr[ny][nx] == '.') {
                continue;
            }

            arr[ny][nx] = '.';
            st.push({ ny, nx });
        }
    }
}

void bfs() {
    queue<pair<int, int>> q;
    q.push({ 0, 0 });
    arr[0][0] = '.';

    int y, x;
    while (q.size()) {
        tie(y, x) = q.front();
        q.pop();
        printArr(0);

        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];

            if (ny < 0 || nx < 0 || ny >= n || nx >= m) {
                continue;
            }

            if (arr[ny][nx] == ' ' || arr[ny][nx] == '.') {
                continue;
            }

            arr[ny][nx] = '.';
            q.push({ ny, nx });
        }
    }
}

int main()
{
    printArr(1000);
    int s = GetTickCount();

    dfs1(0, 0);
    //dfs2();
    //bfs();

    cout << GetTickCount() - s << endl;

    return 0;
}

// 00     000     00
// 000000000000  000
//00000  00000000000
//0000   00000000000
//000 0  00000000000
//00  0 00000000000
//00 00 000000000
// 0  0 00000000
//      000000