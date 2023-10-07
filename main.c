#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

#include "Header/BaseData.h" 
#include "Header/ScreenRender.h" 
#include "Header/MainMenu.h" 

int main(int argc, char *argv[]) 
{
	initGame();
	loadMainMenu();
		
	return 0;
}
