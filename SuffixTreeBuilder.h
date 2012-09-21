#ifndef H_SUFFIX_TREE_BUILDER
#define H_SUFFIX_TREE_BUILDER

#include "Common.h"
#include "TList.h"
#include "Vertex.h"
#include "Arch.h"
#include "PositionInTree.h"
#include "SuffixTree.h"

class SuffixTreeBuilder
{
public:
	SuffixTreeBuilder(TList<char>* string);
	SuffixTree* BuildTree();

private:
	struct ExtensionResult
	{
		ExtensionResult* SetExtensionResult(Arch* arch, int indexInString, Vertex* aNewInnerVertex, bool aIsLastExtension)
		{
			positionInTree.SetPositionToArch(arch, indexInString);
			isLastExtension = aIsLastExtension;
			newInnerVertex = aNewInnerVertex;

			return this;
		}

		PositionInTree positionInTree;
		bool isLastExtension;
		Vertex* newInnerVertex;
	};

	void AddFirstElement(PositionInTree* startPosition, Vertex* root);
	void FindEndOfSuffix(PositionInTree* startPosition, Vertex* startVertex, int startIndex, int finishIndex);
	Arch* FindNextArch(Vertex* vertex, int index);
	bool AreEqualsChars(int firstIndex, int secondIndex);

	void UpdatePlaceToExtend(PositionInTree* startPosition);
	void ExtendFromPosition(ExtensionResult* extensionResult, PositionInTree* startPosition, int indexInString, int extensionNumber);
	void ExtendFromArch(ExtensionResult* extensionResult, PositionInTree* startPosition, int indexInString, int extensionNumber );
	void ExtendByAddingNewVertex(ExtensionResult* extensionResult, PositionInTree* startPosition, int extensionNumber, int indexInString);
	void ExtendFromVertex(ExtensionResult* extensionResult, Vertex* vertex, int indexInString, int extensionNumber);
	void ChangeLastIndex(Vertex* root); 
	void TryToAddLinkedVertex(Vertex* lastNewInnerVertex, ExtensionResult* endOfPhase, Vertex* root);

	int _lastAddedIndex;
	TList<char>* _string;
};

#endif

