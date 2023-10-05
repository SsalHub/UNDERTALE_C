#pragma once

#include <stdbool.h>
#include <Windows.h>

#include "BaseData.h"

typedef struct ScreenBuffer
{
	int currentIndex;
	HANDLE buffer[2];
} ScreenBuffer;

extern ScreenBuffer screenBuffer;

void initScreen();
void clearScreen();
void releaseScreen();
void swapScreenIndex();
void setColor(ConsoleColor, ConsoleColor);
void fillColorToScreen(ConsoleColor, ConsoleColor);
void printString(char*, COORD, bool, bool);
void printScreen(void (void));
void printBattleScreen(int);

/* Render Func */
void renderString(char*, COORD);
void renderBattleScreen(int);

