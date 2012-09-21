#include "Linearizator.h"

int Linearizator::Linearize(TList<char>* string)
{
	int inputStringLength = string->GetCount();
	//если строка меньше или равна 1 символам.
	if (inputStringLength <= 1)
	{	
		return 0;
		//return GetCloneString(new TList<char>(), string);
	}
	//если в строке всего 2 символа, то просто сравниваем.
	if (inputStringLength == 2)
	{
		return string->GetElement(0) > string->GetElement(1) ? 1 : 0;
		//return FindMinStringFromDouble(string);
	}

	AddCopyToEnd(string);
	StringComparer* stringComparer = new StringComparer(string);
	SuffixTreeBuilder builder(stringComparer);
	SuffixTree* tree = builder.BuildTree();

	return FindMinString(tree, inputStringLength, string);
}

void Linearizator::AddCopyToEnd(TList<char>* inputString)
{
	inputString->AddRange(inputString);
}

int Linearizator::FindMinString(SuffixTree* tree, int inputStringLength, TList<char>* string)
{
	//TList<char>* linearizedString = new TList<char>();
	int checkedLength = 0;
	int lastIndex = string->GetCount() - 1;

	Vertex* root = tree->GetRoot();
	Arch* nextArch = tree->FindMinArch(root);

	while (checkedLength + nextArch->GetLength(lastIndex) < inputStringLength)
	{
		int realLength = nextArch->GetLength(lastIndex);
		//for (int i = 0; i < realLength; i++)
		//{
		//	int index = nextArch->GetStartIndex() + i;
		//	linearizedString->Add(string->GetElement(index));
		//}
		checkedLength += realLength;
		nextArch = tree->FindMinArch(nextArch->_nextVertex);
	}

	int number = tree->FindLeafArchNumber(nextArch);
	//for (int i = 0; i < inputStringLength - checkedLength; i++)
	//{
	//	int index = nextArch->GetStartIndex() + i;
	//	linearizedString->Add(string->GetElement(index));
	//}
	
	//linearizedString->Add('\0');
	//linearizedString->TrimExcess();

	delete tree;
	return number >= inputStringLength ? number - inputStringLength : number;
}

//TList<char>* Linearizator::FindMinStringFromDouble(TList<char>* string)
//{
//	char firstChar = string->GetElement(0);
//	char secondChar = string->GetElement(1);
//	TList<char>* linString =  new TList<char>();
//	if (firstChar < secondChar)
//	{
//		return GetCloneString(linString, string);
//	}
//
//	linString->Add(secondChar);
//	linString->Add(firstChar);
//	linString->Add('\0');
//	return linString;
//}

//TList<char>* Linearizator::GetCloneString(TList<char>* linString, TList<char>* string)
//{
//	linString->AddRange(string);
//	linString->Add('\0');
//	return linString;
//}


