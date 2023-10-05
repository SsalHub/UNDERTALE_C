#include "../Header/ScreenRender.h"

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "../Header/BaseData.h"

ScreenBuffer screenBuffer;

void initScreen()
{
	char screenInitCommand[64] = "";
	sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	
	/* Cursor */
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	screenBuffer.currentIndex = 0;
	screenBuffer.buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleCursorInfo(screenBuffer.buffer[0], &cursor);
	screenBuffer.buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleCursorInfo(screenBuffer.buffer[1], &cursor);
	
	/* Font */
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);
	font.nFont = 1;
    font.dwFontSize.X = _FONT_WIDTH_;
    font.dwFontSize.Y = _FONT_HEIGHT_;
    
    SetCurrentConsoleFontEx(screenBuffer.buffer[0], true, &font);
    SetCurrentConsoleFontEx(screenBuffer.buffer[1], true, &font);
	system(screenInitCommand);
}

void clearScreen()	
{
	COORD zero = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_*2)*_SCREEN_HEIGHT_+1];
	int i, j, currIdx;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], _WHITE_ | (_BLACK_ << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		currIdx =  i * (_SCREEN_WIDTH_ + 1);
		memset(bufferString+currIdx, ' ', sizeof(char));
		bufferString[currIdx+_SCREEN_WIDTH_] = '\n';
	}
	bufferString[(_SCREEN_WIDTH_+1)*_SCREEN_HEIGHT_] = '\0';
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
}

void releaseScreen()
{
	CloseHandle(screenBuffer.buffer[0]);
	CloseHandle(screenBuffer.buffer[1]);
}

void swapScreenIndex()
{
	screenBuffer.currentIndex = !(screenBuffer.currentIndex);
}

void setColor(ConsoleColor bColor, ConsoleColor tColor)
{
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
}

void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor)
{	
	COORD zero = { 0, 0 };
	DWORD dw;
	char bufferString[(_SCREEN_WIDTH_+1)*_SCREEN_HEIGHT_+1] = "";
	int i, j, currIdx;
	
	SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], zero);
	SetConsoleTextAttribute(screenBuffer.buffer[screenBuffer.currentIndex], tColor | (bColor << 4));
	bufferString[0] = '\0';
	for (i = 0; i < _SCREEN_HEIGHT_; i++)
	{
		currIdx =  i * (_SCREEN_WIDTH_ + 1);
		memset(bufferString+currIdx, ' ', sizeof(char));
		bufferString[currIdx+_SCREEN_WIDTH_] = '\n';
	}
	bufferString[(_SCREEN_WIDTH_+1)*_SCREEN_HEIGHT_] = '\0';
	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], bufferString, strlen(bufferString), &dw, NULL);
}


/* Print Func */


void printString(char* str, COORD pos, bool bClear, bool bSwap)
{
	DWORD dw;
	char* nextLine;
	
	if (bClear) clearScreen();
	
	nextLine = strtok(str, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		pos.Y++;
	}
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	if (bSwap) swapScreenIndex();
}

void printScreen(void render(void))
{
	clearScreen();
	render();
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
}

void printBattleScreen(int selected)
{
	clearScreen();
	renderBattleScreen(selected);
	SetConsoleActiveScreenBuffer(screenBuffer.buffer[screenBuffer.currentIndex]);
	swapScreenIndex();
}


/* Render Func */


void renderString(char* str, COORD pos)
{
	DWORD dw;
	char* nextLine;
	int originY = pos.Y;
	
	nextLine = strtok(str, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(screenBuffer.buffer[screenBuffer.currentIndex], pos);
		WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		pos.Y++;
	}
//	WriteFile(screenBuffer.buffer[screenBuffer.currentIndex], str, strlen(str), &dw, NULL);
	pos.Y = originY;
}

void renderBattleScreen(int selected)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_, tSelColor = _YELLOW_;
	COORD choiceBoxPos[4];
	DWORD dw;
	char choiceBox[4][_SCREEN_WIDTH_*6];
	const int offset = 0;
	int i, boxWidth, choiceBoxY = _SCREEN_HEIGHT_ * 0.6;
	
	fillColorToScreen(bColor, tColor);
	
	/* Initialize Chioce boxes. */
	i = 0;
	choiceBox[i][0] = '\0';
	strcat(choiceBox[i], "旨收收收收旬 \n");
	boxWidth = strlen(choiceBox[i]);
	strcat(choiceBox[i], "早 FIGHT 早 \n");
	strcat(choiceBox[i], "曲收收收收旭 \n");
	choiceBoxPos[i].X = (_SCREEN_WIDTH_ * 0.1) + (((boxWidth * 2) + offset) * i);
	choiceBoxPos[i].Y = choiceBoxY;
	
	i++;
	choiceBox[i][0] = '\0';
	strcat(choiceBox[i], "旨收收收收旬 \n");
	boxWidth = strlen(choiceBox[i]);
	strcat(choiceBox[i], "早  ACT  早 \n");
	strcat(choiceBox[i], "曲收收收收旭 \n");
	choiceBoxPos[i].X = (_SCREEN_WIDTH_ * 0.1) + (((boxWidth * 2) + offset) * i);
	choiceBoxPos[i].Y = choiceBoxY;
	
	i++;
	choiceBox[i][0] = '\0';
	strcat(choiceBox[i], "旨收收收收旬 \n");
	boxWidth = strlen(choiceBox[i]);
	strcat(choiceBox[i], "早  ACT  早 \n");
	strcat(choiceBox[i], "曲收收收收旭 \n");
	choiceBoxPos[i].X = (_SCREEN_WIDTH_ * 0.1) + (((boxWidth * 2) + offset) * i);
	choiceBoxPos[i].Y = choiceBoxY;
	
	i++;
	choiceBox[i][0] = '\0';
	strcat(choiceBox[i], "旨收收收收旬 \n");
	boxWidth = strlen(choiceBox[i]);
	strcat(choiceBox[i], "早  ITEM 早 \n");
	strcat(choiceBox[i], "曲收收收收旭 \n");
	choiceBoxPos[i].X = (_SCREEN_WIDTH_ * 0.1) + (((boxWidth * 2) + offset) * i);
	choiceBoxPos[i].Y = 0;
	
	for (i = 0; i < 4; i++)
	{
		if (i == selected)
			setColor(bColor, tSelColor);
		else
			setColor(bColor, tColor);
		renderString(choiceBox[i], choiceBoxPos[i]);
	}
}
