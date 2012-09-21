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
		//если узел уже существует
		if (key == x->key) 
		{
			//free(key);
			return EXIST;
		}
		// если новый ключ меньше текущего, то идем влево
		if (key != SuffixTree::LAST_CHARACTER && key < x->key)
		{
			x = x->left;
		}
		else 
		{
			x = x->right;
		}
	}
	//нашли место для вставки,родителя
	TreeNode* z = (TreeNode*)malloc(sizeof(TreeNode));
	//если память не выделена
	if (z == NULL)
	{
		//delete data;
		return MEMORY_ERROR;
	}

	*z = TreeNode(key, data);
	z->parent = y;
	//если дерево пустое
	if (y == NULL)
	{
		root = z;
	}
	else 
	{
		// если новый ключ меньше родительского, то добавляем узел в левое поддерево

		if (key != SuffixTree::LAST_CHARACTER && key < y->key)
		{
			y->left = z;
			y->balance++;
		}
		// иначе в правое
		else
		{
			y->right = z;
			y->balance--;
		}
		//корректируем показатели сбалансированности в каждом узле
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
	//если нет одного из поддеревьев
	if ((z->left == NULL) || (z->right == NULL))
		y = z;
	else 
	{
		y = TreeSuccesor(z);
	}
	TreeNode* x;
	//если есть левый, то идем влево
	if (y->left != NULL)
		x = y->left;
	else 
		x = y->right;
	//если у y есть потомок, то его родителем становится родитель y
	if (x != NULL)
		x->parent = y->parent;
	//если y - корень
	if (y->parent == NULL)
		root = x;
	else
	{
		//если y - левый узел
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
		//присваиваем ссылку на ключ
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
	//если есть правое поддерево, то следующий элемент - это минимальный в поддереве
	if (x->right != NULL) 
	{
		return TreeMin(x->right);
	}

	TreeNode* y = x->parent;
	//пока есть родитель и пока мы в правом поддереве родителя
	while ((y != NULL) && (x == y->right))
	{
		x = y;
		y = y->parent;
	}
	//как только мы оказываемся в левом поддереве родителя, то возвращаем родителя
	return y;
}

void AVLTree::CorrectInsertBalance(TreeNode* x)
{
	//баланс x уже поправлен
	while ((x->balance != 0) && (x->balance != 2) && (x->balance != -2) && (x->parent != NULL))
	{
		//если x - левый узел
		if (x == x->parent->left) 
			x->parent->balance++;
		//если x - правый узел
		else 
			x->parent->balance--;
		x = x->parent;
	}
	if ((x->balance == 2) || (x->balance == -2)) TreeBalance(x);
}

void AVLTree::CorrectDeleteBalance(TreeNode* x)
{
	// TreeBalance возвращает указатель на корень поворота
	if ((x->balance == 2) || (x->balance == -2)) x = TreeBalance(x);

	//баланс x уже поправлен, поправляем всех предков, пока 
	while ((x->balance != 1) && (x->balance != -1) && (x->parent != NULL))
	{
		//если удален слева
		if (x == x->parent->left) x->parent->balance--;
		//если удален справа
		else x->parent->balance++;

		if ((x->parent->balance == 2) || (x->parent->balance == -2)) 
		{
			x = TreeBalance(x->parent);
			continue;
		}

		//на уровень выше
		if (x->parent != NULL) x = x->parent;
	}
}


TreeNode* AVLTree::TreeBalance(TreeNode* x)
{
	// если левое поддерево на 2 больше правого
	if (x->balance == 2)
	{
		// правое вращение
		if ((x->left->balance == 1) || (x->left->balance == 0)) return Rrotate(x);
		// двойное вращение (большое правое вращение)
		// x->left == -1;
		else return LRrotate(x);
	}
	// если правое поддерево на 2 больше левого
	// balance == -2
	else
	{
		if ((x->right->balance == -1) || (x->right->balance == 0)) return Lrotate(x);
		//большое левое вращение
		else return RLrotate(x);
	}
}

void AVLTree::SwapNodes(TreeNode* a, TreeNode* b)
{
	//меняем местами a и b (a становится child b, родители a становятся родителями b)
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

	// устанавливаем родителя c
	if (c != NULL) c->parent = a;
	// меняем показатель сбалансированности
	//если С == L
	if (b->balance == 0) 
	{
		a->balance = 1;
		b->balance = -1;
	}
	//если C < L
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

	// устанавливаем родителя c
	if (c != NULL) c->parent = a;
	// меняем показатель сбалансированности
	//если С == R
	if (b->balance == 0) 
	{
		a->balance = -1;
		b->balance = 1;
	}
	//если C < R
	else
		a->balance = b->balance = 0;

	return b;
}

//большое правое
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

//большое левое 
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
	// всегда leftN - левое, rightN - правое
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


