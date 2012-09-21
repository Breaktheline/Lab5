#include "SuffixTree.h"

SuffixTree::SuffixTree(TList<char>* string, Vertex* root)
{
	_string = string;
	_root = root;
}

Vertex* SuffixTree::GetRoot()
{
	return _root;
}

Arch* SuffixTree::FindMinArch(Vertex* vertex)
{
	TList<Arch*>* arches = vertex->GetOutcomeArches();
	int archFirstIndex = arches->GetElement(0)->_startIndex;
	char minChar = _string->GetElement(archFirstIndex);
	int minArchIndex = 0;
	int archesCount = arches->GetCount();
	for (int i = 1; i < archesCount; i++)
	{
		int archFirstIndex = arches->GetElement(i)->_startIndex;
		char currentChar = _string->GetElement(archFirstIndex);
		if (currentChar != StringComparer::LAST_CHARACTER && currentChar < minChar)
		{
			minChar = currentChar;
			minArchIndex = i;
		}
	}

	return arches->GetElement(minArchIndex);
}

TList<char>* SuffixTree::GetString()
{
	return _string;
}

SuffixTree::~SuffixTree()
{
	//TreeFullDelete(_root);
}

void SuffixTree::TreeFullDelete(Vertex* vertex)
{
	TList<Arch*>* arches = vertex->GetOutcomeArches();
	int archesCount = arches->GetCount();
	for (int i = 0; i < archesCount; i++)
	{
		Arch* arch = arches->GetElement(i);
		if (!arch->IsLeaf())
		{
			TreeFullDelete(arch->_nextVertex);
		}
	}

	delete vertex;
}

int SuffixTree::FindLeafArchNumber(Arch* arch)
{
	while(!arch->IsLeaf())
	{
		arch = arch->_nextVertex->GetOutcomeArch(0);
	}
	
	return arch->GetNumber();
}


