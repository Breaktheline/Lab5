#ifndef H_ARCH
#define H_ARCH

#include "Vertex.h"

class Vertex;

class Arch
{
public:
	Arch(int number, int startIndex, int finishIndex);

	//void SetPrevVertex(Vertex* prevVertex);
	//void SetNextVertex(Vertex* nextVertex);
	//void ChangeStartIndex(int newStartIndex);
	//void ChangeFinishIndex(int newFinishIndex);

	//Vertex* GetPrevVertex();
	//Vertex* GetNextVertex();

	//int GetStartIndex();
	//int GetRealFinishIndex();
	int GetFinishIndex(int lastAddedIndex);
	int GetRealLength();
	int GetLength(int lastAddedIndex);
	int GetNumber();
	bool IsLeaf();

	Vertex* _prevVertex;
	Vertex* _nextVertex;
	int _startIndex;
	// Индекс символа в строке: если имеется ввиду последний пройденный символ (e), то индекс -1.
	int _finishIndex;

private:
	// Номер дуги: если дуга не имеет листа, то ее номер -1.
	int _number;
};

#endif


