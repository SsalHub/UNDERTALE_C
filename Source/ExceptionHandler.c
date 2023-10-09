#include "../Header/ExceptionHandler.h"

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
			
		case _INVALID_DATA_FORMAT_:
			printf("Fatal Exception : Loaded file data is invalid!\nEnd console game progresses.\n");
			break;
	}
	
	exit(1);
}
