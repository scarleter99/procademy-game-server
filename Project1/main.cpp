#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> 
#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <conio.h>
#include "Console.h"
#include "GameScene.h"
#include "InputManager.h"
#include "ScreenManager.h"

#pragma comment(lib, "winmm.lib")

using namespace std;

//--------------------------------------------------------------------
// GetAsyncKeyState(int iKey)  #include <Windows.h>
//
// 윈도우 API 로 키보드가 눌렸는지를 확인한다.
// 인자로 키보드 버튼에 대한 디파인 값을 넣으면 해당 키가 눌렸는지 (눌렸던적이 있는지) 를 확인 해준다.
// 모든 키에대한 확인이 가능하고, 논블럭 체크가 되므로 게임에서도 쓰기 좋다.
//
// Virtual-Key Codes
//
// VK_SPACE / VK_ESCAPE / VK_LEFT / VK_UP / 키보드 문자는 대문자 아스키 코드와 같음.
// winuser.h 파일에 위와 같이 디파인 되어 있다.
//
//
// GetAsyncKeyState(VK_LEFT) 호출시 결과값은
//
// 0x0001  > *이전 체크 이후 눌린적이 있음
// 0x8000  > 지금 눌려있음
// 0x8001  > *이전 체크 이후 눌린적도 있고 지금도 눌려 있음
//
// * 이전 체크라는건 이전에 GetAsyncKeyState 를 호출한 때를 말 한다.
// 
// 10프레임 짜리 게임이라면 1초에 10회의 키 체크를 하게 되므로 체크 간격은 20ms 가 된다.
// 빠른 커맨드 입력이 필요한 게임에서는 20ms 이내에 여러개의 키입력이 있다면 체크하지 못하는 키 입력이 발생 할 수 있다.
// 그래서 0x0001 비트에 대한 처리도 필요하다.
//

bool LoadData();

void UpdateTitle();

void UpdateGame();

void UpdateFinish();

int main()
{
	cs_Initial();

	//--------------------------------------------------------------------
	// 게임의 메인 루프
	// 이 루프가  1번 돌면 1프레임 이다.
	//--------------------------------------------------------------------

	g_currentScene = TITLE;
	
	timeBeginPeriod(1); // 해상도를 1ms로 낮춘다.

	DWORD startTick = timeGetTime();
	DWORD endTick = 0;
	while (1)
	{
		KeyProcess();
		Buffer_Clear();
		switch (g_currentScene) {
			case TITLE:
				UpdateTitle();
				break;
			case GAME:
				UpdateGame();
				break;
			case FINISH:
				UpdateFinish();
				break;
		}

		endTick = timeGetTime();
		long useTick = (long)(endTick - startTick);
		startTick += 20;
		if (20 - useTick > 0) {
			Sleep(20 - useTick);
			Buffer_Flip();
		}
	}

	return 0;
}

bool LoadData()
{
	ifstream fin("", ios::binary);
	if (!fin) {
		cout << "No File" << endl;
		return false;
	}

	vector<unsigned char> buf(
		(istreambuf_iterator<char>(fin)),
		istreambuf_iterator<char>()
	);

	fin.close();
	return true;
}

void UpdateTitle()
{
	char shootStr[] = "SHOOT!";
	for (int i = 0; i < strlen(shootStr); i++) {
		Sprite_Draw(38 + i, 8, shootStr[i]);
	}

	char pressSpaceBarStr[] = "Press [Space] To Start";
	for (int i = 0; i < strlen(pressSpaceBarStr); i++) {
		Sprite_Draw(30 + i,12, pressSpaceBarStr[i]);
	}

	if (g_spaceKeyDown) {
		g_currentScene = GAME;
		g_currentStage = 1;
		StartStage();
	}
}

void UpdateGame() 
{
	if (g_loadStageFlag) {
		LoadData();
	}

	MovePlayer();
	MoveEnemy();
	MoveShot();

	char stageStr[] = "Stage ";
	for (int i = 0; i < strlen(stageStr); i++) {
		Sprite_Draw(i, 0, stageStr[i]);
	}

	Sprite_Draw(strlen(stageStr), 0, g_currentStage + '0');

	for (int i = 0; i < dfSCREEN_WIDTH - 1; i++) {
		Sprite_Draw(i, 1, '-');
	}

	if (g_stPlayer.visible) {
		Sprite_Draw(g_stPlayer.x, g_stPlayer.y, g_stPlayer.shape);
	}
	
	for (const ENEMY& enemy : g_stEnemy) {
		if (enemy.visible == false) {
			continue;
		}
		Sprite_Draw(enemy.x, enemy.y, enemy.shape);
	}

	for (const SHOT& shot : g_stShot) {
		if (shot.visible == false) {
			continue;
		}

		Sprite_Draw(shot.x, shot.y, shot.shape);
	}
}

void UpdateFinish()
{
	if (g_isClear) {
		char finishStr[] = "Game Clear!";
		for (int i = 0; i < strlen(finishStr); i++) {
			Sprite_Draw(35 + i, 8, finishStr[i]);
		}
	}
	else {
		char finishStr[] = "Game Over!";
		for (int i = 0; i < strlen(finishStr); i++) {
			Sprite_Draw(35 + i, 8, finishStr[i]);
		}
	}
	

	char pressSpaceBarStr[] = "Press [Space] To Back To Title";
	for (int i = 0; i < strlen(pressSpaceBarStr); i++) {
		Sprite_Draw(26 + i, 12, pressSpaceBarStr[i]);
	}

	if (g_spaceKeyDown) {
		g_currentScene = TITLE;
	}
}
