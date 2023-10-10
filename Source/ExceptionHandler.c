#include "../Header/ExceptionHandler.h"

void throwFatalException(ExceptionType e)
{
	// switch (e)
	// {
	// 	case _UNKNOWN_EXCEPTION_:
	// 		printf("Fatal Exception : Unknown exception occurred while running this game.\nEnd console game progresses.\n");
	// 		break;
			
	// 	case _FILE_NOT_FOUND_:
	// 		printf("Fatal Exception : Failed to load data file!\nEnd console game progresses.\n");
	// 		break;
			
	// 	case _INVALID_DATA_FORMAT_:
	// 		printf("Fatal Exception : Loaded file data is invalid!\nEnd console game progresses.\n");
	// 		break;
	// }
	printScreen(renderErrorScreen);
	exit(1);
}

void renderErrorScreen()
{
	COORD zero = { 0, 0 };
	DWORD dw;
	char message[128] = "Fatal Exception Occurred!\nEnd console game progresses.\n";

	SetConsoleCursorPosition(_CURRENT_SCREEN_, zero);
	WriteFile(_CURRENT_SCREEN_, message, strlen(message), &dw, NULL);

}