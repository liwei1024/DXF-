#include "pch.h"
#include "utils.h"


void utils::myprintf(const char *_Format,WORD Color,...)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),Color);
	va_list argList;
	char buffer[0x1024];
	va_start(argList, Color);
	vsprintf_s(buffer, _Format, argList);
	std::cout << buffer << std::endl;
	va_end(argList);
}
