#include "SuffixTreeBuilder.h"

SuffixTreeBuilder::SuffixTreeBuilder(StringComparer* stringComparer)
{
	_stringComparer = stringComparer;
}

SuffixTree* SuffixTreeBuilder::BuildTree()
{
	if (_stringComparer->GetStringCount() < 1)
	{
		return NULL;
	}

	_stringComparer->AddLastChar();

	Vertex* root = new Vertex(NULL);
	ExtensionResult* endOfPhase = new ExtensionResult(AddFirstElement(root), NULL, true);

	// ����� ����������� j, � �������� ���������� ����.
	int startExtension = 1;
	int stringCount = _stringComparer->GetStringCount();
	for(int i = 1; i < stringCount; i++)
	{
		PositionInTree* startPosition = NULL;
		startExtension--;
		// ��������� ����������� �� ����� ����
		while(startExtension <= i)
		{
			Vertex* lastNewInnerVertex = startPosition == NULL ? NULL : endOfPhase->newInnerVertex;
			// ���� �� ���������� ���� ���� �������� �������, �� � ��� �������� ����� ����� �����������.
			if(lastNewInnerVertex != NULL)
			{
				delete startPosition;
				startPosition = new PositionInTree(lastNewInnerVertex);
			}

			if (startPosition == NULL)
			{
				startPosition =	endOfPhase->positionInTree;
			}
			else
			{
				startPosition = FindPlaceToExtend(startPosition);
				delete endOfPhase->positionInTree;
			}

			delete endOfPhase;
			endOfPhase = ExtendFromPosition(startPosition, i, startExtension);
			TryToAddLinkedVertex(lastNewInnerVertex, endOfPhase, root);

			startExtension++;
			// ���� ����������� ������ �������.
			if (endOfPhase->isLastExtension)
			{
				break;
			}
		}

		delete startPosition;
		_lastAddedIndex = i;
	}

	delete endOfPhase->positionInTree;
	delete endOfPhase;
	//ChangeLastIndex(root);

	return new SuffixTree(_stringComparer->GetString(), root);
}

PositionInTree* SuffixTreeBuilder::AddFirstElement(Vertex* root)
{
	// �������� 0 ����: ��������� ������ � ���� �������� ����.
	Arch* firstArch = new Arch(0, 0, -1);
	root->AddOutcomeArch(firstArch);
	_lastAddedIndex = 0;
	return new PositionInTree(firstArch, 0);
}

PositionInTree* SuffixTreeBuilder::FindPlaceToExtend(PositionInTree* startPosition)
{
	// ���� ���������� ����� ����������� � �������, �� 
	// ���� �������� ���������� ������� (���� ������),
	// ���� �������� �� ����� � ���������� ��������� ������� (���� � ������� ������� ��� ���� �����),
	// ���� ����������� �� ��������� �������, �������� �� ����� � ���� ����� ��������.
	if (startPosition->IsInVertex())
	{
		Vertex* vertex = startPosition->GetVertex();
		delete startPosition;
		if (vertex->IsRoot())
		{
			return new PositionInTree(vertex);
		}
		if (vertex->HasLinkedVertex())
		{
			return new PositionInTree(vertex->GetLinkedVertex());
		}

		Arch* incomeArch = vertex->GetIncomeArch();
		Vertex* prevVertex = incomeArch->_prevVertex;
		// ���� ��� ������, �� �������� � ����, ����� �������� �� �����.
		if (prevVertex->IsRoot())
		{
			return FindEndOfSuffix(prevVertex, incomeArch->_startIndex + 1, incomeArch->GetFinishIndex(_lastAddedIndex));
		}

		return FindEndOfSuffix(prevVertex->GetLinkedVertex(), incomeArch->_startIndex, incomeArch->GetFinishIndex(_lastAddedIndex));
	}

	// ���� ���������� ����� ����������� �� ����, �� ����������� � �������, �������� �� ����� � ���� ����� ��������.
	if (startPosition->IsInArch())
	{
		Vertex* prevVertex = startPosition->GetArchPrevVertex();
		Vertex* startVertex = prevVertex->IsRoot() ? prevVertex : prevVertex->GetLinkedVertex();
		int startIndex = startPosition->GetArchStartIndex() + 1;
		int finishIndex = startPosition->GetIndexInArch();
		delete startPosition;
		return FindEndOfSuffix(startVertex, startIndex, finishIndex);
	}

	// ���� �� ������ ������.
	throw AppException("Wrong tree");
}

// ���������� ������ �� �������� �� �������� �������.
PositionInTree* SuffixTreeBuilder::FindEndOfSuffix(Vertex* startVertex, int startIndex, int finishIndex)
{
	// ���� �� ���� ��������� �� ����.
	if (startIndex > finishIndex)
	{
		return new PositionInTree(startVertex);
	}
	Arch* currentArch = FindNextArch(startVertex, startIndex);
	int substringLength = finishIndex - startIndex + 1;
	int checkedSubstringLength = 0;
	int currentArchLength = currentArch->GetLength(_lastAddedIndex);
	while (substringLength - checkedSubstringLength > currentArchLength)
	{
		checkedSubstringLength += currentArchLength;
		currentArch = FindNextArch(currentArch->_nextVertex, startIndex + checkedSubstringLength);
		currentArchLength = currentArch->GetLength(_lastAddedIndex);
	}

	return new PositionInTree(currentArch, currentArch->_startIndex + substringLength - checkedSubstringLength - 1);
}

Arch* SuffixTreeBuilder::FindNextArch(Vertex* vertex, int index)
{
	TList<Arch*>* arches = vertex->GetOutcomeArches();
	int archesCount = arches->GetCount();
	for (int i = 0; i < archesCount; i++)
	{
		Arch* arch = arches->GetElement(i);
		int archFirstIndex = arch->_startIndex;
		
		if (_stringComparer->AreEqual(index, archFirstIndex))
		{
			return arch;
		}
	}

	return NULL;
}

// ��������� ���� �� ���� ������ �����������.
SuffixTreeBuilder::ExtensionResult* SuffixTreeBuilder::ExtendFromPosition(PositionInTree* startPosition, int indexInString, int extensionNumber)
{
	if (startPosition->IsInVertex())
	{
		Vertex* startVertex = startPosition->GetVertex();
		//delete startPosition;
		return ExtendFromVertex(startVertex, indexInString, extensionNumber);
	}

	if (startPosition->IsInArch())
	{
		return ExtendFromArch(startPosition, indexInString, extensionNumber);
	}

	// ���� �� ������ ������.
	throw AppException("Wrong tree");
}

SuffixTreeBuilder::ExtensionResult* SuffixTreeBuilder::ExtendFromVertex(Vertex* vertex, int indexInString, int extensionNumber )
{
	Arch* nextArch = FindNextArch(vertex, indexInString);
	// ���� �� ������� ��� ���� � ������� ������, �� ��������� ����.
	if (nextArch == NULL)
	{
		Arch* newArch = new Arch(extensionNumber, indexInString, -1);
		vertex->AddOutcomeArch(newArch);
		return new ExtensionResult(new PositionInTree(newArch, indexInString), vertex, false);
	}

	// ���� ���� ����� ���� �� ������� => 3 ������� - �������� ����� �� ��� ����.
	return new ExtensionResult(new PositionInTree(nextArch, nextArch->_startIndex), vertex, true);
}

SuffixTreeBuilder::ExtensionResult* SuffixTreeBuilder::ExtendByAddingNewVertex(PositionInTree* startPosition, int extensionNumber, int indexInString)
{
	Arch* currentArch = startPosition->GetArch();
	int indexInArch = startPosition->GetIndexInArch();
	//delete startPosition;

	// ������� ������� ���� �� �������.
	Vertex* prevVertex = currentArch->_prevVertex;
	prevVertex->RemoveOutcomeArch(currentArch);
	// ������� ����� ����, ������� �������� �����.
	Arch* commonArch = new Arch(-1, currentArch->_startIndex, indexInArch);
	prevVertex->AddOutcomeArch(commonArch);
	// �������� ������ ������� ����.
	currentArch->_startIndex = indexInArch + 1;
	// ������� ����� ����, ������� �������� ������������.
	Arch* newArch = new Arch(extensionNumber, indexInString, -1);
	// ������� ����� �������.
	Vertex* newVertex = new Vertex(commonArch);
	newVertex->AddOutcomeArch(currentArch);
	newVertex->AddOutcomeArch(newArch);

	return new ExtensionResult(new PositionInTree(newArch, newArch->_startIndex), newVertex, false);
}

SuffixTreeBuilder::ExtensionResult* SuffixTreeBuilder::ExtendFromArch(PositionInTree* startPosition, int indexInString, int extensionNumber)
{
	Arch* currentArch = startPosition->GetArch();
	int indexInArch = startPosition->GetIndexInArch();

	// ���� ��������� ������ �������� ��������� � ����.
	if (startPosition->IsLastArchIndex(_lastAddedIndex))
	{
		// ���� �� ����� �� ������� ������� - ��� ��������, �� ������ �� ������.
		if (currentArch->IsLeaf())
		{
			return new ExtensionResult(new PositionInTree(currentArch, indexInArch + 1), NULL, false);
		}

		// ����� ���������� �� �������.
		return ExtendFromVertex(currentArch->_nextVertex, indexInString, extensionNumber);

	}
	else
	{
		// ���� ��������� ������ ����� ��, �� ������ �������
		if (_stringComparer->AreEqual(indexInArch + 1, indexInString))
		{
			//delete startPosition;
			return new ExtensionResult(new PositionInTree(currentArch, indexInArch + 1), NULL, true);
		}

		// ���� ������ �� ����� ��, �� ��������� ������� ���� � ��������� ����� �������.

		return ExtendByAddingNewVertex(startPosition, extensionNumber, indexInString);
	}
}

void SuffixTreeBuilder::ChangeLastIndex(Vertex* vertex)
{
	TList<Arch*>* arches = vertex->GetOutcomeArches();
	int archesCount = arches->GetCount();
	for (int i = 0; i < archesCount; i++)
	{
		Arch* arch = arches->GetElement(i);
		if (arch->IsLeaf())
		{
			arch->_finishIndex = _lastAddedIndex;
		}
		else
		{
			ChangeLastIndex(arch->_nextVertex);
		}
	}
}

void SuffixTreeBuilder::TryToAddLinkedVertex(Vertex* lastNewInnerVertex, ExtensionResult* endOfPhase, Vertex* root)
{
	// ��������� ���������� ������ ����������� �������.
	if (lastNewInnerVertex != NULL && !lastNewInnerVertex->HasLinkedVertex())
	{
		//������� ����� ����� ����������� �� ������� ���� �������� � �������
		if (endOfPhase->newInnerVertex != NULL)
		{
			lastNewInnerVertex->AddLinkedVertex(endOfPhase->newInnerVertex);
		}
		//� �������������, ��� ���� �� ������� ���� ������� ���� ���������, � �� ������� ���, �� ��� ������� - ������
		else
		{
			lastNewInnerVertex->AddLinkedVertex(root);
		}
	}
}

SuffixTreeBuilder::~SuffixTreeBuilder()
{
	delete _stringComparer;
}


