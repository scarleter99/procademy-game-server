#include "DataManager.h"

StageInfo g_stageInfo;
vector<vector<EnemyData>> g_stageDatas;
unordered_map<int, EnemyInfo> g_enemyInfos;
unordered_map<int, Pattern>   g_patternInfos;

void loadStageInfo(const string& fileName) {
    ifstream fin(fileName);
    if (!fin) {
        return;
    }

    g_stageInfo.stageCount = 0;
    g_stageInfo.stageFiles.clear();

    fin >> g_stageInfo.stageCount;
    if (!fin) {
        return;
    }

    if (g_stageInfo.stageCount < 0) {
        g_stageInfo.stageCount = 0;
    }
    else if (g_stageInfo.stageCount > STAGE_MAX) {
        g_stageInfo.stageCount = STAGE_MAX;
    }

    g_stageInfo.stageFiles.resize(g_stageInfo.stageCount + 1);
    g_stageDatas.clear();
    g_stageDatas.resize(g_stageInfo.stageCount + 1);

    for (int i = 1; i <= g_stageInfo.stageCount; i++) {
        string stageFile;
        fin >> stageFile;
        if (!fin) {
            break;
        }

        g_stageInfo.stageFiles[i] = stageFile;
    }
}

void loadStageData(int stage) {
    if (stage < 1 || stage > g_stageInfo.stageCount) {
        return;
    }

    if (g_stageDatas[stage].empty() == false) {
        return;
    }

    const string& stageFileName = g_stageInfo.stageFiles[stage];
    if (stageFileName.empty()) {
        return;
    }

    ifstream fin(stageFileName);
    if (!fin) {
        return;
    }

    string header;
    getline(fin, header);

    int id = 0, x = 0, y = 0;
    int count = 0;
    while (fin >> id >> x >> y) {
        g_stageDatas[stage].push_back({ id, x, y });
        count++;
        if (count > ENEMY_MAX) {
            break;
        }
    }
}

void loadEnemyInfo(const string& fileName) {
    ifstream fin(fileName);
    if (!fin) {
        return;
    }

    g_enemyInfos.clear();

    string header;
    getline(fin, header);

    string line;
    while (getline(fin, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        EnemyInfo e{};
        if (ss >> e.id >> e.symbol >> e.hp >> e.speed >> e.patternId) {
            g_enemyInfos[e.id] = e;
        }
        else {
            continue;
        }
    }
}

void loadPatternInfo(const string& fileName) {
    ifstream fin(fileName);
    if (!fin) {
        return;
    }

    g_patternInfos.clear();

    string idline;
    while (getline(fin, idline)) {
        if (idline.empty() || idline[0] != '#') {
            continue;
        }
        int id = stoi(idline.substr(1));

        string countLine;
        if (!getline(fin, countLine)) {
            break;
        }
        int patternCount = stoi(countLine);

        Pattern p{};
        p.id = id;
        p.moveCount = 0;

        for (int i = 0; i < patternCount; i++) {
            string moveLine;
            if (!getline(fin, moveLine)) {
                break;
            }

            stringstream ss(moveLine);
            int dx, dy;
            if (!(ss >> dx >> dy)) {
                continue;
            }

            p.moves.push_back({ dx, dy });
            p.moveCount = p.moves.size();
        }

        g_patternInfos[id] = move(p);
    }
}

