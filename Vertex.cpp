#include "Vertex.h"

Vertex::Vertex(Arch* incomeArch)
{
	if (incomeArch != NULL)
	{
		incomeArch->nextVertex = this;
	}

	_incomeArch = incomeArch;
	_linkedVertex = NULL;
	_outcomeArches = new AVLTree();
}

Vertex::~Vertex()
{
	DeleteNode(_outcomeArches->GetRoot());
	delete _outcomeArches;
}

void Vertex::AddOutcomeArch(char startChar, Arch* outcomeArch)
{
	outcomeArch->prevVertex = this;
	_outcomeArches->TreeInsert(startChar, outcomeArch);
}

void Vertex::AddLinkedVertex(Vertex* linkedVertex)
{
	_linkedVertex = linkedVertex;
}

bool Vertex::IsRoot()
{
	return _incomeArch == NULL;
}

bool Vertex::HasLinkedVertex()
{
	return _linkedVertex != NULL;
}

Vertex* Vertex::GetLinkedVertex()
{
	return _linkedVertex;
}

Arch* Vertex::GetIncomeArch()
{
	return _incomeArch;
}

void Vertex::RemoveOutcomeArch(char startChar, Arch* archToRemove)
{
	_outcomeArches->TreeDelete(startChar);
	archToRemove->prevVertex = NULL;
}

Arch* Vertex::FindOutcomeArch(char startChar)
{
	TreeNode* node = _outcomeArches->TreeSearch(startChar);
	if (node != NULL)
	{
		return node->data;
	}

	return NULL;
}

Arch* Vertex::FindMinOutcomeArch()
{
	TreeNode* minNode = _outcomeArches->TreeMin();
	if (minNode != NULL)
	{
		return minNode->data;
	}

	throw AppException("Wrong tree");
}



void Vertex::DeleteNode(TreeNode* node)
{
	if (node == NULL)
	{
		return;
	}

	Arch* arch = node->data;
	if (!arch->IsLeaf())
	{
		delete arch->nextVertex;
	}

	DeleteNode(node->left);
	DeleteNode(node->right);
}

int Vertex::GetOutcomeArchesCount()
{
	return _outcomeArches->GetCount();
}

Arch* Vertex::GetRootArch()
{
	TreeNode* root =  _outcomeArches->GetRoot();
	if (root == NULL)
	{
		return NULL;
	}

	return root->data;
}

