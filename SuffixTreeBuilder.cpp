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
	//результат окончания продолжения
	ExtensionResult extensionResult;
	//позиция, с которого выполняется продолжение
	PositionInTree startPosition;;
	AddFirstElement(&startPosition, root);
	extensionResult.SetExtensionResult(startPosition.arch, startPosition.indexInArch, NULL, true);

	// номер продолжения j, с которого начинается фаза.
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
			// добавляем продолжения до конца фазы
			while(startExtension <= i)
			{
				Vertex* lastNewInnerVertex = isFirstExtensionOfPhase ? NULL : extensionResult.newInnerVertex;
				// Если на предыдущем шаге была выделена вершина, то с нее начинать поиск места продолжения.
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
				// Если применилось третье правило.
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
	// Выполням 0 фазу: добавляем корень и одну листовую дугу.
	Arch* firstArch = new Arch(0, 0, -1);
	root->AddOutcomeArch(GetStringElement(firstArch->startIndex), firstArch);
	_lastAddedIndex = 0;
	startPosition->SetPositionToArch(firstArch, 0);
}

void SuffixTreeBuilder::UpdatePlaceToExtend(PositionInTree* startPosition)
{
	// Если предыдущее место продолжения в вершине, то 
	// либо проходим возвращаем вершину (если корень),
	// либо проходим по связи и возвращаем связанную вершину (если в текущей вершине уже есть связь),
	// либо поднимаемся до следующей вершины, проходим по связи и ищем конец суффикса.
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
		// Если это корень, то начинаем с него, иначе проходим по связи.
		if (prevVertex->IsRoot())
		{
			FindEndOfSuffix(startPosition, prevVertex, incomeArch->startIndex + 1, incomeArch->GetFinishIndex(_lastAddedIndex));
		}
		else
		{
			FindEndOfSuffix(startPosition, prevVertex->GetLinkedVertex(), incomeArch->startIndex, incomeArch->GetFinishIndex(_lastAddedIndex));
		}
	}
	// Если предыдущее место продолжения на дуге, то поднимаемся в вершину, проходим по связи и ищем конец суффикса.
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

// Используем прыжки по счетчику от заданной вершины.
void SuffixTreeBuilder::FindEndOfSuffix(PositionInTree* startPosition, Vertex* startVertex, int startIndex, int finishIndex)
{
	// Если не надо проходить по дуге.
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

// Выполняем одно из трех правил продолжения.
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
	// Если из вершины нет дуги с искомой меткой, то добавляем дугу.
	if (nextArch == NULL)
	{
		Arch* newArch = new Arch(extensionNumber, indexInString, -1);
		vertex->AddOutcomeArch(GetStringElement(newArch->startIndex), newArch);
		extensionResult->SetExtensionResult(newArch, indexInString, vertex, false);
	}
	else
	{
		// Если есть такая дуга из вершины => 3 правило - сдвигаем конец на эту дугу.
		extensionResult->SetExtensionResult(nextArch, nextArch->startIndex,  vertex, true);
	}
}

void SuffixTreeBuilder::ExtendByAddingNewVertex(ExtensionResult* extensionResult, PositionInTree* startPosition, int extensionNumber, int indexInString)
{
	Arch* currentArch = startPosition->arch;
	int indexInArch = startPosition->indexInArch;

	// Удаляем текущую дугу из вершины.
	Vertex* prevVertex = currentArch->prevVertex;
	prevVertex->RemoveOutcomeArch(GetStringElement(currentArch->startIndex), currentArch);
	// Создаем новую дугу, которая является общей.
	Arch* commonArch = new Arch(-1, currentArch->startIndex, indexInArch);
	prevVertex->AddOutcomeArch(GetStringElement(commonArch->startIndex), commonArch);
	// Изменяем начало текущей дуги.
	currentArch->startIndex = indexInArch + 1;
	// Создаем новую дугу, которая является ответвлением.
	Arch* newArch = new Arch(extensionNumber, indexInString, -1);
	// Создаем новую вершину.
	Vertex* newVertex = new Vertex(commonArch);
	newVertex->AddOutcomeArch(GetStringElement(currentArch->startIndex), currentArch);
	newVertex->AddOutcomeArch(GetStringElement(newArch->startIndex), newArch);

	extensionResult->SetExtensionResult(newArch, newArch->startIndex, newVertex, false);
}

void SuffixTreeBuilder::ExtendFromArch(ExtensionResult* extensionResult, PositionInTree* startPosition, int indexInString, int extensionNumber)
{
	Arch* currentArch = startPosition->arch;
	int indexInArch = startPosition->indexInArch;

	// Если начальный индекс является последним в дуге.
	if (startPosition->IsLastArchIndex(_lastAddedIndex))
	{
		// Если за дугой не следует вершины - она листовая, то ничего не делаем.
		if (currentArch->IsLeaf())
		{
			extensionResult->SetExtensionResult(currentArch, indexInArch + 1, NULL, false);
		}
		else
		{
			// Иначе продлеваем из вершины.
			ExtendFromVertex(extensionResult, currentArch->nextVertex, indexInString, extensionNumber);
		}
	}
	else
	{
		// Если следующий символ такой же, то третье правило
		if (AreEqualsChars(indexInArch + 1, indexInString))
		{
			extensionResult->SetExtensionResult(currentArch, indexInArch + 1, NULL, true);
		}
		else
		{
			// Если символ не такой же, то разбиваем текущую дугу и добавляем новую вершину.
			ExtendByAddingNewVertex(extensionResult, startPosition, extensionNumber, indexInString);
		}
	}
}

void SuffixTreeBuilder::TryToAddLinkedVertex(Vertex* lastNewInnerVertex, ExtensionResult* extensionResult, Vertex* root)
{
	// Соединяем суффиксной связью добавленные вершины.
	if (lastNewInnerVertex != NULL && !lastNewInnerVertex->HasLinkedVertex())
	{
		//создаем связь между добавленной на прошлом шаге вершиной и текущей
		if (extensionResult->newInnerVertex != NULL)
		{
			lastNewInnerVertex->AddLinkedVertex(extensionResult->newInnerVertex);
		}
		//в предположении, что если на прошлом шаге вершина была добавлена, а на текущем нет, то эта вершина - корень
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
