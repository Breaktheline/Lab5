#include "Arch.h"

Arch::Arch(int aNnumber, int aStartIndex, int aFinishIndex)
{
	_number = aNnumber;
	startIndex = aStartIndex;
	finishIndex = aFinishIndex;
	prevVertex = NULL;
	nextVertex = NULL;
}

//void Arch::SetPrevVertex(Vertex* prevVertex)
//{
//	_prevVertex = prevVertex;
//}
//
//void Arch::SetNextVertex(Vertex* nextVertex)
//{
//	_nextVertex = nextVertex;
//}
//void Arch::ChangeStartIndex(int newStartIndex)
//{
//	_startIndex = newStartIndex;
//}
//
//void Arch::ChangeFinishIndex(int newFinishIndex)
//{
//	_finishIndex = newFinishIndex;
//}

int Arch::GetLength(int lastAddedIndex)
{
	return GetFinishIndex(lastAddedIndex) - startIndex + 1;
}

int Arch::GetRealLength()
{
	return finishIndex - startIndex + 1;
}

//Vertex* Arch::GetPrevVertex()
//{
//	return _prevVertex;
//}
//
//Vertex* Arch::GetNextVertex()
//{
//	return _nextVertex;
//}
//
//int Arch::GetStartIndex()
//{
//	return _startIndex;
//}

int Arch::GetFinishIndex(int lastAddedIndex)
{
	return finishIndex == -1 ? lastAddedIndex : finishIndex;
}

bool Arch::IsLeaf()
{
	return _number != -1;
}

int Arch::GetNumber()
{
	return _number;
}

//int Arch::GetRealFinishIndex()
//{
//	return _finishIndex;
//}


