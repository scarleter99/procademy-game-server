#define PROFILE

#include <iostream>
#include "Profile.h"

void SleepTest() {
    PRO_BEGIN(L"SleepTest");
    cout << "SleepTest" << endl;
    PRO_END(L"SleepTest");
}

void SleepTest2() {
    PRO_BEGIN(L"SleepTest2");
    for (int i = 0; i < 10; i++) {
        SleepTest();
    }
    PRO_END(L"SleepTest2");
}

int main()
{
    for (int i = 0; i < 1000; i++) {
        SleepTest2();
    }

    ProfileDataOutText(L"Data/ProfileData.txt");
    ProfileReset();
}
