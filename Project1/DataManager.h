#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct StageInfo {
    int stageCount;
    vector<string> stageFiles;
};

struct EnemyData {
    int id;
    int x;
    int y;
};

struct EnemyInfo {
    int id;
    char symbol;
    int hp;
    int speed;
    int patternId;
};

struct Pattern {
    int id;
    vector<pair<int, int>> moves;
};

extern StageInfo g_stageInfo;
extern vector<vector<EnemyData>> g_stageData;
extern vector<EnemyInfo> g_enemyInfos;
extern vector<Pattern> g_patterns;

void loadStageInfo(const string& fileName);

void loadStageData();

void loadEnemyInfo(const string& fileName);