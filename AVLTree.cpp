#include "AVLTree.h"

AVLTree::AVLTree()
{
	root = NULL;
	count = 0;
}

AVLTree::~AVLTree() 
{
	if (root != NULL) 
	{
		TreeFullDelete(root);
	}
}

TreeNode* AVLTree::GetRoot()
{
	return root;
}
void AVLTree::SetRoot(TreeNode* newRoot)
{
	root = newRoot;
}

TreeNode* AVLTree::TreeSearch(char key)
{
	TreeNode* x = root;
	while ((x != NULL) && (key != x->key))
	{
		if (key != SuffixTree::LAST_CHARACTER && key < x->key)
		{
			x = x->left;
		}
		else 
		{
			x = x->right;
		}
	}
	return x;
}

Result AVLTree::TreeInsert(char key, Arch* data)
{
	TreeNode* y = NULL;
	TreeNode* x = root;
	while (x != NULL)
	{
		y = x;
		//���� ���� ��� ����������
		if (key == x->key) 
		{
			//free(key);
			return EXIST;
		}
		// ���� ����� ���� ������ ��������, �� ���� �����
		if (key != SuffixTree::LAST_CHARACTER && key < x->key)
		{
			x = x->left;
		}
		else 
		{
			x = x->right;
		}
	}
	//����� ����� ��� �������,��������
	TreeNode* z = (TreeNode*)malloc(sizeof(TreeNode));
	//���� ������ �� ��������
	if (z == NULL)
	{
		//delete data;
		return MEMORY_ERROR;
	}

	*z = TreeNode(key, data);
	z->parent = y;
	//���� ������ ������
	if (y == NULL)
	{
		root = z;
	}
	else 
	{
		// ���� ����� ���� ������ �������������, �� ��������� ���� � ����� ���������

		if (key != SuffixTree::LAST_CHARACTER && key < y->key)
		{
			y->left = z;
			y->balance++;
		}
		// ����� � ������
		else
		{
			y->right = z;
			y->balance--;
		}
		//������������ ���������� ������������������ � ������ ����
		CorrectInsertBalance(y);
	}
	count++;
	return OK;
}

Result AVLTree::TreeDelete(char key)
{
	TreeNode* delNode = TreeSearch(key);
	if (delNode == NULL) 
	{
		return NO_SUCH_WORD;
	}
	TreeNodeDelete(delNode);
	count--;
	return OK;
}

void AVLTree::TreeNodeDelete(TreeNode* z)
{
	TreeNode* y;
	//���� ��� ������ �� �����������
	if ((z->left == NULL) || (z->right == NULL))
		y = z;
	else 
	{
		y = TreeSuccesor(z);
	}
	TreeNode* x;
	//���� ���� �����, �� ���� �����
	if (y->left != NULL)
		x = y->left;
	else 
		x = y->right;
	//���� � y ���� �������, �� ��� ��������� ���������� �������� y
	if (x != NULL)
		x->parent = y->parent;
	//���� y - ������
	if (y->parent == NULL)
		root = x;
	else
	{
		//���� y - ����� ����
		if(y == y->parent->left)
		{
			y->parent->left = x;
			y->parent->balance--;
		}
		else 
		{
			y->parent->right = x;
			y->parent->balance++;
		}
		CorrectDeleteBalance(y->parent);
	}

	//delete z->data;
	if (y != z)
	{
		//����������� ������ �� ����
		z->key = y->key;
		z->data = y->data;
	}

	free(y);
}

void AVLTree::TreeFullDelete(TreeNode* x)
{
	if (x->left != NULL) 
	{
		TreeFullDelete(x->left);
	}

	if (x->right != NULL) 
	{
		TreeFullDelete(x->right);
	}

	delete x->data;
	free(x);
}

TreeNode* AVLTree::TreeMin(TreeNode* x)
{
	while (x->left != NULL)
	{
		x = x->left;
	}

	return x;
}

TreeNode* AVLTree::TreeMin()
{
	return TreeMin(root);
}

TreeNode* AVLTree::TreeSuccesor(TreeNode* x)
{
	//���� ���� ������ ���������, �� ��������� ������� - ��� ����������� � ���������
	if (x->right != NULL) 
	{
		return TreeMin(x->right);
	}

	TreeNode* y = x->parent;
	//���� ���� �������� � ���� �� � ������ ��������� ��������
	while ((y != NULL) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}
	//��� ������ �� ����������� � ����� ��������� ��������, �� ���������� ��������
	return y;
}

void AVLTree::CorrectInsertBalance(TreeNode* x)
{
	//������ x ��� ���������
	while ((x->balance != 0) && (x->balance != 2) && (x->balance != -2) && (x->parent != NULL))
	{
		//���� x - ����� ����
		if (x == x->parent->left) 
			x->parent->balance++;
		//���� x - ������ ����
		else 
			x->parent->balance--;
		x = x->parent;
	}
	if ((x->balance == 2) || (x->balance == -2)) TreeBalance(x);
}

void AVLTree::CorrectDeleteBalance(TreeNode* x)
{
	// TreeBalance ���������� ��������� �� ������ ��������
	if ((x->balance == 2) || (x->balance == -2)) x = TreeBalance(x);

	//������ x ��� ���������, ���������� ���� �������, ���� 
	while ((x->balance != 1) && (x->balance != -1) && (x->parent != NULL))
	{
		//���� ������ �����
		if (x == x->parent->left) x->parent->balance--;
		//���� ������ ������
		else x->parent->balance++;

		if ((x->parent->balance == 2) || (x->parent->balance == -2)) 
		{
			x = TreeBalance(x->parent);
			continue;
		}

		//�� ������� ����
		if (x->parent != NULL) x = x->parent;
	}
}


TreeNode* AVLTree::TreeBalance(TreeNode* x)
{
	// ���� ����� ��������� �� 2 ������ �������
	if (x->balance == 2)
	{
		// ������ ��������
		if ((x->left->balance == 1) || (x->left->balance == 0)) return Rrotate(x);
		// ������� �������� (������� ������ ��������)
		// x->left == -1;
		else return LRrotate(x);
	}
	// ���� ������ ��������� �� 2 ������ ������
	// balance == -2
	else
	{
		if ((x->right->balance == -1) || (x->right->balance == 0)) return Lrotate(x);
		//������� ����� ��������
		else return RLrotate(x);
	}
}

void AVLTree::SwapNodes(TreeNode* a, TreeNode* b)
{
	//������ ������� a � b (a ���������� child b, �������� a ���������� ���������� b)
	if (a->parent != NULL)
	{
		if (a->parent->left == a)
			a->parent->left = b;
		else
			a->parent->right = b;
	}
	else root = b;
	b->parent = a->parent;
	a->parent = b;
}

TreeNode* AVLTree::Rrotate(TreeNode* a)
{
	TreeNode* b = a->left;
	TreeNode* c = b->right;

	SwapNodes(a, b);
	b->right = a;
	a->left = c;

	// ������������� �������� c
	if (c != NULL) c->parent = a;
	// ������ ���������� ������������������
	//���� � == L
	if (b->balance == 0) 
	{
		a->balance = 1;
		b->balance = -1;
	}
	//���� C < L
	else
		a->balance = b->balance = 0;
	return b;
}

TreeNode* AVLTree::Lrotate(TreeNode* a)
{
	TreeNode* b = a->right;
	TreeNode* c = b->left;

	SwapNodes(a, b);
	b->left = a;
	a->right =c;

	// ������������� �������� c
	if (c != NULL) c->parent = a;
	// ������ ���������� ������������������
	//���� � == R
	if (b->balance == 0) 
	{
		a->balance = -1;
		b->balance = 1;
	}
	//���� C < R
	else
		a->balance = b->balance = 0;

	return b;
}

//������� ������
TreeNode* AVLTree::LRrotate(TreeNode* a)
{
	TreeNode* b = a->left;
	TreeNode* c = b->right;
	TreeNode* m = c->left;
	TreeNode* n = c->right;

	SwapNodes(a, c);

	c->right = a;
	c->left = b;
	b->parent = c;
	b->right = m;
	a->left = n;

	if (n != NULL) n->parent = a;
	if (m != NULL) m->parent = b;

	CorrectRotationBalance(b, a, c);
	return c;
}

//������� ����� 
TreeNode* AVLTree::RLrotate(TreeNode* a)
{
	TreeNode* b = a->right;
	TreeNode* c = b->left;
	TreeNode* m = c->left;
	TreeNode* n = c->right;

	SwapNodes(a, c);

	c->left = a;
	c->right = b;
	b->parent = c;
	a->right = m;
	b->left = n;

	if (n != NULL) n->parent = b;
	if (m != NULL) m->parent = a;

	CorrectRotationBalance(a, b, c);
	return c;
}


void AVLTree::CorrectRotationBalance(TreeNode* leftN, TreeNode* rightN, TreeNode* c)
{
	// ������ leftN - �����, rightN - ������
	switch(c->balance)
	{
	case 0: // m == n
		{
			leftN->balance = rightN->balance = 0;
			break;
		}
	case 1: // m > n
		{
			leftN->balance = 0;
			rightN->balance = -1;
			break;
		}
	case -1: // m < n
		{
			leftN->balance = 1;
			rightN->balance = 0;
			break;
		}
	}
	c->balance = 0;
}

int AVLTree::GetCount()
{
	return count;
}


