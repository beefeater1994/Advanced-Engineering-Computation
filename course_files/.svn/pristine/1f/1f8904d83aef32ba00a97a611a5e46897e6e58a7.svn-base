#include <stdio.h>
#include <time.h>
#include <stdlib.h>


class BinaryTreeNode
{
public:
	int key;
	BinaryTreeNode *up,*left,*right;

	BinaryTreeNode()
	{
		up=nullptr;
		left=nullptr;
		right=nullptr;
	}
};

void Print(const BinaryTreeNode *node)
{
	if(nullptr!=node)
	{
		Print(node->left);
		printf("%d\n",node->key);
		Print(node->right);
	}
}

BinaryTreeNode *AddNode(BinaryTreeNode *root,int key)
{
	if(nullptr==root)
	{
		root=new BinaryTreeNode;
		root->key=key;
	}
	else
	{
		BinaryTreeNode *ptr;
		ptr=root;
		for(;;)
		{
			if(key<ptr->key)
			{
				if(nullptr==ptr->left)
				{
					ptr->left=new BinaryTreeNode;
					ptr->left->up=ptr;
					ptr->left->key=key;
					break;
				}
				else
				{
					ptr=ptr->left;
				}
			}
			else
			{
				if(nullptr==ptr->right)
				{
					ptr->right=new BinaryTreeNode;
					ptr->right->up=ptr;
					ptr->right->key=key;
					break;
				}
				else
				{
					ptr=ptr->right;
				}
			}
		}
	}
	return root;
}

BinaryTreeNode *FirstNode(BinaryTreeNode *ptr)
{
	while(nullptr!=ptr && nullptr!=ptr->left)
	{
		ptr=ptr->left;
	}
	return ptr;
}

BinaryTreeNode *NextNode(BinaryTreeNode *ptr)
{
	if(nullptr!=ptr)
	{
		if(nullptr!=ptr->right)
		{
			return FirstNode(ptr->right);
		}
		while(nullptr!=ptr)
		{
			auto up=ptr->up;
			if(nullptr==up)
			{
				return nullptr;
			}
			else if(up->left==ptr)
			{
				return up;
			}
			ptr=up;
		}
	}
	return nullptr;
}
int main(void)
{
	srand((int)time(nullptr));

	BinaryTreeNode *root=nullptr;

	for(int i=0; i<10; ++i)
	{
		root=AddNode(root,rand()%100);
	}

	// Print(root);
	for(auto ptr=FirstNode(root); nullptr!=ptr; ptr=NextNode(ptr))
	{
		printf("> %d\n",ptr->key);
	}

	return 0;
}
