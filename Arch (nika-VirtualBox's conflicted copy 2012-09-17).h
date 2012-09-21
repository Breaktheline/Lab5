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
	// ������ ������� � ������: ���� ������� ����� ��������� ���������� ������ (e), �� ������ -1.
	int _finishIndex;

private:
	// ����� ����: ���� ���� �� ����� �����, �� �� ����� -1.
	int _number;
};

#endif


