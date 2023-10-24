#include <ExceptionHandler.h>

void throwFatalException(ExceptionType e)
{
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