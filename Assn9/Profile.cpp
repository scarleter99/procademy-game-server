#include "Profile.h"

void ProfileBegin(const string& szName) {
    if (g_freq.QuadPart == 0) {
        QueryPerformanceFrequency(&g_freq);
    }

    PROFILE_DATA* pd = nullptr;
    for (PROFILE_DATA& i : g_profileDatas) {
        if (i.lFlag == false) {
            continue;
        }

        if (i.szName == szName) {
            pd = &i;
            break;
        }
    }

    if (pd == nullptr) {
        for (PROFILE_DATA& i : g_profileDatas) {
            if (i.lFlag) {
                continue;
            }

            i.lFlag = true;
            i.szName = szName;

            i.iTotalTime = 0;
            fill(begin(i.iMin), end(i.iMin), INT64_MAX);
            fill(begin(i.iMax), end(i.iMax), 0);
            i.iCall = 0;

            pd = &i;
            break;
        }
    }

    if (pd == nullptr) {
        return;
    }

    QueryPerformanceCounter(&pd->lStartTime);
}

void ProfileEnd(const string& szName) {
    PROFILE_DATA* pd = nullptr;
    for (PROFILE_DATA& i : g_profileDatas) {
        if (i.lFlag == false) {
            continue;
        }

        if (i.szName == szName) {
            pd = &i;
            break;
        }
    }

    if (pd == nullptr) {
        __debugbreak();
        return;
    }

    LARGE_INTEGER lEndTime;
    QueryPerformanceCounter(&lEndTime);

    __int64 elapsedClocks = lEndTime.QuadPart - pd->lStartTime.QuadPart;

    pd->iTotalTime += elapsedClocks;

    if (elapsedClocks < pd->iMin[0]) {
        pd->iMin[1] = pd->iMin[0];
        pd->iMin[0] = elapsedClocks;
    }
    else if (elapsedClocks < pd->iMin[1]) {
        pd->iMin[1] = elapsedClocks;
    }

    if (elapsedClocks > pd->iMax[0]) {
        pd->iMax[1] = pd->iMax[0];
        pd->iMax[0] = elapsedClocks;
    }
    else if (elapsedClocks > pd->iMax[1]) {
        pd->iMax[1] = elapsedClocks;
    }

    pd->iCall++;
}

void ProfileDataOutText(const string& filename)
{
    ofstream fout(filename);
    if (fout.is_open() == false) {
        return;
    }

    fout << "           Name  |     Average  |        Min   |        Max   |      Call |\n";
    fout << "---------------------------------------------------------------------------\n";

    for (int i = 0; i < 20; ++i)
    {
        const PROFILE_DATA& pd = g_profileDatas[i];
        if (pd.lFlag == 0 || pd.iCall == 0) {
            continue;
        }

        double avg = (double)pd.iTotalTime / pd.iCall * 1000000.0 / g_freq.QuadPart;
        double min = (double)pd.iMin[0] * 1000000.0 / g_freq.QuadPart;
        double max = (double)pd.iMax[0] * 1000000.0 / g_freq.QuadPart;
        cout << g_freq.QuadPart << endl;
        fout << setw(15) << pd.szName << " | "
            << setw(10) << fixed << setprecision(1) << avg << "§Á | "
            << setw(10) << fixed << setprecision(1) << min << "§Á | "
            << setw(10) << fixed << setprecision(1) << max << "§Á | "
            << setw(10) << pd.iCall << "\n";
    }

    fout << "---------------------------------------------------------------------------\n";
    fout.close();
}

void ProfileReset()
{
    for (PROFILE_DATA& i : g_profileDatas) {
        i.lFlag = false;
    }
}