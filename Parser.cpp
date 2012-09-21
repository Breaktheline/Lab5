#include "Parser.h"

TList<char>* Parser::ReadString(FILE* inputFile)
{
	if (inputFile == NULL)
	{
		throw AppException(ErrorMessages::FILE_OPEN_ERROR);
	}

	TList<char>* string = new TList<char>();
	int ch = '$';
	while (CanRead(ch = fgetc(inputFile), inputFile))
	{
		string->Add(ch);
	}

	if (string->GetCount() == 0)
	{
		delete string;
		return NULL;
	}

	//string->TrimExcess();
	fclose(inputFile);
	printf("%d\n", string->GetCount());
	return string;
}

void Parser::PrintResult(int leafNumber, TList<char>* string, int inputStringLength)
{
	for (int i = leafNumber; i < inputStringLength; i++)
	{
		printf("%c", string->GetElement(i));
	}

	for (int i = 0; i < leafNumber; i++)
	{
		printf("%c", string->GetElement(i));
	}
	printf("\n");
}

void Parser::PrintError(const char* errorMessage)
{
	printf("ERROR: %s", errorMessage);
}

bool Parser::IsEndOfFile(FILE* file)
{
	return feof(file);
}

bool Parser::IsEndOfFile(int character)
{
	return character == EOF;
}

bool Parser::IsSeparator(int character)
{
	return character == ' ' || character == '\t' || character == '\n' || character == '\r' || character == EOF;
}

bool Parser::IsEndOfLine(int character)
{
	return character == '\n' || character == '\r';
}

bool Parser::CanRead( int ch, FILE* inputFile )
{
	return !IsEndOfLine(ch) && !IsEndOfFile(ch) && !IsEndOfFile(inputFile) && !IsSeparator(ch);
}


