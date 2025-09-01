#pragma once

#include <stdio.h>
#include <memory.h>
#include <Windows.h>

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> 

#include "Console.h"
#include "InputManager.h"
#include "ScreenManager.h"

using namespace std;

#define ENEMY_MAX		30
#define SHOT_MAX		50

struct PLAYER {
	bool visible = false;
	char shape;
	int hp;
	int speed;
	int moveCounter;
	
	int x;
	int y;
};

struct ENEMY {
	bool visible = false;
	char shape;
	int hp;
	int speed;
	int moveCounter;
	int pattern[10][2];

	int x;
	int y;
};

struct SHOT {
	bool visible = false;
	char shape;
	bool isEnemy;

	int x;
	int y;
};

extern int g_maxStage;
extern int g_currentStage;
extern int g_enemyCnt;
extern bool g_loadStageFlag;
extern PLAYER g_stPlayer;
extern ENEMY g_stEnemy[ENEMY_MAX];
extern SHOT g_stShot[SHOT_MAX];
extern bool g_isClear;

void StartStage();

void StageClear();

void GameClear();

void GameOver();

void MovePlayer();

void MoveEnemy();

void MoveShot();

void CreateShot(int x, int y, bool isEnemy, char shape);