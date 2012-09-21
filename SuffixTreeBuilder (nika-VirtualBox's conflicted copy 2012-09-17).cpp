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

	// номер продолжения j, с которого начинается фаза.
	int startExtension = 1;
	int stringCount = _stringComparer->GetStringCount();
	for(int i = 1; i < stringCount; i++)
	{
		PositionInTree* startPosition = NULL;
		startExtension--;
		// добавляем продолжения до конца фазы
		while(startExtension <= i)
		{
			Vertex* lastNewInnerVertex = startPosition == NULL ? NULL : endOfPhase->newInnerVertex;
			// Если на предыдущем шаге была выделена вершина, то с нее начинать поиск места продолжения.
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
			// Если применилось третье правило.
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
	// Выполням 0 фазу: добавляем корень и одну листовую дугу.
	Arch* firstArch = new Arch(0, 0, -1);
	root->AddOutcomeArch(firstArch);
	_lastAddedIndex = 0;
	return new PositionInTree(firstArch, 0);
}

PositionInTree* SuffixTreeBuilder::FindPlaceToExtend(PositionInTree* startPosition)
{
	// Если предыдущее место продолжения в вершине, то 
	// либо проходим возвращаем вершину (если корень),
	// либо проходим по связи и возвращаем связанную вершину (если в текущей вершине уже есть связь),
	// либо поднимаемся до следующей вершины, проходим по связи и ищем конец суффикса.
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
		// Если это корень, то начинаем с него, иначе проходим по связи.
		if (prevVertex->IsRoot())
		{
			return FindEndOfSuffix(prevVertex, incomeArch->_startIndex + 1, incomeArch->GetFinishIndex(_lastAddedIndex));
		}

		return FindEndOfSuffix(prevVertex->GetLinkedVertex(), incomeArch->_startIndex, incomeArch->GetFinishIndex(_lastAddedIndex));
	}

	// Если предыдущее место продолжения на дуге, то поднимаемся в вершину, проходим по связи и ищем конец суффикса.
	if (startPosition->IsInArch())
	{
		Vertex* prevVertex = startPosition->GetArchPrevVertex();
		Vertex* startVertex = prevVertex->IsRoot() ? prevVertex : prevVertex->GetLinkedVertex();
		int startIndex = startPosition->GetArchStartIndex() + 1;
		int finishIndex = startPosition->GetIndexInArch();
		delete startPosition;
		return FindEndOfSuffix(startVertex, startIndex, finishIndex);
	}

	// Сюда не должны придти.
	throw AppException("Wrong tree");
}

// Используем прыжки по счетчику от заданной вершины.
PositionInTree* SuffixTreeBuilder::FindEndOfSuffix(Vertex* startVertex, int startIndex, int finishIndex)
{
	// Если не надо проходить по дуге.
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

// Выполняем одно из трех правил продолжения.
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

	// Сюда не должны придти.
	throw AppException("Wrong tree");
}

SuffixTreeBuilder::ExtensionResult* SuffixTreeBuilder::ExtendFromVertex(Vertex* vertex, int indexInString, int extensionNumber )
{
	Arch* nextArch = FindNextArch(vertex, indexInString);
	// Если из вершины нет дуги с искомой меткой, то добавляем дугу.
	if (nextArch == NULL)
	{
		Arch* newArch = new Arch(extensionNumber, indexInString, -1);
		vertex->AddOutcomeArch(newArch);
		return new ExtensionResult(new PositionInTree(newArch, indexInString), vertex, false);
	}

	// Если есть такая дуга из вершины => 3 правило - сдвигаем конец на эту дугу.
	return new ExtensionResult(new PositionInTree(nextArch, nextArch->_startIndex), vertex, true);
}

SuffixTreeBuilder::ExtensionResult* SuffixTreeBuilder::ExtendByAddingNewVertex(PositionInTree* startPosition, int extensionNumber, int indexInString)
{
	Arch* currentArch = startPosition->GetArch();
	int indexInArch = startPosition->GetIndexInArch();
	//delete startPosition;

	// Удаляем текущую дугу из вершины.
	Vertex* prevVertex = currentArch->_prevVertex;
	prevVertex->RemoveOutcomeArch(currentArch);
	// Создаем новую дугу, которая является общей.
	Arch* commonArch = new Arch(-1, currentArch->_startIndex, indexInArch);
	prevVertex->AddOutcomeArch(commonArch);
	// Изменяем начало текущей дуги.
	currentArch->_startIndex = indexInArch + 1;
	// Создаем новую дугу, которая является ответвлением.
	Arch* newArch = new Arch(extensionNumber, indexInString, -1);
	// Создаем новую вершину.
	Vertex* newVertex = new Vertex(commonArch);
	newVertex->AddOutcomeArch(currentArch);
	newVertex->AddOutcomeArch(newArch);

	return new ExtensionResult(new PositionInTree(newArch, newArch->_startIndex), newVertex, false);
}

SuffixTreeBuilder::ExtensionResult* SuffixTreeBuilder::ExtendFromArch(PositionInTree* startPosition, int indexInString, int extensionNumber)
{
	Arch* currentArch = startPosition->GetArch();
	int indexInArch = startPosition->GetIndexInArch();

	// Если начальный индекс является последним в дуге.
	if (startPosition->IsLastArchIndex(_lastAddedIndex))
	{
		// Если за дугой не следует вершины - она листовая, то ничего не делаем.
		if (currentArch->IsLeaf())
		{
			return new ExtensionResult(new PositionInTree(currentArch, indexInArch + 1), NULL, false);
		}

		// Иначе продлеваем из вершины.
		return ExtendFromVertex(currentArch->_nextVertex, indexInString, extensionNumber);

	}
	else
	{
		// Если следующий символ такой же, то третье правило
		if (_stringComparer->AreEqual(indexInArch + 1, indexInString))
		{
			//delete startPosition;
			return new ExtensionResult(new PositionInTree(currentArch, indexInArch + 1), NULL, true);
		}

		// Если символ не такой же, то разбиваем текущую дугу и добавляем новую вершину.

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
	// Соединяем суффиксной связью добавленные вершины.
	if (lastNewInnerVertex != NULL && !lastNewInnerVertex->HasLinkedVertex())
	{
		//создаем связь между добавленной на прошлом шаге вершиной и текущей
		if (endOfPhase->newInnerVertex != NULL)
		{
			lastNewInnerVertex->AddLinkedVertex(endOfPhase->newInnerVertex);
		}
		//в предположении, что если на прошлом шаге вершина была добавлена, а на текущем нет, то эта вершина - корень
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


