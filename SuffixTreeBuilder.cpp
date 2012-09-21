#include "SuffixTreeBuilder.h"

SuffixTreeBuilder::SuffixTreeBuilder(TList<char>* string)
{
	_string = string;
}

SuffixTree* SuffixTreeBuilder::BuildTree()
{
	if (_string->GetCount() < 1)
	{
		return NULL;
	}

	_string->Add(SuffixTree::LAST_CHARACTER);

	Vertex* root = new Vertex(NULL);
	//��������� ��������� �����������
	ExtensionResult extensionResult;
	//�������, � �������� ����������� �����������
	PositionInTree startPosition;;
	AddFirstElement(&startPosition, root);
	extensionResult.SetExtensionResult(startPosition.arch, startPosition.indexInArch, NULL, true);

	// ����� ����������� j, � �������� ���������� ����.
	int startExtension = 1;
	int stringCount = _string->GetCount();
	bool IsFirstExtensionOfPhase = true;
	for(int i = 1; i < stringCount; i++)
	{
		startExtension--;
		// ��������� ����������� �� ����� ����
		while(startExtension <= i)
		{
			Vertex* lastNewInnerVertex = IsFirstExtensionOfPhase ? NULL : extensionResult.newInnerVertex;
			// ���� �� ���������� ���� ���� �������� �������, �� � ��� �������� ����� ����� �����������.
			if(lastNewInnerVertex != NULL)
			{
				startPosition.SetPositionToVertex(lastNewInnerVertex);
			}

			if (IsFirstExtensionOfPhase)
			{
				startPosition.SetPosition(&extensionResult.positionInTree);
			}
			else
			{
				UpdatePlaceToExtend(&startPosition);
			}

			ExtendFromPosition(&extensionResult, &startPosition, i, startExtension);
			TryToAddLinkedVertex(lastNewInnerVertex, &extensionResult, root);

			IsFirstExtensionOfPhase = false;
			startExtension++;
			// ���� ����������� ������ �������.
			if (extensionResult.isLastExtension)
			{
				break;
			}
		}

		IsFirstExtensionOfPhase = true;
		_lastAddedIndex = i;
	}

	return new SuffixTree(_string, root);
}

void SuffixTreeBuilder::AddFirstElement(PositionInTree* startPosition, Vertex* root)
{
	// �������� 0 ����: ��������� ������ � ���� �������� ����.
	Arch* firstArch = new Arch(0, 0, -1);
	root->AddOutcomeArch(_string->GetElement(firstArch->startIndex), firstArch);
	_lastAddedIndex = 0;
	startPosition->SetPositionToArch(firstArch, 0);
}

void SuffixTreeBuilder::UpdatePlaceToExtend(PositionInTree* startPosition)
{
	// ���� ���������� ����� ����������� � �������, �� 
	// ���� �������� ���������� ������� (���� ������),
	// ���� �������� �� ����� � ���������� ��������� ������� (���� � ������� ������� ��� ���� �����),
	// ���� ����������� �� ��������� �������, �������� �� ����� � ���� ����� ��������.
	if (startPosition->IsInVertex())
	{
		Vertex* vertex = startPosition->vertex;

		if (vertex->IsRoot())
		{
			startPosition->SetPositionToVertex(vertex);
		}
		else if (vertex->HasLinkedVertex())
		{
			startPosition->SetPositionToVertex(vertex->GetLinkedVertex());
		}

		Arch* incomeArch = vertex->GetIncomeArch();
		Vertex* prevVertex = incomeArch->prevVertex;
		// ���� ��� ������, �� �������� � ����, ����� �������� �� �����.
		if (prevVertex->IsRoot())
		{
			FindEndOfSuffix(startPosition, prevVertex, incomeArch->startIndex + 1, incomeArch->GetFinishIndex(_lastAddedIndex));
		}
		else
		{
			FindEndOfSuffix(startPosition, prevVertex->GetLinkedVertex(), incomeArch->startIndex, incomeArch->GetFinishIndex(_lastAddedIndex));
		}
	}
	// ���� ���������� ����� ����������� �� ����, �� ����������� � �������, �������� �� ����� � ���� ����� ��������.
	else if (startPosition->IsInArch())
	{
		Vertex* prevVertex = startPosition->arch->prevVertex;
		Vertex* startVertex = prevVertex->IsRoot() ? prevVertex : prevVertex->GetLinkedVertex();
		int startIndex = startPosition->arch->startIndex + 1;
		int finishIndex = startPosition->indexInArch;
		FindEndOfSuffix(startPosition, startVertex, startIndex, finishIndex);
	}
	else
	{
		throw AppException("Wrong tree");
	}
}

// ���������� ������ �� �������� �� �������� �������.
void SuffixTreeBuilder::FindEndOfSuffix(PositionInTree* startPosition, Vertex* startVertex, int startIndex, int finishIndex)
{
	// ���� �� ���� ��������� �� ����.
	if (startIndex > finishIndex)
	{
		startPosition->SetPositionToVertex(startVertex);
	}
	else
	{
		Arch* currentArch = FindNextArch(startVertex, startIndex);
		int substringLength = finishIndex - startIndex + 1;
		int checkedSubstringLength = 0;
		int currentArchLength = currentArch->GetLength(_lastAddedIndex);
		while (substringLength - checkedSubstringLength > currentArchLength)
		{
			checkedSubstringLength += currentArchLength;
			currentArch = FindNextArch(currentArch->nextVertex, startIndex + checkedSubstringLength);
			currentArchLength = currentArch->GetLength(_lastAddedIndex);
		}

		startPosition->SetPositionToArch(currentArch, currentArch->startIndex + substringLength - checkedSubstringLength - 1);
	}
}

Arch* SuffixTreeBuilder::FindNextArch(Vertex* vertex, int index)
{
	//Arch** arches = vertex->GetOutcomeArches();
	//int archesCount = vertex->GetOutcomeArchesCount();
	//for (int i = 0; i < archesCount; i++)
	//{
	//	Arch* arch = arches[i];
	//	
	//	if (AreEqualsChars(index, arch->startIndex))
	//	{
	//		return arch;
	//	}
	//}

	//return NULL;

	return vertex->FindOutcomeArch(_string->GetElement(index));
}

// ��������� ���� �� ���� ������ �����������.
void SuffixTreeBuilder::ExtendFromPosition(ExtensionResult* extensionResult, PositionInTree* startPosition, int indexInString, int extensionNumber)
{
	if (startPosition->IsInVertex())
	{
		ExtendFromVertex(extensionResult, startPosition->vertex, indexInString, extensionNumber);
	}
	else if (startPosition->IsInArch())
	{
		ExtendFromArch(extensionResult, startPosition, indexInString, extensionNumber);
	}
	else
	{
		throw AppException("Wrong tree");
	}
}

void SuffixTreeBuilder::ExtendFromVertex(ExtensionResult* extensionResult, Vertex* vertex, int indexInString, int extensionNumber )
{
	Arch* nextArch = FindNextArch(vertex, indexInString);
	// ���� �� ������� ��� ���� � ������� ������, �� ��������� ����.
	if (nextArch == NULL)
	{
		Arch* newArch = new Arch(extensionNumber, indexInString, -1);
		vertex->AddOutcomeArch(_string->GetElement(newArch->startIndex), newArch);
		extensionResult->SetExtensionResult(newArch, indexInString, vertex, false);
	}
	else
	{
		// ���� ���� ����� ���� �� ������� => 3 ������� - �������� ����� �� ��� ����.
		extensionResult->SetExtensionResult(nextArch, nextArch->startIndex,  vertex, true);
	}
}

void SuffixTreeBuilder::ExtendByAddingNewVertex(ExtensionResult* extensionResult, PositionInTree* startPosition, int extensionNumber, int indexInString)
{
	Arch* currentArch = startPosition->arch;
	int indexInArch = startPosition->indexInArch;

	// ������� ������� ���� �� �������.
	Vertex* prevVertex = currentArch->prevVertex;
	prevVertex->RemoveOutcomeArch(_string->GetElement(currentArch->startIndex), currentArch);
	// ������� ����� ����, ������� �������� �����.
	Arch* commonArch = new Arch(-1, currentArch->startIndex, indexInArch);
	prevVertex->AddOutcomeArch(_string->GetElement(commonArch->startIndex), commonArch);
	// �������� ������ ������� ����.
	currentArch->startIndex = indexInArch + 1;
	// ������� ����� ����, ������� �������� ������������.
	Arch* newArch = new Arch(extensionNumber, indexInString, -1);
	// ������� ����� �������.
	Vertex* newVertex = new Vertex(commonArch);
	newVertex->AddOutcomeArch(_string->GetElement(currentArch->startIndex), currentArch);
	newVertex->AddOutcomeArch(_string->GetElement(newArch->startIndex), newArch);

	extensionResult->SetExtensionResult(newArch, newArch->startIndex, newVertex, false);
}

void SuffixTreeBuilder::ExtendFromArch(ExtensionResult* extensionResult, PositionInTree* startPosition, int indexInString, int extensionNumber)
{
	Arch* currentArch = startPosition->arch;
	int indexInArch = startPosition->indexInArch;

	// ���� ��������� ������ �������� ��������� � ����.
	if (startPosition->IsLastArchIndex(_lastAddedIndex))
	{
		// ���� �� ����� �� ������� ������� - ��� ��������, �� ������ �� ������.
		if (currentArch->IsLeaf())
		{
			extensionResult->SetExtensionResult(currentArch, indexInArch + 1, NULL, false);
		}
		else
		{
			// ����� ���������� �� �������.
			ExtendFromVertex(extensionResult, currentArch->nextVertex, indexInString, extensionNumber);
		}
	}
	else
	{
		// ���� ��������� ������ ����� ��, �� ������ �������
		if (AreEqualsChars(indexInArch + 1, indexInString))
		{
			extensionResult->SetExtensionResult(currentArch, indexInArch + 1, NULL, true);
		}
		else
		{
			// ���� ������ �� ����� ��, �� ��������� ������� ���� � ��������� ����� �������.
			ExtendByAddingNewVertex(extensionResult, startPosition, extensionNumber, indexInString);
		}
	}
}

void SuffixTreeBuilder::TryToAddLinkedVertex(Vertex* lastNewInnerVertex, ExtensionResult* extensionResult, Vertex* root)
{
	// ��������� ���������� ������ ����������� �������.
	if (lastNewInnerVertex != NULL && !lastNewInnerVertex->HasLinkedVertex())
	{
		//������� ����� ����� ����������� �� ������� ���� �������� � �������
		if (extensionResult->newInnerVertex != NULL)
		{
			lastNewInnerVertex->AddLinkedVertex(extensionResult->newInnerVertex);
		}
		//� �������������, ��� ���� �� ������� ���� ������� ���� ���������, � �� ������� ���, �� ��� ������� - ������
		else
		{
			lastNewInnerVertex->AddLinkedVertex(root);
		}
	}
}

bool SuffixTreeBuilder::AreEqualsChars(int firstIndex, int secondIndex)
{
	return _string->GetElement(firstIndex) == _string->GetElement(secondIndex);
}
