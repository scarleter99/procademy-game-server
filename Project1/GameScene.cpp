#include "GameScene.h"

int g_maxStage = 2;
int g_currentStage = 1;
int g_enemyCnt;
bool g_loadStageFlag;
PLAYER g_stPlayer;
ENEMY g_stEnemy[ENEMY_MAX];
SHOT g_stShot[SHOT_MAX];
bool g_isClear;

void StartStage()
{
	g_stPlayer.visible = true;
	g_stPlayer.symbol = '#';
	g_stPlayer.hp = 3;
	g_stPlayer.speed = 2;
	g_stPlayer.x = 40;
	g_stPlayer.y = 20;

	g_stEnemy[0].visible = true;
	g_stEnemy[0].symbol = '@';
	g_stEnemy[0].hp = 1;
	g_stEnemy[0].speed = 3;
	g_stEnemy[0].x = 30;
	g_stEnemy[0].y = 5;

	g_stEnemy[1].visible = true;
	g_stEnemy[1].symbol = '@';
	g_stEnemy[1].hp = 1;
	g_stEnemy[1].speed = 3;
	g_stEnemy[1].x = 40;
	g_stEnemy[1].y = 5;

	g_stEnemy[2].visible = true;
	g_stEnemy[2].symbol = '@';
	g_stEnemy[2].hp = 1;
	g_stEnemy[2].speed = 3;
	g_stEnemy[2].x = 50;
	g_stEnemy[2].y = 5;

	g_enemyCnt = 3;
	g_isClear = false;
}

void StageClear()
{
	g_currentStage++;
	if (g_currentStage > g_maxStage) {
		GameClear();
		return;
	}

	StartStage();
}

void GameClear()
{
	g_isClear = true;
	g_stPlayer.visible = false;
	g_currentScene = FINISH;
}

void GameOver()
{
	g_stPlayer.visible = false;
	g_currentScene = FINISH;
}

void MovePlayer()
{
	if (g_spaceKeyDown) {
		CreateShot(g_stPlayer.x, g_stPlayer.y, false, '!');
	}

	g_stPlayer.moveCounter++;
	if (g_stPlayer.moveCounter < g_stPlayer.speed)
		return;

	g_stPlayer.moveCounter = 0;
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
		g_stPlayer.x = max(2, g_stPlayer.x - 1);
	}
}

void MoveEnemy()
{

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
				g_stPlayer.hp--;
				if (g_stPlayer.hp <= 0) {
					GameOver();
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
						g_enemyCnt--;
						if (g_enemyCnt == 0) {
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
		shot.x = x;
		shot.y = y;
		break;
	}
}
