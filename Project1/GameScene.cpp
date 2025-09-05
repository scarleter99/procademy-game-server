#include "GameScene.h"

int g_currentStage = 1;
int g_enemyCount = 0;
bool g_loadStageFlag = false;
bool g_isClear = false;
PLAYER g_stPlayer;
ENEMY g_stEnemy[ENEMY_MAX];
SHOT g_stShot[SHOT_MAX];

void StartStage()
{
	loadStageData(g_currentStage);

	g_stPlayer.visible = true;
	g_stPlayer.symbol = '#';
	g_stPlayer.hp = 3;
	g_stPlayer.speed = 2;
	g_stPlayer.x = 40;
	g_stPlayer.y = 20;

	for (size_t i = 0; i < g_stageDatas[g_currentStage].size(); i++) {
		int id = g_stageDatas[g_currentStage][i].id;
		g_stEnemy[i].x = g_stageDatas[g_currentStage][i].x;
		g_stEnemy[i].y = g_stageDatas[g_currentStage][i].y;

		g_stEnemy[i].visible = true;
		g_stEnemy[i].symbol = g_enemyInfos[id].symbol;
		g_stEnemy[i].hp = g_enemyInfos[id].hp;
		g_stEnemy[i].shotCount = rand() % SHOT_COUNT;
		g_stEnemy[i].speed = g_enemyInfos[id].speed;
		g_stEnemy[i].speedCount = rand() % g_enemyInfos[id].speed;
		g_stEnemy[i].patternId = g_enemyInfos[id].patternId;
		g_stEnemy[i].patternCount = 0;
	}

	g_enemyCount = g_stageDatas[g_currentStage].size();
	g_loadStageFlag = true;
}

void StageClear()
{
	g_currentStage++;
	g_loadStageFlag = false;
	if (g_currentStage > g_stageInfo.stageCount) {
		GameFinish(true);
		return;
	}

	g_stPlayer.visible = false;
	for (ENEMY& enemy : g_stEnemy) {
		enemy.visible = false;
	}

	for (SHOT& shot : g_stShot) {
		shot.visible = false;
	}
}

void GameFinish(bool isClear)
{
	g_isClear = isClear;
	g_currentScene = FINISH;

	g_stPlayer.visible = false;
	for (ENEMY& enemy : g_stEnemy) {
		enemy.visible = false;
	}

	for (SHOT& shot : g_stShot) {
		shot.visible = false;
	}
}

void MovePlayer()
{
	if (g_zKeyDown) {
		CreateShot(g_stPlayer.x, g_stPlayer.y, false, '!');
	}

	if (g_stPlayer.speedCount < g_stPlayer.speed) {
		g_stPlayer.speedCount++;
		return;
	}
	g_stPlayer.speedCount = 0;

	if (g_upKey) {
		g_stPlayer.y = max(2, g_stPlayer.y - 1);
	}

	if (g_downKey) {
		g_stPlayer.y = min(dfSCREEN_HEIGHT - 1, g_stPlayer.y + 1);
	}

	if (g_rightKey) {
		g_stPlayer.x = min(dfSCREEN_WIDTH - 2, g_stPlayer.x + 1);
	}

	if (g_leftKey) {
		g_stPlayer.x = max(0, g_stPlayer.x - 1);
	}
}

void MoveEnemy()
{
	for (ENEMY& enemy : g_stEnemy) {
		if (enemy.visible == false) {
			continue;
		}

		if (enemy.speedCount < enemy.speed) {
			enemy.speedCount++;
			continue;
		}
		enemy.speedCount = 0;

		pair<int, int> pattern = g_patternInfos[enemy.patternId].moves[enemy.patternCount];
		enemy.patternCount = enemy.patternCount + 1 < g_patternInfos[enemy.patternId].patternCount
			? enemy.patternCount + 1 : 0;

		int ny = min(dfSCREEN_HEIGHT - 1, enemy.y + pattern.second);
		enemy.y = max(2, ny);
		int nx = min(dfSCREEN_WIDTH - 2, enemy.x + pattern.first);
		enemy.x = max(0, nx);
	}
}

void ShotEnemy()
{
	for (ENEMY& enemy : g_stEnemy) {
		if (enemy.visible == false) {
			continue;
		}

		if (enemy.shotCount < SHOT_COUNT) {
			enemy.shotCount++;
			continue;
		}
		enemy.shotCount = 0;

		CreateShot(enemy.x, enemy.y, true, '!');
	}
}

void MoveShot()
{
	for (SHOT& shot : g_stShot) {
		if (shot.visible == false) {
			continue;
		}

		if (shot.isEnemy) {
			shot.y++;
			if (shot.y > dfSCREEN_HEIGHT - 1) {
				shot.visible = false;
				continue;
			}

			if (g_stPlayer.visible && shot.x == g_stPlayer.x && shot.y == g_stPlayer.y) {
				shot.visible = false;
				//g_stPlayer.hp--;
				if (g_stPlayer.hp <= 0) {
					GameFinish(false);
					break;
				}

				break;
			}
		}
		else {
			shot.y--;
			if (shot.y < 2) {
				shot.visible = false;
				continue;
			}

			for (ENEMY& enemy : g_stEnemy) {
				if (enemy.visible && shot.x == enemy.x && shot.y == enemy.y) {
					shot.visible = false;
					enemy.hp--;
					if (enemy.hp <= 0) {
						enemy.visible = false;
						g_enemyCount--;
						if (g_enemyCount <= 0) {
							StageClear();
							break;
						}
					}

					break;
				}
				
			}
		}
	}
}

void CreateShot(int x, int y, bool isEnemy, char symbol)
{
	for (SHOT& shot : g_stShot) {
		if (shot.visible) {
			continue;
		}

		shot.visible = true;
		shot.symbol = '!';
		shot.isEnemy = isEnemy;
		shot.x = x;
		shot.y = y;
		break;
	}
}
