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
	// ������ ������� � ������: ���� ������� ����� ��������� ���������� ������ (e), �� ������ -1.
	int finishIndex;

private:
	// ����� ����: ���� ���� �� ����� �����, �� �� ����� -1.
	int _number;
};

#endif

