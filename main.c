#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>

#include "./Header/BaseData.h" 
#include "./Header/ScreenRender.h" 

int main(int argc, char *argv[]) 
{
	COORD zero = {0, 0};
	char hello[32] = "hello\n";
	
	initGame();
	initScreen();
	
	printString(hello, zero, true, true);
	
	while (1)
	{
		if (_kbhit())
		{
			break;
		}
	}	
		
	return 0;
}
