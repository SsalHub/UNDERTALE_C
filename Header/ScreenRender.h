#pragma once

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <stdbool.h>
#include <windows.h>
#include <wincon.h>

#include "BaseData.h"
#include "ExceptionHandler.h"

#define _CURRENT_SCREEN_ screenBuffer.buffer[screenBuffer.currentIndex]

typedef struct ScreenBuffer
{
	int currentIndex;
	HANDLE buffer[2];
} ScreenBuffer;

typedef struct ScreenInfo
{
	int width;
	int height;
	int areaForStrlen;
} ScreenInfo;

extern ScreenBuffer screenBuffer;
extern ScreenInfo screenInfo;

/* Basic Screen Func */
void initScreen();
void vibrateScreen();
void clearScreen();
void releaseScreen();
void swapScreenIndex();
void setColor(ConsoleColor, ConsoleColor);
void fillColorToScreen(ConsoleColor, ConsoleColor);

/* Print Func */
void printScreen(void (void));
void printMainMenuScreen(int);
void printBattleScreen(int, int);

/* Render Func */
void renderString(char*, COORD);
void renderMainMenuScreen(int);
void renderTestScreen();
void renderBattleEnemy();
void renderBattleExplainBox(int);
void renderBattleChoiceBoxes(int);

