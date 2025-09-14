#include <windows.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string> // string 사용을 위해 추가
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
    bool			lFlag;				// 프로파일의 사용 여부. (배열시에만)
    string			szName;				// 프로파일 샘플 이름.

    LARGE_INTEGER	lStartTime;			// 프로파일 샘플 실행 시간.

    __int64			iTotalTime;			// 전체 사용시간 카운터 Time.
    __int64			iMin[2];			// 최소 사용시간 카운터 Time.
    __int64			iMax[2];			// 최대 사용시간 카운터 Time.

    __int64			iCall;				// 누적 호출 횟수.

};

void ProfileBegin(const string& szName);
void ProfileEnd(const string& szName);
void ProfileDataOutText(const string& szFileName);
void ProfileReset(void);

inline LARGE_INTEGER g_freq;
inline PROFILE_DATA g_profileDatas[20];