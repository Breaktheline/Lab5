#ifndef H_PARSER
#define H_PARSER

#include "Common.h"
#include "stdio.h"
#include <string.h>
#include "AppException.h"
#include "TList.h"

class Parser
{
public:
	TList<char>* ReadString(FILE* inputFile);

	bool CanRead(int ch, FILE* inputFile);

	void PrintResult(int leafNumber, TList<char>* string, int inputStringLength);
	void PrintError(const char* errorMessage);

private:
	bool IsEndOfFile(FILE* file);
	bool IsSeparator(int character);
	bool IsEndOfLine(int character);
	bool IsEndOfFile(int character);
};

#endif



