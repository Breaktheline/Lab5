
#include "gtest/gtest.h"
#include <stdlib.h>
#include <time.h>
#include "Worker.h"

void PrintLongTest()
{
	unsigned long maxLength = 2000000;
	srand ( time(NULL) );
	for(unsigned long i = 0; i < maxLength; i++)
	{
		int code = rand()%26 + 97;
		printf("%c", code);
	}
}

int main(int argc, char** argv)
{
	testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	std::getchar(); // keep console window open until Return keystroke

	//Worker worker;
	//worker.Execute();
	//PrintLongTest();
	//getchar();
	//int digit = 2147483647;
	//printf("%d", digit+5);
}

