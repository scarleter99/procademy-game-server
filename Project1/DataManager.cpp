#include "DataManager.h"

StageInfo g_stageInfo;
vector<vector<EnemyData>> g_stageData;
vector<EnemyInfo> g_enemyInfos;
vector<Pattern> g_patterns;

void loadStageInfo(const string& fileName) {
    ifstream fin(fileName);
    if (!fin) {
        cerr << "Failed to open StageInfo file: " << fileName << endl;
        return;
    }

    // 스테이지 개수 읽기
    fin >> g_stageInfo.stageCount;

    g_stageInfo.stageFiles.clear();
    g_stageInfo.stageFiles.reserve(g_stageInfo.stageCount);

    // 파일 이름들 읽기
    for (int i = 0; i < g_stageInfo.stageCount; i++) {
        string stageFile;
        fin >> stageFile;
        if (!stageFile.empty()) {
            g_stageInfo.stageFiles.push_back(stageFile);
        }
    }
}

void loadStageData() {
    for (int i = 0; i < g_stageInfo.stageCount; i++) {
        const string& stageFile = g_stageInfo.stageFiles[i];
        ifstream fin(stageFile);
        if (!fin) {
            cerr << "Failed to open StageData file: " << stageFile << endl;
            continue;
        }

        string header;
        getline(fin, header);

        vector<EnemyData>& enemies = g_stageData[i];
        enemies.clear();

        int id, x, y;
        while (fin >> id >> x >> y) {
            enemies.push_back(EnemyData{ id, x, y });
        }
    }
}

void loadEnemyInfo(const string& fileName) {
    ifstream fin(fileName);
    if (!fin) {
        cerr << "Failed to open " << fileName << endl;
        return;
    }

    string line;
    bool readingHeader = true;
    Pattern* currentPattern = nullptr;

    while (getline(fin, line)) {
        if (line.empty()) continue;

        if (readingHeader) {
            // 첫 줄 "ID 모양 HP 속도 패턴" 건너뜀
            if (line.find("ID") != string::npos) {
                continue;
            }
            // 패턴 시작(#) 만나면 헤더 부분 끝
            if (line[0] == '#') {
                readingHeader = false;
            }
            else {
                // EnemyInfo 읽기
                stringstream ss(line);
                EnemyInfo e;
                ss >> e.id >> e.symbol >> e.hp >> e.speed >> e.patternId;
                g_enemyInfos.push_back(e);
                continue;
            }
        }

        // 패턴 부분
        if (line[0] == '#') {
            // 새 패턴 시작
            int pid = stoi(line.substr(1));
            g_patterns.push_back(Pattern{ pid });
            currentPattern = &g_patterns.back();
        }
        else if (currentPattern) {
            // 방향 데이터
            stringstream ss(line);
            int dx, dy;
            ss >> dx >> dy;
            currentPattern->moves.push_back({ dx, dy });
        }
    }
}