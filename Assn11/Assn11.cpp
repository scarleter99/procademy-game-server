#include "Object.h"
#include <iostream>
#include <Windows.h>
#include <synchapi.h>
#include <conio.h>

using namespace std;

#define MAX_STAR_NUM 20

BaseObject* stars[MAX_STAR_NUM] = { nullptr };

void KeyProcess();
void Update();
void Render();

int main()
{
    while (1)
    {
        KeyProcess();

        Update();

        system("cls");
        Render();

        Sleep(30);
    }
}

void KeyProcess()
{
    if (_kbhit()) {
        _getch();
        for (int i = 0; i < MAX_STAR_NUM; i++) {
            if (stars[i] == nullptr) {
                switch (rand() % 3) {
                case 0:
                    stars[i] = new OneStar();
                    break;

                case 1:
                    stars[i] = new TwoStar();
                    break;

                case 2:
                    stars[i] = new ThreeStar();
                    break;
                }
                break;
            }
        }
    }
}

void Update()
{
    for (int i = 0; i < MAX_STAR_NUM; i++) {
        if (stars[i] != nullptr) {
            stars[i]->Update();
            if (stars[i]->GetDelete()) {
                delete stars[i];
                stars[i] = nullptr;
            }
        }
    }

}

void Render()
{
    for (int i = 0; i < MAX_STAR_NUM; i++) {
        if (stars[i] != nullptr) {
            stars[i]->Render();
        }

        cout << endl;
    }
}