#ifndef H_ARCH
#define H_ARCH

#include "Vertex.h"

class Vertex;

class Arch
{
public:
	Arch(int number, int startIndex, int finishIndex);
	int GetFinishIndex(int lastAddedIndex);
	int GetRealLength();
	int GetLength(int lastAddedIndex);
	int GetNumber();
	bool IsLeaf();

	Vertex* prevVertex;
	Vertex* nextVertex;
	int startIndex;
	// Индекс символа в строке: если имеется ввиду последний пройденный символ (e), то индекс -1.
	int finishIndex;

private:
	// Номер дуги: если дуга не имеет листа, то ее номер -1.
	int _number;
};

#endif

