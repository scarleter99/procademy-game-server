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
#include "DataManager.h"

using namespace std;

#define ENEMY_MAX		30
#define SHOT_MAX		50
#define SHOT_COUNT		100


struct PLAYER {
	bool visible = false;
	char symbol = '?';
	int hp = 0;
	int speed = 0;
	int speedCount = 0;
	
	int x = 0;
	int y = 0;
};

struct ENEMY {
	bool visible = false;
	char symbol = '?';
	int hp = 0;
	int shotCount = 0;
	int speed = 0;
	int speedCount = 0;
	int patternId = 0;
	int patternCount = 0;

	int x = 0;
	int y = 0;
};

struct SHOT {
	bool visible = false;
	char symbol = 0;
	bool isEnemy = 0;

	int x = 0;
	int y = 0;
};

extern int g_currentStage;
extern int g_enemyCount;
extern bool g_loadStageFlag;
extern bool g_isClear;
extern PLAYER g_stPlayer;
extern ENEMY g_stEnemy[ENEMY_MAX];
extern SHOT g_stShot[SHOT_MAX];

void StartStage();

void StageClear();

void GameClear();

void GameOver();

void MovePlayer();

void MoveEnemy();

void ShotEnemy();

void MoveShot();

void CreateShot(int x, int y, bool isEnemy, char symbol);