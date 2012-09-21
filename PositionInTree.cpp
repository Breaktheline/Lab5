#include "PositionInTree.h"

PositionInTree::PositionInTree()
{
	vertex = NULL;
	arch = NULL;
}

bool PositionInTree::IsInVertex()
{
	return vertex != NULL;
}

bool PositionInTree::IsInArch()
{
	return arch != NULL;
}

//если последний индекс = -1
bool PositionInTree::IsLastArchIndex(int lastAddedIndex)
{
	return indexInArch == arch->GetFinishIndex(lastAddedIndex);
}

bool PositionInTree::IsNextArchIndex(int lastAddedIndex, int nextIndex)
{
	if (nextIndex <= arch->GetFinishIndex(lastAddedIndex))
	{
		return indexInArch + 1 == nextIndex;
	}

	return false;
}

void PositionInTree::SetPositionToArch(Arch* aArch, int aIndex)
{
	arch = aArch;
	indexInArch = aIndex;
	vertex = NULL;
}

void PositionInTree::SetPositionToVertex(Vertex* aVertex)
{
	vertex = aVertex;
	arch = NULL;
}

void PositionInTree::SetPosition(PositionInTree* position)
{
	arch = position->arch;
	indexInArch = position->indexInArch;
	vertex = position->vertex;
}

