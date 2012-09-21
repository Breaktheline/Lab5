#ifndef H_AVLTREE
#define H_AVLTREE

#include "Arch.h"
#include "Common.h"
#include "SuffixTree.h"

class Arch;

enum Result {OK, EXIST, NO_SUCH_WORD, SEARCH_OK, MEMORY_ERROR, FILE_ERROR, INPUT_ERROR, LOAD_ERROR, SAVE_ERROR};

struct TreeNode
{
	char key;
	Arch* data;
	TreeNode* parent;
	TreeNode* left;
	TreeNode* right;
	int balance;

	TreeNode(char nKey, Arch* nData, int nBalance = 0)
	{
		parent = left = right = NULL;
		key = nKey;
		data = nData;
		balance = nBalance;
	}

};

class AVLTree
{
public:
	AVLTree();
	~AVLTree();

	TreeNode* GetRoot();
	void SetRoot(TreeNode* newRoot);

	TreeNode* TreeSearch(char key);
	Result TreeInsert(char key, Arch* data);
	Result TreeDelete(char key);
	void TreeFullDelete(TreeNode* x);
	TreeNode* TreeMin();
	int GetCount();
private:

	TreeNode* root;
	int count;
	TreeNode* TreeMin(TreeNode* x);
	TreeNode* TreeSuccesor(TreeNode* x);
	void TreeNodeDelete(TreeNode* x);

	void CorrectInsertBalance(TreeNode* x);
	void CorrectDeleteBalance(TreeNode* x);
	void CorrectRotationBalance(TreeNode* lN, TreeNode* rN, TreeNode* c);

	TreeNode* TreeBalance(TreeNode* x);
	TreeNode* Rrotate(TreeNode* a);
	TreeNode* Lrotate(TreeNode* a);
	TreeNode* LRrotate(TreeNode* a);
	TreeNode* RLrotate(TreeNode* a);
	void SwapNodes(TreeNode* a, TreeNode* b);
	
};

#endif

