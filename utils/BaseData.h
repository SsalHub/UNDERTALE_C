#ifndef _BASEDATA_
#define _BASEDATA_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <direct.h>
#include <Windows.h>

#include <UtilData.h>
#include <MainMenu.h>
#include <ScreenRender.h>
#include <ExceptionHandler.h>


#define 	_SCREEN_WIDTH_ 			180
#define 	_SCREEN_HEIGHT_ 		50

typedef enum BattleChoiceType
{
	_FIGHT_ = 0,
	_ACT_,
	_ITEM_,
	_MERCY_,
	
	_FIGHT_ING_ = 4,
	_ACT_ING_,
	_ITEM_ING_,
	_MERCY_ING_,
} BattleChoiceType;


void test();

/* Game System Func */
void initGame();
void mainMenu();
void exitGame();

/* Load DataFile Func */
/* Returns Image's Pointer - you MUST free() it */
char* loadImage(char*);

#endif