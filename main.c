#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

#include "./Header/BaseData.h" 
#include "./Header/ScreenRender.h" 
#include "./Header/PlayGame.h" 

int main(int argc, char *argv[]) 
{
	COORD pos = { 5, _SCREEN_HEIGHT_ * 0.5 };
	initGame();
	
	playGame();
		
	return 0;
}
