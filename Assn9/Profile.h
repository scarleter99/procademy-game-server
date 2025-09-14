#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string> // string ����� ���� �߰�
using namespace std;

#ifdef PROFILE
#define PRO_BEGIN(TagName)	ProfileBegin(TagName)
#define PRO_END(TagName)	ProfileEnd(TagName)
#else
#define PRO_BEGIN(TagName)
#define PRO_END(TagName)
#endif

struct PROFILE_DATA
{
    bool			lFlag;				// ���������� ��� ����. (�迭�ÿ���)
    string			szName;				// �������� ���� �̸�.

    LARGE_INTEGER	lStartTime;			// �������� ���� ���� �ð�.

    __int64			iTotalTime;			// ��ü ���ð� ī���� Time.
    __int64			iMin[2];			// �ּ� ���ð� ī���� Time.
    __int64			iMax[2];			// �ִ� ���ð� ī���� Time.

    __int64			iCall;				// ���� ȣ�� Ƚ��.

};

void ProfileBegin(const string& szName);
void ProfileEnd(const string& szName);
void ProfileDataOutText(const string& szFileName);
void ProfileReset(void);

inline LARGE_INTEGER g_freq;
inline PROFILE_DATA g_profileDatas[20];