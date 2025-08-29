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

using namespace std;

#define ENEMY_MAX		30
#define SHOT_MAX		50

struct PLAYER {
	int hp;

	int x;
	int y;
};

struct ENEMY {
	bool visible;
	int hp;

	int x;
	int y;
};

struct SHOT {
	bool visible;
	bool isEnemy;

	int x;
	int y;
};

enum Scene {
	TITLE,
	GAME,
	FINISH,
};

int g_stage = 1;
bool g_loadStageFlag;
PLAYER g_stPlayer;
ENEMY g_stEnemy[ENEMY_MAX];
SHOT g_stShot[SHOT_MAX];

void StartGameScene();

void MovePlayer();

void MoveEnemy();

void MoveShot();

void CreateShot(int x, int y, bool isEnemy);