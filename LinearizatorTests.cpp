#include "gtest/gtest.h"
#include "Common.h"
#include "UnitTestsHelper.h"
#include "Linearizator.h"

void Checkresult(TList<char>* expected, TList<char>* linString)
{
	ASSERT_EQ(expected->GetCount() + 1, linString->GetCount());
	for (int i = 0; i < expected->GetCount(); i++)
	{
		ASSERT_EQ(expected->GetElement(i), linString->GetElement(i));
	}
}

TEST(Linearization, CanLinearizeOneChar)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("a", 1);
	TList<char>* expString = UnitTestsHelper::CreateInputString("a", 1);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(0, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeTwoCharsNotLin)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("fc", 2);
	TList<char>* expString = UnitTestsHelper::CreateInputString("cf", 2);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(1, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeTwoCharsLin)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("cf", 2);
	TList<char>* expString = UnitTestsHelper::CreateInputString("cf", 2);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(0, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}


TEST(Linearization, CanLinearizeSimpleString)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("abc", 3);
	TList<char>* expString = UnitTestsHelper::CreateInputString("abc", 3);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(0, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeLinString)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("abcdef", 6);
	TList<char>* expString = UnitTestsHelper::CreateInputString("abcdef", 6);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(0, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeNotLinString)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("efabcd", 6);
	TList<char>* expString = UnitTestsHelper::CreateInputString("abcdef", 6);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(2, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeStringWithVertexes)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("abcabbcabc", 10);
	TList<char>* expString = UnitTestsHelper::CreateInputString("abbcabcabc", 10);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(3, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeStringBCCAABBCCBBAAB)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("bccaabbccbbaab", 14);
	TList<char>* expString = UnitTestsHelper::CreateInputString("aabbccaabbccbb", 14);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(11, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeStringAAAAAAAA)
{
	TList<char>* inputString = UnitTestsHelper::CreateInputString("aaaaaaaa", 8);
	TList<char>* expString = UnitTestsHelper::CreateInputString("aaaaaaaa", 8);

	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(0, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeStringWithHardLinks)
{
	//fasaslsaassvsfs
	TList<char>* inputString = UnitTestsHelper::CreateInputString("fasassaasvs", 11);
	TList<char>* expString = UnitTestsHelper::CreateInputString("aasvsfasass", 11);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(6, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

TEST(Linearization, CanLinearizeLongString)
{
	//zsdjkuensk
	TList<char>* inputString = UnitTestsHelper::CreateInputString("defhkljahkgnfasdjnckagruibvdnmcvaslkdkwabgjsavbaslkjfnsadlkfvhadbvhnzbcalskufgjasbdjlncsxn", 90);
	TList<char>* expString = UnitTestsHelper::CreateInputString("abgjsavbaslkjfnsadlkfvhadbvhnzbcalskufgjasbdjlncsxndefhkljahkgnfasdjnckagruibvdnmcvaslkdkw", 90);
	Linearizator* linearizator = new Linearizator();
	//TList<char>* linString = linearizator->Linearize(inputString);

	ASSERT_EQ(39, linearizator->Linearize(inputString));
	//Asserts
	//Checkresult(expString, linString);
}

