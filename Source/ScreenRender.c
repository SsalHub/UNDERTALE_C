#include "../Header/ScreenRender.h"

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 	0x0004
#endif
#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 		0x0008
#endif
#ifndef ENABLE_LVB_GRID_WORLDWIDE
#define ENABLE_LVB_GRID_WORLDWIDE 			0x0010
#endif

ScreenBuffer screenBuffer;
ScreenInfo screenInfo;

void initScreen()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	SMALL_RECT rect = {0, 0, _SCREEN_WIDTH_, _SCREEN_HEIGHT_}; 
	
	/* Create Screen Buffer */
	SetConsoleOutputCP(CP_UTF8);	// Set output encoding UTF-8  
	screenBuffer.currentIndex = 0;
	screenBuffer.buffer[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	screenBuffer.buffer[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

	/* Initialize Screen */
    SetConsoleWindowInfo(screenBuffer.buffer[0], TRUE, &rect);
    SetConsoleWindowInfo(screenBuffer.buffer[1], TRUE, &rect);
	SetConsoleMode(screenBuffer.buffer[0], ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN | ENABLE_LVB_GRID_WORLDWIDE);
	SetConsoleMode(screenBuffer.buffer[1], ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN | ENABLE_LVB_GRID_WORLDWIDE);

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
}

void vibrateScreen()
{
	RECT currWindowRectPos;
	COORD currWindowPos;
	GetWindowRect(_CURRENT_SCREEN_, &currWindowRectPos);
	currWindowPos.X = currWindowRectPos.left;	// top left's X
	currWindowPos.Y = currWindowRectPos.top;	// top left's Y

	MoveWindow(screenBuffer.buffer[0], currWindowPos.X+1, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	MoveWindow(screenBuffer.buffer[1], currWindowPos.X+1, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	WaitForSeconds(0.02);
	MoveWindow(screenBuffer.buffer[0], currWindowPos.X-2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	MoveWindow(screenBuffer.buffer[1], currWindowPos.X-2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	WaitForSeconds(0.02);
	MoveWindow(screenBuffer.buffer[0], currWindowPos.X+2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	MoveWindow(screenBuffer.buffer[1], currWindowPos.X+2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	WaitForSeconds(0.02);
	MoveWindow(screenBuffer.buffer[0], currWindowPos.X-2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	MoveWindow(screenBuffer.buffer[1], currWindowPos.X-2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	WaitForSeconds(0.02);
	MoveWindow(screenBuffer.buffer[0], currWindowPos.X+2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	MoveWindow(screenBuffer.buffer[1], currWindowPos.X+2, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	WaitForSeconds(0.02);
	MoveWindow(screenBuffer.buffer[0], currWindowPos.X-1, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
	MoveWindow(screenBuffer.buffer[1], currWindowPos.X-1, currWindowPos.Y, screenInfo.width, screenInfo.height, true);
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
	COORD logoPos = { 0, screenInfo.height * 0.36 }, titlePos = { 0, screenInfo.height * 0.36 + 10 };
	COORD contentPos[2], selectedPos;
	DWORD dw;
	char logoFName[32] = "UNDERTALE.logo", title[128];
	char content[2][32];
	char* logo, *nextLine, *selectedChar;
	int i, contentPosY = screenInfo.height * 0.76;
	
	fillColorToScreen(bColor, tColor);
	
	/* Render Logo */
	setColor(bColor, tLogoColor);
	logo = loadImage(logoFName);
	nextLine = logo;
	while (nextLine != NULL && *nextLine != '\n') { nextLine++; }
	/* If error */ if (nextLine == NULL) throwFatalException(_INVALID_DATA_FORMAT_);
	logoPos.X = (screenInfo.width - (int)(nextLine - logo)) * 0.5;
	renderString(logo, logoPos);
	
	/* Render Title */
	setColor(bColor, tTitleColor);
	sprintf(title, "19 Song JaeUk in Hansung Univ.");
	titlePos.X = (screenInfo.width - strlen(title)) * 0.5;
	renderString(title, titlePos);
    
    /* Render MainMeny Choices */
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
	
	selectedChar = title;
	selectedChar[0] = '\0';
	setColor(bColor, tSelColor);
	SetConsoleCursorPosition(_CURRENT_SCREEN_, selectedPos);
	WriteFile(_CURRENT_SCREEN_, selectedChar, strlen(selectedChar), &dw, NULL);
	
	free(logo);
}

void renderBattleEnemy()
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_;
	COORD pos = { 0, screenInfo.height * 0.04 };
	char enemyFName[32] = "SANS.block";
	char* buffer, *prevLine, *nextLine;
	int slen, maxW = 0;
	
	/* Render Enemy */
	setColor(bColor, tColor);
	buffer = loadImage(enemyFName);
	pos.X = screenInfo.width * 0.3;
	nextLine = buffer;
	while (nextLine != NULL && *nextLine != '\0')
	{
		prevLine = nextLine;
		while (nextLine != NULL && *nextLine != '\n') 
		{
			nextLine++;
		}
		maxW = (int)(nextLine - prevLine) < maxW ? maxW : (int)(nextLine - prevLine);
		nextLine++;
	}
	pos.X = (screenInfo.width - maxW) * 0.5;
	renderString(buffer, pos);
	free(buffer);
}

void renderBattleExplainBox(int currTurn)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_;
	const int CENTERBOX_WIDTH = screenInfo.width * 0.58, CENTERBOX_HEIGHT = screenInfo.height * 0.34;
	COORD centerBoxPos = { (screenInfo.width - CENTERBOX_WIDTH) * 0.5, (screenInfo.height - CENTERBOX_HEIGHT) * 0.24 };
	DWORD dw;
	char centerBox[128];
	int i, j;
	
	/* Initialize Center Box */
	setColor(bColor, tColor);
	centerBox[0] = '\0';
	strcat(centerBox, "\n┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓ ");
	strcat(centerBox, "\n┃                                                    ┃ ");
	strcat(centerBox, "\n┃                                                    ┃ ");
	strcat(centerBox, "\n┃                                                    ┃ ");
	strcat(centerBox, "\n┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛ ");
	// Top Line
	// sprintf(centerBox, "┏");
	// for (i = 0; i < CENTERBOX_WIDTH - 2; i++)
	// 	strcat(centerBox, "━");
	// strcat(centerBox, "┓ ");
	// Mid Lines
	// for (i = 0; i < CENTERBOX_HEIGHT - 2; i++)
	// {
	// 	strcat(centerBox, "\n┃");
	// 	for (j = 0; j < CENTERBOX_WIDTH - 2; j++)
	// 		strcat(centerBox, " ");
	// 	strcat(centerBox, "┃ ");
	// }
	// Bottom Line
	// strcat(centerBox, "\n┗");
	// for (i = 0; i < CENTERBOX_WIDTH - 2; i++)
	// 	strcat(centerBox, "━");
	// strcat(centerBox, "┛ ");
	
	renderString(centerBox, centerBoxPos);
}

void renderBattleChoiceBoxes(int selected)
{
	ConsoleColor bColor = _BLACK_, tColor = _WHITE_, tSelColor = _YELLOW_;
	COORD choiceBoxPos = { screenInfo.width * 0.16, 0 };
	DWORD dw;
	char choiceBoxText[4][16] = { "√ FIGHT", "♣ ACT", "◈ ITEM", "♥ MERCY" };
	char choiceBox[128];
	int i, choiceBoxPosY = screenInfo.height * 0.6, offset = 60;
	
	/* choiceBox[4][currentScreenInfo.width * 7] */
	
	/* Initialize Chioce Boxes. */
	for (i = 0; i < 4; i++)
	{
		if (i == selected)
			setColor(bColor, tSelColor);
		else
			setColor(bColor, tColor);
		choiceBox[0] = '\0';
		sprintf(choiceBox, "%s\n┏━━━━━━━━━━━━━━━━━━━━━━┓ ");
		sprintf(choiceBox, "%s\n┃        %-10s      ┃ ", choiceBox, choiceBoxText[i]);
		sprintf(choiceBox, "%s\n┗━━━━━━━━━━━━━━━━━━━━━━┛ ", choiceBox);
		
		choiceBoxPos.Y = choiceBoxPosY;
		renderString(choiceBox, choiceBoxPos);
		choiceBoxPos.X += offset;
	}
}
