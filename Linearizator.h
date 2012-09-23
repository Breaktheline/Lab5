#ifndef H_LINEARIZATOR
#define H_LINEARIZATOR

#include "Vertex.h"
#include "Arch.h"
#include "SuffixTree.h"
#include "SuffixTreeBuilder.h"
#include "TList.h"

class Linearizator
{
public:
	int Linearize(TList<char>* inputString);

private:
	int FindMinString(SuffixTree* tree, int inputStringLength);
	void AddCopyToEnd(TList<char>* inputString);
};

#endif

