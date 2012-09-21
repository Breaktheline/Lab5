#ifndef H_UNIT_TESTS_HELPER
#define H_UNIT_TESTS_HELPER

#include "Common.h"
#include "TList.h"

class UnitTestsHelper
{
public:
	static TList<char>* CreateInputString(char* string, int length)
	{
		TList<char>* inputString = new TList<char>();
		for(int i = 0; i < length; i++)
		{
			inputString->Add(string[i]);
		}
		return inputString;
	}
};

#endif

