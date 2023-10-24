#include <PlayGame.h>

void playGame()
{
	battle();
}

void battle()
{
	const int REFRESH_DELAY = 3;
	time_t refresh;
	char input;
	int turn, selected;
	
	turn = 0;
	selected = 0;
	
	printBattleScreen(turn, selected);
	refresh = clock();
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
					printBattleScreen(turn, selected);
					refresh = clock();
					break;
					
				case _UPPER_D_:
				case _LOWER_D_:
				case _RIGHT_:
					if (selected + 1 < 4)
						selected++;
					printBattleScreen(turn, selected);
					refresh = clock();
					break;
			}
		}
		
		/* Refresh screen every 3.0sec if there was no changes. */
		if (REFRESH_DELAY < clock() - refresh)
		{
			printBattleScreen(turn, selected);
			refresh = clock();
		}
	}
}
