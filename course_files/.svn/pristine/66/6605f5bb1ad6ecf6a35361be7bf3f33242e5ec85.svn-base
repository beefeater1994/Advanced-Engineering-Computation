#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include <cstddef>
#include <vector>

#include "bintree.h"



void Print(const BinaryTree<int,int> &tree,BinaryTree<int,int>::NodeHandle ndHd)
{
	if(ndHd.IsNotNull())
	{
		Print(tree,tree.Left(ndHd));
		printf(": %d\n",tree.GetKey(ndHd));
		Print(tree,tree.Right(ndHd));
	}
}


int main(void)
{
	auto seed=(int)time(nullptr);
	srand(seed);

	BinaryTree <int,int> tree;

	for(int i=0; i<20; ++i)
	{
		tree.Insert(rand()%100,0);
	}

	std::vector <BinaryTree <int,int>::NodeHandle> toDel;

	// Print(tree,tree.RootNode());
	for(auto ndHd=tree.First(); true==ndHd.IsNotNull(); ndHd=tree.FindNext(ndHd))
	{
		printf("@ %d\n",tree.GetKey(ndHd));
		if(1==(tree.GetKey(ndHd)&1))
		{
			toDel.push_back(ndHd);
		}
	}

	for(auto hd : toDel)
	{
		tree.Delete(hd);
	}

	for(auto ndHd=tree.First(); true==ndHd.IsNotNull(); ndHd=tree.FindNext(ndHd))
	{
		printf("& %d\n",tree.GetKey(ndHd));
	}

	return 0;
}
