#include "Linearizator.h"

int Linearizator::Linearize(TList<char>* string)
{
	int inputStringLength = string->GetCount();
	//если строка меньше или равна 1 символам.
	if (inputStringLength <= 1)
	{	
		return 0;
	}

	//если в строке всего 2 символа, то просто сравниваем.
	if (inputStringLength == 2)
	{
		return string->GetElement(0) > string->GetElement(1) ? 1 : 0;
	}

	AddCopyToEnd(string);
	SuffixTreeBuilder builder(string);
	SuffixTree* tree = builder.BuildTree();

	return FindMinString(tree, inputStringLength, string);
}

void Linearizator::AddCopyToEnd(TList<char>* inputString)
{
	inputString->AddRange(inputString);
}

int Linearizator::FindMinString(SuffixTree* tree, int inputStringLength, TList<char>* string)
{
	int checkedLength = 0;
	int lastIndex = string->GetCount() - 1;

	Vertex* root = tree->GetRoot();
	Arch* nextArch = root->FindMinOutcomeArch();

	while (checkedLength + nextArch->GetLength(lastIndex) < inputStringLength)
	{
		int realLength = nextArch->GetLength(lastIndex);
		checkedLength += realLength;
		nextArch = nextArch->nextVertex->FindMinOutcomeArch();
	}

	int number = tree->FindLeafArchNumber(nextArch);

	delete tree;
	return number >= inputStringLength ? number - inputStringLength : number;
}

