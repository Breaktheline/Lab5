#ifndef H_SUFFIX_TREE
#define H_SUFFIX_TREE

#include "Vertex.h"
#include "Arch.h"
#include "TList.h"

class Vertex;
class Arch;

class SuffixTree
{
public:
	static const char LAST_CHARACTER;

	SuffixTree(TList<char>* string, Vertex* root);
	~SuffixTree();

	TList<char>* GetString();
	Vertex* GetRoot();
	int FindLeafArchNumber(Arch* arch);

private:
	Vertex* _root;
	TList<char>* _string;
};

#endif

