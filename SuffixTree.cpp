#include "SuffixTree.h"

const char SuffixTree::LAST_CHARACTER = '$';

SuffixTree::SuffixTree(TList<char>* string, Vertex* root)
{
	_string = string;
	_root = root;
}

Vertex* SuffixTree::GetRoot()
{
	return _root;
}

TList<char>* SuffixTree::GetString()
{
	return _string;
}

SuffixTree::~SuffixTree()
{
	//delete _root;
}

int SuffixTree::FindLeafArchNumber(Arch* arch)
{
	while(!arch->IsLeaf())
	{
		arch = arch->nextVertex->GetRootArch();
	}
	
	return arch->GetNumber();
}

