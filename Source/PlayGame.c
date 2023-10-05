#include "../Header/PlayGame.h"

#include <stdio.h>
#include <conio.h>

#include "../Header/BaseData.h"
#include "../Header/ScreenRender.h"

void playGame()
{
	char input;
	int selected;
	
	selected = 0;
	
	printBattleScreen(selected);
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch(input)
			{
				case _UPPER_A_:
				case _LOWER_A_:
				case _LEFT_:
					if (0 < selected)
						selected--;
					printBattleScreen(selected);
					break;
					
				case _UPPER_D_:
				case _LOWER_D_:
				case _RIGHT_:
					if (selected + 1 < 4)
						selected++;
					printBattleScreen(selected);
					break;
			}
		}
		
	}
}
