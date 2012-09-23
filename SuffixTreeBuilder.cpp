#include "SuffixTreeBuilder.h"

SuffixTreeBuilder::SuffixTreeBuilder(TList<char>* string)
{
	_string = string;
	_stringCount = string->GetCount();
}

SuffixTree* SuffixTreeBuilder::BuildTree()
{
	if (_stringCount < 1)
	{
		return NULL;
	}

	//_string->Add(SuffixTree::LAST_CHARACTER);

	Vertex* root = new Vertex(NULL);
	//��������� ��������� �����������
	ExtensionResult extensionResult;
	//�������, � �������� ����������� �����������
	PositionInTree startPosition;;
	AddFirstElement(&startPosition, root);
	extensionResult.SetExtensionResult(startPosition.arch, startPosition.indexInArch, NULL, true);

	// ����� ����������� j, � �������� ���������� ����.
	int startExtension = 1;
	int stringCount = _stringCount;
	bool isFirstExtensionOfPhase = true;
	int startIndex = 1;
	for(int j = 0; j < 2; j++)
	{
		if (j == 1)
		{
			_string->Add(SuffixTree::LAST_CHARACTER);
			startIndex = stringCount;
			stringCount += stringCount + 1;
		}

		for(int i = startIndex; i < stringCount; i++)
		{
			startExtension--;
			// ��������� ����������� �� ����� ����
			while(startExtension <= i)
			{
				Vertex* lastNewInnerVertex = isFirstExtensionOfPhase ? NULL : extensionResult.newInnerVertex;
				// ���� �� ���������� ���� ���� �������� �������, �� � ��� �������� ����� ����� �����������.
				if(lastNewInnerVertex != NULL)
				{
					startPosition.SetPositionToVertex(lastNewInnerVertex);
				}

				if (isFirstExtensionOfPhase)
				{
					startPosition.SetPosition(&extensionResult.positionInTree);
				}
				else
				{
					UpdatePlaceToExtend(&startPosition);
				}

				ExtendFromPosition(&extensionResult, &startPosition, i, startExtension);
				TryToAddLinkedVertex(lastNewInnerVertex, &extensionResult, root);

				isFirstExtensionOfPhase = false;
				startExtension++;
				// ���� ����������� ������ �������.
				if (extensionResult.isLastExtension)
				{
					break;
				}
			}

		isFirstExtensionOfPhase = true;
		_lastAddedIndex = i;
		}
	}

	return new SuffixTree(_string, root);
}

void SuffixTreeBuilder::AddFirstElement(PositionInTree* startPosition, Vertex* root)
{
	// �������� 0 ����: ��������� ������ � ���� �������� ����.
	Arch* firstArch = new Arch(0, 0, -1);
	root->AddOutcomeArch(GetStringElement(firstArch->startIndex), firstArch);
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
	return vertex->FindOutcomeArch(GetStringElement(index));
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
		vertex->AddOutcomeArch(GetStringElement(newArch->startIndex), newArch);
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
	prevVertex->RemoveOutcomeArch(GetStringElement(currentArch->startIndex), currentArch);
	// ������� ����� ����, ������� �������� �����.
	Arch* commonArch = new Arch(-1, currentArch->startIndex, indexInArch);
	prevVertex->AddOutcomeArch(GetStringElement(commonArch->startIndex), commonArch);
	// �������� ������ ������� ����.
	currentArch->startIndex = indexInArch + 1;
	// ������� ����� ����, ������� �������� ������������.
	Arch* newArch = new Arch(extensionNumber, indexInString, -1);
	// ������� ����� �������.
	Vertex* newVertex = new Vertex(commonArch);
	newVertex->AddOutcomeArch(GetStringElement(currentArch->startIndex), currentArch);
	newVertex->AddOutcomeArch(GetStringElement(newArch->startIndex), newArch);

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
	return GetStringElement(firstIndex) == GetStringElement(secondIndex);
}

char SuffixTreeBuilder::GetStringElement(int index)
{
	if (index >= _stringCount)
	{
		index -= _stringCount;
	}

	return _string->GetElement(index);
}
