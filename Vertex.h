#ifndef H_VERTEX
#define H_VERTEX

#include "Arch.h"
#include "AVLTree.h"

class Arch;
class AVLTree;
struct TreeNode;

class Vertex
{
public:
	Vertex(Arch* incomeArch);
	~Vertex();

	void AddOutcomeArch(char startChar, Arch* outcomeArch);
	void AddLinkedVertex(Vertex* linkedVertex);

	bool IsRoot();
	bool HasLinkedVertex();

	Vertex* GetLinkedVertex();
	Arch* GetIncomeArch();
	void RemoveOutcomeArch(char startChar, Arch* currentArch);
	Arch* FindOutcomeArch(char startChar);
	Arch* FindMinOutcomeArch();
	Arch* GetRootArch();
	int GetOutcomeArchesCount();
private:
	void DeleteNode(TreeNode* node);
	Arch* _incomeArch;
	AVLTree* _outcomeArches;
	Vertex* _linkedVertex;
};

#endif

