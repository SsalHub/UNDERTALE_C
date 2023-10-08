#include "../Header/ScreenRender.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Windows.h>

#include "../Header/UtilData.h"
#include "../Header/BaseData.h"

ScreenBuffer screenBuffer;
ScreenInfo screenInfo;

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
    screenInfo.width = info.srWindow.Right - info.srWindow.Left + 1;
    screenInfo.height = info.srWindow.Bottom - info.srWindow.Top + 1;
    screenInfo.areaForStrlen = (screenInfo.width + 1) * screenInfo.height + 1;

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
	
	bufferString = (char*)calloc(screenInfo.areaForStrlen, sizeof(char));
	
	SetConsoleCursorPosition(_CURRENT_SCREEN_, zero);
	SetConsoleTextAttribute(_CURRENT_SCREEN_, tColor | (bColor << 4));
	bufferString[0] = '\0';
	for (i = 0; i < screenInfo.height; i++)
	{
		currIdx =  i * (screenInfo.width + 1);
		memset(bufferString+currIdx, ' ', screenInfo.width * sizeof(char));
		bufferString[currIdx+screenInfo.width] = '\n';
	}
	bufferString[screenInfo.areaForStrlen-1] = '\0';
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
	
	renderBattleEnemy();
	switch (selected)
	{
		case _FIGHT_:
			renderBattleExplainBox(turn);
			break;
			
		case _ACT_:
			renderBattleExplainBox(turn);
			break;
			
		case _ITEM_:
			renderBattleExplainBox(turn);
			break;
			
		case _MERCY_:
			renderBattleExplainBox(turn);
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
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_, tTitleColor = _GRAY_, tLogoColor = _WHITE_, tSelColor = _YELLOW_;
	COORD titlePos = { 0, screenInfo.height * 0.36 };
	COORD contentPos[2], selectedPos;
	DWORD dw;
	char content[2][64], selectedChar[5] = "Ⅱ ";
	char* title, nextLine;
	int i, contentPosY = screenInfo.height * 0.76;
	
	title = (char*)calloc(screenInfo.areaForStrlen, sizeof(char));
	fillColorToScreen(bColor, tColor);
	
	setColor(bColor, tLogoColor);
	sprintf(title,     "$$\\   $$\\ $$\\   $$\\ $$$$$$$\\  $$$$$$$$\\ $$$$$$$\\ $$$$$$$$\\  $$$$$$\\  $$\\       $$$$$$$$\\  ");
	titlePos.X = (screenInfo.width - strlen(title)) * 0.5;
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
	titlePos.X = (screenInfo.width - strlen(title)) * 0.5;
	titlePos.Y += 10;
	renderString(title, titlePos);
                 
	sprintf(content[0], "Game Start");
	contentPos[0].X = (screenInfo.width - strlen(content[0])) * 0.5;
	contentPos[0].Y = contentPosY;
	sprintf(content[1], "Exit Game");
	contentPos[1].X = (screenInfo.width - strlen(content[1])) * 0.5;
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

void renderBattleEnemy()
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_;
	COORD pos = { 0, screenInfo.height * 0.03 };
	char* buffer;
	char* pStr = buffer;
	int slen, maxW = 0;
	
	buffer = (char*)calloc(screenInfo.areaForStrlen, sizeof(char));
	
	setColor(bColor, tColor);
	buffer[0] = '\0'; 
//	strcat(buffer,   "                               .......... ");
//	strcat(buffer, "\n                           ..!############!.. ");
//	strcat(buffer, "\n                      ..!######################!... ");
//	strcat(buffer, "\n                    ..!##########################!.. ");
//	strcat(buffer, "\n                      ##!\"\"\"\"\"\"\"!####!\"\"\"\"\"\"\"\"\"### ");
//	strcat(buffer, "\n                       ##  ....   ##    ....   \"!# ");
//	strcat(buffer, "\n                     \"!.  \"\"   .#!\"\"!#..  \"\"  ..!\" ");
//	strcat(buffer, "\n                      \"##!--....##!  !##....\"\"#####\" ");
//	strcat(buffer, "\n                     !!\" \"!!###!!. -- .!!##!\"\"\"\"  \"\"\"##\" ");
//	strcat(buffer, "\n                     \"\"###..\"!!-.. .. ...-!!####!\" ...... ");
//	strcat(buffer, "\n                 ..#.  \"!##.!\"\"\"\"\" \"\" \"\"..-!##\"\"    .##!!\"\" ");
//	strcat(buffer, "\n               -\"##-..    \"\"\"\"\"\"\"!!##\"\"\"\"!\"!\"\"\"   ..=##!\"\" \"-.. ");
//	strcat(buffer, "\n             !       \"!#=.      -..##-..       .=#!\"..      \"\"-.. ");
//	strcat(buffer, "\n         .-\"\"      ## #!\"\"\"!-     \"###\"     -.!#\"\"\"   !       \"\"-.. ");
//	strcat(buffer, "\n         .-\"\"       !. ...!...! ..-___-..       ..=!   !       \"\"-.. ");
//	strcat(buffer, "\n        !##        !==\"  \"\" -.- .. !## !!.-   ...=!\"  !!        \"\"-.. ");
//	strcat(buffer, "\n        \"!-         !         !!  !## !..  ..=!\"      !!       \"\"-.. ");
//	strcat(buffer, "\n          \"!-..      !        !!  !## !..\"\"\"          !!      \"\"-.. ");
//	strcat(buffer, "\n             \"!-..!-..!-.     !!  !\"\" !..\"!    ......-!#\"...#-\" ");
//	strcat(buffer, "\n                 \"\"\"\" \"\" \" \"  \"           \"       \"\" \"\"  \"\"\"\"\" ");
//	strcat(buffer, "\n                   .--!  !!                ##!    -!! ");
//	strcat(buffer, "\n                  \" ! ##!!       .\"-.     ##!.      ! ");
//	strcat(buffer, "\n                 .!!\" !##\"       .!  \"!.   ###!      \"! ");
//	strcat(buffer, "\n                 !  !##         !\"\"  \"!.   \"\"##!     !!! ");
//	strcat(buffer, "\n                 !..!!!!\" .... !!   \"!. ......!!!! ...!! ");
//	strcat(buffer, "\n                      \"\"\"\"\"\"\"\"\"\"        \"\"\"\"\"\"\"\"\"\"\"\" ");
//	strcat(buffer, "\n                  ..-!#!-. \"!-.           .--#!\"\" .=!#!-. ");
//	strcat(buffer, "\n                \"\"!!##!-.. \"!...       ...-!!#--.. ..=!##!\" ");

	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 162, 128, 226, 163, 180, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 163, 190, 226, 163, 191, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 163, 191, 226, 160, 139, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 162, 185, 226, 163, 134, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 162, 160, 226, 163, 191, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 184, 226, 163, 167, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 162, 128, 226, 163, 182, 226, 163, 166, 226, 161, 128, 226, 160, 153, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 130, 226, 160, 136, 226, 160, 153, 226, 160, 155, 226, 162, 182, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 161, 160, 226, 160, 138, 226, 160, 128, 226, 160, 128, 226, 163, 180, 226, 160, 128, 226, 160, 128, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 161, 132, 226, 160, 128, 226, 162, 160, 226, 160, 129, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 163, 191, 226, 161, 128, 226, 163, 128, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 161, 135, 226, 160, 176, 226, 162, 191, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 162, 160, 226, 161, 137, 226, 160, 137, 226, 160, 128, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 131, 226, 160, 128, 226, 160, 136, 226, 160, 179, 226, 163, 132, 226, 161, 128, 226, 162, 184, 226, 161, 135, 226, 160, 128, 226, 160, 128, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 136, 226, 160, 131, 226, 160, 152, 226, 163, 183, 226, 163, 164, 226, 160, 128, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 136, 226, 160, 137, 226, 160, 137, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 162, 176, 226, 160, 128, 226, 162, 160, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 161, 152, 226, 162, 128, 226, 163, 156, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 161, 135, 226, 162, 184, 226, 163, 191, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 153, 226, 160, 155, 226, 160, 139, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 162, 128, 226, 163, 180, 226, 163, 182, 226, 163, 166, 226, 163, 132, 226 );
	sprintf("%c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c %c%c%c \n", 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 128, 226, 160, 184, 226, 160, 191, 226, 160, 191, 226, 160, 191, 226, 160, 191, 226, 160 );


	/* Get Enemy String's Max Width, and set position.X */
//	pStr = strtok(buffer, "\n");
//	while (pStr != NULL)
//	{
//		slen = strlen(pStr);
//		maxW = slen > maxW ? slen : maxW;
//		if (pStr != buffer) *(pStr - 1) = '\n';
//		pStr = strtok(NULL, "\n");
//	}
//	pos.X = (screenInfo.width - maxW) * 0.5;
	pos.X = screenInfo.width * 0.3;
	
	renderString(buffer, pos);
	free(buffer);
}

void renderBattleExplainBox(int currTurn)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_;
	const int CENTERBOX_WIDTH = screenInfo.width * 0.78, CENTERBOX_HEIGHT = screenInfo.height * 0.4;
	COORD centerBoxPos = { (screenInfo.width - CENTERBOX_WIDTH) * 0.5, (screenInfo.height - CENTERBOX_HEIGHT) * 0.64 };
	DWORD dw;
	char* centerBox;
	int i, j;
	
	centerBox = (char*)calloc(CENTERBOX_WIDTH * CENTERBOX_HEIGHT * 1.5, sizeof(char));
	
	/* Initialize Center Box */
	setColor(bColor, tColor);
	// Top Line
	sprintf(centerBox, "旨");
	for (i = 0; i < CENTERBOX_WIDTH - 2; i++)
		strcat(centerBox, "收");
	strcat(centerBox, "旬 ");
	// Mid Lines
	for (i = 0; i < CENTERBOX_HEIGHT - 2; i++)
	{
		strcat(centerBox, "\n早");
		for (j = 0; j < CENTERBOX_WIDTH - 2; j++)
			strcat(centerBox, " ");
		strcat(centerBox, "早 ");
	}
	// Bottom Line
	strcat(centerBox, "\n曲");
	for (i = 0; i < CENTERBOX_WIDTH - 2; i++)
		strcat(centerBox, "收");
	strcat(centerBox, "旭 ");
	
	renderString(centerBox, centerBoxPos);
	free(centerBox);
}

void renderBattleChoiceBoxes(int selected)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_, tSelColor = _YELLOW_;
	COORD choiceBoxPos = { screenInfo.width * 0.16, screenInfo.height * 0.88 };
	DWORD dw;
	char choiceBoxText[4][16] = { "∵ FIGHT", "Ⅷ ACT", "Ⅹ ITEM", "Ⅵ MERCY" };
	char* choiceBox;	// char choiceBox[4][currentScreenInfo.width * 7]
	int i, offset = 60;
	
	/* choiceBox[4][currentScreenInfo.width * 7] */
	choiceBox = (char*)calloc(screenInfo.width * 7, sizeof(char));
	
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
