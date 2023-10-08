#pragma once

#include "./UtilData.h"

#define 	_SCREEN_WIDTH_ 			230
#define 	_SCREEN_HEIGHT_ 		80
#define		_FONT_WIDTH_			6
#define		_FONT_HEIGHT_			12

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
void startGame();
void exitGame();

/* Load DataFile Func */
void loadImage(char*, char*);
