#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> 
#include <stdio.h>
#include <memory.h>
#include <Windows.h>
#include <conio.h>

using namespace std;

extern bool g_upKey;
extern bool g_downKey;
extern bool g_rightKey;
extern bool g_leftKey;
extern bool g_spaceKey;
extern bool g_spaceKeyDown;
extern bool g_zKey;
extern bool g_zKeyDown;

void KeyProcess();