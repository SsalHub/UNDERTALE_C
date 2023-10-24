#include <MainMenu.h>

void loadMainMenu()
{
	int idx = runMainMenu();
	
	switch(idx)
	{
		case 0:
			playGame();
			return;
		case 1:
			exitGame();
			return;
		default:
			throwFatalException(_UNKNOWN_EXCEPTION_);
			return;
	}
}

int runMainMenu()
{
	const int MAX_SELECT = 2;
	int selected = 0;
	char input;
	
	printMainMenuScreen(selected);
	while (1)
	{
		if (_kbhit())
		{
			input = _getch();
			switch (input)
			{
				case _UP_:
					if (0 <= selected - 1)
						selected -= 1;
					printMainMenuScreen(selected);
					WaitForSeconds(0.1);
					break;
					
				case _DOWN_:
					if (selected + 1 < MAX_SELECT)
						selected += 1;
					printMainMenuScreen(selected);
					WaitForSeconds(0.1);
					break;
					
				case _SPACE_:
				case _CARRIGE_RETURN_:
					/* select ok */
					return selected;
					
				default:
					break;
			}
		}
	}
	
	return -1;
}
