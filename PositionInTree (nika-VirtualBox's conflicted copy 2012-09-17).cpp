#include "PositionInTree.h"

PositionInTree::PositionInTree(Arch* arch, int index)
{
	_arch = arch;
	_indexInArch = index;
	_vertex = NULL;
}

PositionInTree::PositionInTree(Vertex* vertex)
{
	_vertex = vertex;
	_arch = NULL;
}

bool PositionInTree::IsInVertex()
{
	return _vertex != NULL;
}

bool PositionInTree::IsInArch()
{
	return _arch != NULL;
}

Vertex* PositionInTree::GetArchPrevVertex()
{
	return IsInArch() ? _arch->_prevVertex : NULL;
}

Vertex* PositionInTree::GetVertex()
{
	return IsInVertex() ? _vertex : NULL;
}

int PositionInTree::GetArchStartIndex()
{
	return _arch->_startIndex;
}

int PositionInTree::GetArchFinishIndex(int lastAddedIndex)
{
	return _arch->GetFinishIndex(lastAddedIndex);
}

int PositionInTree::GetIndexInArch()
{
	return _indexInArch;
}

//если последний индекс = -1
bool PositionInTree::IsLastArchIndex(int lastAddedIndex)
{
	return _indexInArch == _arch->GetFinishIndex(lastAddedIndex);
}

bool PositionInTree::IsNextArchIndex(int lastAddedIndex, int nextIndex)
{
	if (nextIndex <= _arch->GetFinishIndex(lastAddedIndex))
	{
		return _indexInArch + 1 == nextIndex;
	}

	return false;
}

Arch* PositionInTree::GetArch()
{
	return _arch;
}


