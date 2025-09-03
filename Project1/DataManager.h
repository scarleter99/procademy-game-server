#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>

#define STAGE_MAX           100
#define ENEMY_MAX		    30

using namespace std;

struct StageInfo {
    int stageCount = 0;
    vector<string> stageFiles;
};

struct EnemyData {
    int id = 0;
    int x = 0;
    int y = 0;
};

struct EnemyInfo {
    int id = 0;
    char symbol = '?';
    int hp = 0;
    int speed = 0;
    int patternId = 0;
};

struct Pattern {
    int id;
    int patternCount;
    vector<pair<int, int>> moves;
};

extern StageInfo g_stageInfo;
extern vector<vector<EnemyData>> g_stageDatas;
extern unordered_map<int, EnemyInfo> g_enemyInfos;
extern unordered_map<int, Pattern> g_patternInfos;

void loadStageInfo(const string& fileName);

void loadStageData(int stage);

void loadEnemyInfo(const string& fileName);

void loadPatternInfo(const string& fileName);