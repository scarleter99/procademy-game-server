#include <iostream>
#include <conio.h>
using namespace std;

int n = 9;
const int g_Timing[] = { 5, 10, 14, 17, 20, 25, 29, 31, 33 };
string result[] = { "", "", "", "", "", "", "", "", "" };
int nowIdx = 0;

void timingGame() {
    while (1) {
        system("cls");
        float now = (float)clock() / CLOCKS_PER_SEC;
        cout << now << " Sec" << endl << endl;
        
        if (nowIdx < n) {
            float diff = now - g_Timing[nowIdx];
            if (_kbhit()) {
                _getch();
                if (abs(diff) > 1.0f) {
                    result[nowIdx] = "Fail";
                    nowIdx++;
                }
                else if (abs(diff) > 0.75f) {
                    result[nowIdx] = "Bad";
                    nowIdx++;
                }
                else if (abs(diff) > 0.5f) {
                    result[nowIdx] = "Not Good";
                    nowIdx++;
                }
                else if (abs(diff) > 0.25f) {
                    result[nowIdx] = "Good!";
                    nowIdx++;
                }
                else {
                    result[nowIdx] = "Great!!";
                    nowIdx++;
                }
            }

            if (diff > 1.0f) {
                result[nowIdx] = "Fail";
                nowIdx++;
            }
        }

        for (int i = 0; i < n; i++) {
            cout << g_Timing[i] << " Sec: " << result[i] << endl;
        }

        cout << endl;
    }
}

int main()
{
    timingGame();
}
