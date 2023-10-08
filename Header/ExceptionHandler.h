#pragma once

typedef enum ExceptionType
{
	_UNKNOWN_EXCEPTION_,
	_FILE_NOT_FOUND_,
	_INVALID_DATA_FORMAT_,
} ExceptionType;

void throwFatalException(ExceptionType);
