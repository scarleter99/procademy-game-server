#include "InputManager.h"

bool g_upKey = false;
bool g_downKey = false;
bool g_rightKey = false;
bool g_leftKey = false;
bool g_spaceKey = false;
bool g_spaceKeyDown = false;

void KeyProcess()
{
	g_upKey = false;
	g_downKey = false;
	g_rightKey = false;
	g_leftKey = false;
	g_spaceKeyDown = false;

	if (GetAsyncKeyState(VK_UP)) {
		g_upKey = true;
	}

	if (GetAsyncKeyState(VK_DOWN)) {
		g_downKey = true;
	}

	if (GetAsyncKeyState(VK_RIGHT)) {
		g_rightKey = true;
	}

	if (GetAsyncKeyState(VK_LEFT)) {
		g_leftKey = true;
	}

	if (GetAsyncKeyState(VK_SPACE)) {
		if (g_spaceKey == false) {
			g_spaceKeyDown = true;
		}

		g_spaceKey = true;
	}
	else {
		g_spaceKey = false;
	}
}