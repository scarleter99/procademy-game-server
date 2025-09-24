#include <iostream>

#include <windows.h>
#include "Profile.h"

#define PROFILE

void CoutTest() {
    PRO_BEGIN("CoutTest");
    cout << "CoutTest" << endl;
    PRO_END("CoutTest");
}

void CoutTest2() {
    PRO_BEGIN("CoutTest2");
    for (int i = 0; i < 5; i++) {
        CoutTest();
    }
    PRO_END("CoutTest2");
}

int main()
{
    for (int i = 0; i < 1000; i++) {
        CoutTest2();
    }

    profileDataOutText("Data/ProfileData.txt");
    ProfileReset();
}
