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

    // �������� ���� �б�
    fin >> g_stageInfo.stageCount;

    g_stageInfo.stageFiles.clear();
    g_stageInfo.stageFiles.reserve(g_stageInfo.stageCount);

    // ���� �̸��� �б�
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
            // ù �� "ID ��� HP �ӵ� ����" �ǳʶ�
            if (line.find("ID") != string::npos) {
                continue;
            }
            // ���� ����(#) ������ ��� �κ� ��
            if (line[0] == '#') {
                readingHeader = false;
            }
            else {
                // EnemyInfo �б�
                stringstream ss(line);
                EnemyInfo e;
                ss >> e.id >> e.symbol >> e.hp >> e.speed >> e.patternId;
                g_enemyInfos.push_back(e);
                continue;
            }
        }

        // ���� �κ�
        if (line[0] == '#') {
            // �� ���� ����
            int pid = stoi(line.substr(1));
            g_patterns.push_back(Pattern{ pid });
            currentPattern = &g_patterns.back();
        }
        else if (currentPattern) {
            // ���� ������
            stringstream ss(line);
            int dx, dy;
            ss >> dx >> dy;
            currentPattern->moves.push_back({ dx, dy });
        }
    }
}