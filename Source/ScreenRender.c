#include "../Header/ScreenRender.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "../Header/UtilData.h"
#include "../Header/BaseData.h"

ScreenBuffer screenBuffer;
CurrentScreenInfo currentScreenInfo;

void initScreen()
{
	char screenInitCommand[64] = "";
	sprintf(screenInitCommand, "mode con:cols=%d lines=%d", _SCREEN_WIDTH_, _SCREEN_HEIGHT_);
	CONSOLE_SCREEN_BUFFER_INFO info;
	
	/* Basic initialization */
	system(screenInitCommand);
	screenBuffer.currentIndex = 0;
	screenBuffer.buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screenBuffer.buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	
	GetConsoleScreenBufferInfo(_CURRENT_SCREEN_, &info);
    currentScreenInfo.width = info.srWindow.Right - info.srWindow.Left + 1;
    currentScreenInfo.height = info.srWindow.Bottom - info.srWindow.Top + 1;
    currentScreenInfo.areaForStrlen = (currentScreenInfo.width + 1) * currentScreenInfo.height + 1;

	/* Cursor init */
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 1;
	cursor.bVisible = false;
	SetConsoleCursorInfo(screenBuffer.buffer[0], &cursor);
	SetConsoleCursorInfo(screenBuffer.buffer[1], &cursor);
	
	/* Font init */
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);
	font.nFont = 2;
    font.dwFontSize.X = _FONT_WIDTH_;
    font.dwFontSize.Y = _FONT_HEIGHT_;
    SetCurrentConsoleFontEx(screenBuffer.buffer[0], false, &font);
    SetCurrentConsoleFontEx(screenBuffer.buffer[1], false, &font);
}

void clearScreen()	
{
	fillColorToScreen(_BLACK_, _WHITE_);
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
	SetConsoleTextAttribute(_CURRENT_SCREEN_, tColor | (bColor << 4));
}

void fillColorToScreen(ConsoleColor bColor, ConsoleColor tColor)
{	
	COORD zero = { 0, 0 };
	DWORD dw;
	char* bufferString;
	int i, j, currIdx;
	
	bufferString = (char*)calloc(currentScreenInfo.areaForStrlen, sizeof(char));
	
	SetConsoleCursorPosition(_CURRENT_SCREEN_, zero);
	SetConsoleTextAttribute(_CURRENT_SCREEN_, tColor | (bColor << 4));
	bufferString[0] = '\0';
	for (i = 0; i < currentScreenInfo.height; i++)
	{
		currIdx =  i * (currentScreenInfo.width + 1);
		memset(bufferString+currIdx, ' ', currentScreenInfo.width * sizeof(char));
		bufferString[currIdx+currentScreenInfo.width] = '\n';
	}
	bufferString[currentScreenInfo.areaForStrlen-1] = '\0';
	WriteFile(_CURRENT_SCREEN_, bufferString, strlen(bufferString), &dw, NULL);
	
	free(bufferString);
}



/* Print Func */

void printScreen(void render(void))
{
	clearScreen();
	render();
	SetConsoleActiveScreenBuffer(_CURRENT_SCREEN_);
	swapScreenIndex();
}

void printMainMenuScreen(int selected)
{
	clearScreen();
	renderMainMenuScreen(selected);
	SetConsoleActiveScreenBuffer(_CURRENT_SCREEN_);
	swapScreenIndex();
}

void printBattleScreen(int turn, int selected)
{
	clearScreen();
	
	/*
	1. Select some action in choice boxes.
		1-1. If select, 'selected's index is x+4. (for example, player selected FIGHT, now 'selected' == 4.)
	2. If action selected, render the action's screen.
	*/
	
	switch (selected)
	{
		case _FIGHT_:
			renderBattleExplositionBox(turn);
			break;
			
		case _ACT_:
			renderBattleExplositionBox(turn);
			break;
			
		case _ITEM_:
			renderBattleExplositionBox(turn);
			break;
			
		case _MERCY_:
			renderBattleExplositionBox(turn);
			break;
			
		case _FIGHT_ING_:
			break;
			
		case _ACT_ING_:
			break;
			
		case _ITEM_ING_:
			break;
			
		case _MERCY_ING_:
			break;
	}
	renderBattleChoiceBoxes(selected);
	
	
	SetConsoleActiveScreenBuffer(_CURRENT_SCREEN_);
	swapScreenIndex();
}



/* Render Func */

void renderString(char* str, COORD pos)
{
	DWORD dw;
	char* nextLine;
	
	nextLine = strtok(str, "\n");
	while (nextLine != NULL)
	{
		SetConsoleCursorPosition(_CURRENT_SCREEN_, pos);
		WriteFile(_CURRENT_SCREEN_, nextLine, strlen(nextLine), &dw, NULL);
		nextLine = strtok(NULL, "\n");
		pos.Y++;
	}
}

void renderTestScreen()
{
	COORD zero = { 0, 0 };
	DWORD dw;
	char s[10] = "hi\n";
	
	setColor(_BLACK_, _WHITE_);
	SetConsoleCursorPosition(_CURRENT_SCREEN_, zero);
	WriteFile(_CURRENT_SCREEN_, s, strlen(s), &dw, NULL);
}

/* MainMenu */
void renderMainMenuScreen(int selected)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_, tTitleColor = _BRIGHT_GRAY_, tLogoColor = _WHITE_, tSelColor = _YELLOW_;
	COORD titlePos = { 0, currentScreenInfo.height * 0.36 };
	COORD contentPos[2], selectedPos;
	DWORD dw;
	char content[2][64], selectedChar[5] = "Ⅱ ";
	char* title, nextLine;
	int i, contentPosY = currentScreenInfo.height * 0.76;
	
	title = (char*)calloc(currentScreenInfo.areaForStrlen, sizeof(char));
	fillColorToScreen(bColor, tColor);
	
	
	setColor(bColor, tLogoColor);
	sprintf(title,     "$$\\   $$\\ $$\\   $$\\ $$$$$$$\\  $$$$$$$$\\ $$$$$$$\\ $$$$$$$$\\  $$$$$$\\  $$\\       $$$$$$$$\\  ");
	titlePos.X = (currentScreenInfo.width - strlen(title)) * 0.5;
	sprintf(title, "%s\n$$ |  $$ |$$$\\  $$ |$$  __$$\\ $$  _____|$$  __$$\\\\__$$  __|$$  __$$\\ $$ |      $$  _____| ", title);
	sprintf(title, "%s\n$$ |  $$ |$$$$\\ $$ |$$ |  $$ |$$ |      $$ |  $$ |  $$ |   $$ /  $$ |$$ |      $$ |       ", title);
	sprintf(title, "%s\n$$ |  $$ |$$ $$\\$$ |$$ |  $$ |$$$$$\\    $$$$$$$  |  $$ |   $$$$$$$$ |$$ |      $$$$$\\     ", title);
	sprintf(title, "%s\n$$ |  $$ |$$ \\$$$$ |$$ |  $$ |$$  __|   $$  __$$<   $$ |   $$  __$$ |$$ |      $$  __|    ", title);
	sprintf(title, "%s\n$$ |  $$ |$$ |\\$$$ |$$ |  $$ |$$ |      $$ |  $$ |  $$ |   $$ |  $$ |$$ |      $$ |       ", title);
	sprintf(title, "%s\n\\$$$$$$  |$$ | \\$$ |$$$$$$$  |$$$$$$$$\\ $$ |  $$ |  $$ |   $$ |  $$ |$$$$$$$$\\ $$$$$$$$\\  ", title);
	sprintf(title, "%s\n \\______/ \\__|  \\__|\\_______/ \\________|\\__|  \\__|  \\__|   \\__|  \\__|\\________|\\________| ", title);
	renderString(title, titlePos);
	
	setColor(bColor, tTitleColor);
	sprintf(title, "19 Song JaeUk in Hansung Univ.");
	titlePos.X = (currentScreenInfo.width - strlen(title)) * 0.5;
	titlePos.Y += 10;
	renderString(title, titlePos);
                 
	sprintf(content[0], "Game Start");
	contentPos[0].X = (currentScreenInfo.width - strlen(content[0])) * 0.5;
	contentPos[0].Y = contentPosY;
	sprintf(content[1], "Exit Game");
	contentPos[1].X = (currentScreenInfo.width - strlen(content[1])) * 0.5;
	contentPos[1].Y = contentPosY + 1;
	
	for (i = 0; i < 2; i++)
	{
		if (i == selected)
		{
			selectedPos.X = contentPos[i].X - 3;
			selectedPos.Y = contentPos[i].Y;
	        setColor(bColor, tSelColor);
		}
		else
		{
	        setColor(bColor, tColor);
		}
		SetConsoleCursorPosition(_CURRENT_SCREEN_, contentPos[i]);	
		WriteFile(_CURRENT_SCREEN_, content[i], strlen(content[i]), &dw, NULL);
	}
	
	setColor(bColor, tSelColor);
	SetConsoleCursorPosition(_CURRENT_SCREEN_, selectedPos);
	WriteFile(_CURRENT_SCREEN_, selectedChar, strlen(selectedChar), &dw, NULL);
	
	free(title);
}

void renderBattleExplositionBox(int currTurn)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_;
	COORD centerBoxPos = { currentScreenInfo.width * 0.76, currentScreenInfo.height * 0.4 };
	DWORD dw;
	char* centerBox;
	int i, lastIdx;
	
	centerBox = (char*)calloc((centerBoxPos.X + 1) * centerBoxPos.Y + 1, sizeof(char));
	fillColorToScreen(bColor, tColor);
	
	/* Center Box */
	sprintf(centerBox,     "  旨");
	lastIdx = strlen(centerBox);
	strrptcat(centerBox, "收", centerBoxPos.X - 4);
	strcat(centerBox, "旬 ");
	for (i = 0; i < centerBoxPos.Y - 2; i++)
	{
		strcat(centerBox, "\n早");
		lastIdx = strlen(centerBox);
		strrptcat(centerBox, " ", centerBoxPos.X - 4);
		strcat(centerBox, "早 ");
	}
	strcat(centerBox, "\n曲");
	lastIdx = strlen(centerBox);
	strrptcat(centerBox, "收", centerBoxPos.X - 4);
	strcat(centerBox, "旭 ");
	
	renderString(centerBox, centerBoxPos);
	
	free(centerBox);
}

void renderBattleChoiceBoxes(int selected)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_, tSelColor = _YELLOW_;
	COORD choiceBoxPos = { currentScreenInfo.width * 0.17, currentScreenInfo.height * 0.8 };
	DWORD dw;
	char choiceBoxText[4][16] = { "∵ FIGHT", "Ⅷ ACT", "Ⅹ ITEM", "Ⅵ MERCY" };
	char* choiceBox;	// char choiceBox[4][currentScreenInfo.width * 7]
	int i, offset = 60;
	
	/* choiceBox[4][currentScreenInfo.width * 7] */
	choiceBox = (char*)calloc(currentScreenInfo.width * 7, sizeof(char));
	fillColorToScreen(bColor, tColor);
	
	/* Initialize Chioce Boxes. */
	for (i = 0; i < 4; i++)
	{
		if (i == selected)
			setColor(bColor, tSelColor);
		else
			setColor(bColor, tColor);
			
		sprintf(choiceBox,     "旨收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旬 ");
		sprintf(choiceBox, "%s\n早                                早 ", choiceBox);
		sprintf(choiceBox, "%s\n早            %-10s          早  ", choiceBox, choiceBoxText[i]);
		sprintf(choiceBox, "%s\n早                                早 ", choiceBox);
		sprintf(choiceBox, "%s\n曲收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收收旭 ", choiceBox);
		
		renderString(choiceBox, choiceBoxPos);
		choiceBoxPos.X += offset;
	}
	
	free(choiceBox);
}
