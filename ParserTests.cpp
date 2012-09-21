#include "gtest/gtest.h"
#include "Parser.h"
#include "UnitTestsHelper.h"
#include "Worker.h"

void CompareStrings(TList<char>* expectedString, TList<char>* actualString) 
{
	EXPECT_EQ(expectedString->GetCount(), actualString->GetCount());

	for (int i = 0; i < expectedString->GetCount(); i++)
	{
		EXPECT_EQ(expectedString->GetElement(i), actualString->GetElement(i));
	}
}

void WriteDataToFile(char* fileName, TList<char>* string) 
{
	FILE* inputFile = fopen(fileName, "w");
	for(int i = 0; i < string->GetCount(); i++)
	{
		fprintf(inputFile, "%c", string->GetElement(i));
	}
	fprintf(inputFile, "\n");
	fclose(inputFile);
}

TEST(ParserTest, ShouldReadString)
{
	Parser parser;
	TList<char>* inputString = UnitTestsHelper::CreateInputString("defhkljahkgnfasdjnckagruibvdnmcvaslkdkwabgjsavbaslkjfnsadlkfvhadbvhnzbcalskufgjasbdjlncsxn", 90);
	char* fileName = "Tests/in.txt";
	WriteDataToFile(fileName, inputString);

	FILE* inputFile = fopen(fileName, "r");
	TList<char>* readedString = parser.ReadString(inputFile);
	fclose(inputFile);

	CompareStrings(inputString, readedString);
}

TEST(ParserTest, ShouldOutputResult)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("defhkljahkgnfasdjnckagruibvdnmcvaslkdkwabgjsavbaslkjfnsadlkfvhadbvhnzbcalskufgjasbdjlncsxn", 90);
	char* fileName = "Tests/in.txt";
	WriteDataToFile(fileName, inputString);

	FILE* inputFile = fopen(fileName, "r");
	Worker worker;
	worker.Execute(inputFile);

}