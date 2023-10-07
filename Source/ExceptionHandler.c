#include "../Header/ExceptionHandler.h"

#include <stdio.h>
#include <Windows.h>

#include "../Header/BaseData.h"

void throwFatalException(ExceptionType e)
{
	switch (e)
	{
		case _UNKNOWN_EXCEPTION_:
			printf("Fatal Exception : Unknown exception occurred while running this game.\nEnd console game progresses.\n");
			break;
		case _FILE_NOT_FOUND_:
			printf("Fatal Exception : Failed to load data file!\nEnd console game progresses.\n");
			break;
	}
	
	exit(1);
}
