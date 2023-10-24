#ifndef _EXCEPTIONHANDLER_
#define _EXCEPTIONHANDLER_

#include <stdio.h>
#include <windows.h>

#include <BaseData.h>
#include <ScreenRender.h>

typedef enum ExceptionType
{
	_UNKNOWN_EXCEPTION_,
	_FILE_NOT_FOUND_,
	_INVALID_DATA_FORMAT_,
} ExceptionType;

void throwFatalException(ExceptionType);
void renderErrorScreen();

#endif