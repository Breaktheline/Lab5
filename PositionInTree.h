#ifndef H_POSITION_IN_TREE
#define H_POSITION_IN_TREE

#include "Vertex.h"
#include "Arch.h"

class PositionInTree
{
public:
	PositionInTree();

	void SetPosition(PositionInTree* position);
	void  SetPositionToArch(Arch* arch, int index);
	void  SetPositionToVertex(Vertex* vertex);

	bool IsInVertex();
	bool IsInArch();

	bool IsLastArchIndex(int lastAddedIndex);
	bool IsNextArchIndex(int lastAddedIndex, int nextIndex);

	Vertex* vertex;
	Arch* arch;
	int indexInArch;
};

#endif

