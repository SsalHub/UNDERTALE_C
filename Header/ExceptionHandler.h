#pragma once

typedef enum ExceptionType
{
	_UNKNOWN_EXCEPTION_,
	_FILE_NOT_FOUND_,
} ExceptionType;

void throwFatalException(ExceptionType);
