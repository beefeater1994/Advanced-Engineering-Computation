#ifndef BINTREE_IS_INCLUDED
#define BINTREE_IS_INCLUDED
/* { */

template <class KeyClass,class ValueClass>
class BinaryTree
{
protected:
	class Node
	{
	public:
		KeyClass key;
		ValueClass value;
		Node *up,*left,*right;

		Node()
		{
			up=nullptr;
			left=nullptr;
			right=nullptr;
		}
	};
public:
	class NodeHandle
	{
	friend BinaryTree <KeyClass,ValueClass>;
	private:
		Node *ptr;
	public:
		inline void Nullify(void)
		{
			ptr=nullptr;
		}
		inline bool IsNull(void) const
		{
			return ptr==nullptr;
		}
		inline bool IsNotNull(void) const
		{
			return ptr!=nullptr;
		}
		inline bool operator==(NodeHandle hd) const
		{
			return this->ptr==hd.ptr;
		}
		inline bool operator!=(NodeHandle hd) const
		{
			return this->ptr!=hd.ptr;
		}
		inline bool operator==(std::nullptr_t) const
		{
			return IsNull();
		}
		inline bool operator!=(std::nullptr_t) const
		{
			return IsNotNull();
		}
	};
protected:
	inline Node *GetNode(NodeHandle ndHd)
	{
		return ndHd.ptr;
	}
	inline const Node *GetNode(NodeHandle ndHd) const
	{
		return ndHd.ptr;
	}
	inline static NodeHandle MakeHandle(Node *nodePtr)
	{
		NodeHandle hd;
		hd.ptr=nodePtr;
		return hd;
	}
private:
	Node *root;
	long long int nElem;

public:
	BinaryTree()
	{
		root=nullptr;
		nElem=0;
	}
	~BinaryTree()
	{
		CleanUp();
	}
protected:
	void Free(Node *ptr)
	{
		if(nullptr!=ptr)
		{
			Free(ptr->left);
			Free(ptr->right);
			delete ptr;
		}
	}
public:
	void CleanUp(void)
	{
		Free(root);
		nElem=0;
		root=nullptr;
	}

	static NodeHandle Null(void)
	{
		NodeHandle hd;
		hd.Nullify();
		return hd;
	}
	NodeHandle RootNode(void) const
	{
		return MakeHandle(root);
	}
	NodeHandle Left(NodeHandle ndHd) const
	{
		auto ndPtr=GetNode(ndHd);
		if(nullptr!=ndPtr)
		{
			return MakeHandle(ndPtr->left);
		}
		return Null();
	}
	NodeHandle Up(NodeHandle ndHd) const
	{
		auto ndPtr=GetNode(ndHd);
		if(nullptr!=ndPtr)
		{
			return MakeHandle(ndPtr->up);
		}
		return Null();
	}
	NodeHandle Right(NodeHandle ndHd) const
	{
		auto ndPtr=GetNode(ndHd);
		if(nullptr!=ndPtr)
		{
			return MakeHandle(ndPtr->right);
		}
		return Null();
	}

	long long int GetN(void) const
	{
		return nElem;
	}
	/*! It is caller's responsibility to make sure ndHd is not a null handle.
	*/
	const KeyClass &GetKey(NodeHandle ndHd) const
	{
		auto ndPtr=GetNode(ndHd);
		return ndPtr->key;
	}
	ValueClass &GetValue(NodeHandle ndHd)
	{
		auto ndPtr=GetNode(ndHd);
		return ndPtr->value;
	}
	const ValueClass &GetValue(NodeHandle ndHd) const
	{
		auto ndPtr=GetNode(ndHd);
		return ndPtr->value;
	}
	NodeHandle FindNode(const KeyClass &key) const
	{
		auto ndHd=RootNode();
		while(ndHd.IsNotNull())
		{
			auto ndPtr=GetNode(ndHd);
			if(ndPtr->key==key)
			{
				return ndHd;
			}
			else if(key<ndPtr->key)
			{
				ndHd=Left(ndHd);
			}
			else
			{
				ndHd=Right(ndHd);
			}
		}
		return Null();
	}
	bool IsKeyIncluded(const KeyClass &key) const
	{
		auto found=FindNode(key);
		return found!=nullptr;
	}

	NodeHandle Insert(const KeyClass &key,const ValueClass &value)
	{
		if(nullptr==root)
		{
			root=new Node;
			root->key=key;
			root->value=value;
			++nElem;
			return MakeHandle(root);
		}
		else
		{
			++nElem;
			Node *ptr=root;
			for(;;)
			{
				if(key<ptr->key)
				{
					if(nullptr==ptr->left)
					{
						ptr->left=new Node;
						ptr->left->up=ptr;
						ptr->left->key=key;
						ptr->left->value=value;
						return MakeHandle(ptr->left);
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
						ptr->right=new Node;
						ptr->right->up=ptr;
						ptr->right->key=key;
						ptr->right->value=value;
						return MakeHandle(ptr->right);
					}
					else
					{
						ptr=ptr->right;
					}
				}
			}
		}
		return Null();
	}

protected:
	bool SimpleDetach(NodeHandle ndHd)
	{
		auto ndPtr=GetNode(ndHd);
		if(nullptr==ndPtr->left && nullptr==ndPtr->right)
		{
			if(nullptr==ndPtr->up)
			{
				root=nullptr;
			}
			else if(ndPtr->up->left==ndPtr)
			{
				ndPtr->up->left=nullptr;
			}
			else if(ndPtr->up->right==ndPtr)
			{
				ndPtr->up->right=nullptr;
			}
			return true;
		}
		else if(nullptr==ndPtr->left)
		{
			auto right=ndPtr->right;
			right->up=ndPtr->up;
			if(ndPtr->up==nullptr)
			{
				root=right;
			}
			else if(ndPtr->up->left==ndPtr)
			{
				ndPtr->up->left=right;
			}
			else // if(ndPtr->up->right==ndPtr)
			{
				ndPtr->up->right=right;
			}
			return true;
		}
		else if(nullptr==ndPtr->right)
		{
			auto left=ndPtr->left;
			left->up=ndPtr->up;
			if(ndPtr->up==nullptr)
			{
				root=left;
			}
			else if(ndPtr->up->left==ndPtr)
			{
				ndPtr->up->left=left;
			}
			else // if(ndPtr->up->right==ndPtr)
			{
				ndPtr->up->right=left;
			}
			return true;
		}
		return false;
	}

public:
	bool Delete(NodeHandle ndHd)
	{
		if(ndHd.IsNull())
		{
			return false;
		}

		auto ndPtr=GetNode(ndHd);
		// First try a simple method
		if(true==SimpleDetach(ndHd))
		{
			delete ndPtr;
			return true;
		}
		// Then try non-simple situation
		else
		{
			auto RMOLhd=Left(ndHd);
			while(true==Right(RMOLhd).IsNotNull())
			{
				RMOLhd=Right(RMOLhd);
			}
			auto RMOLptr=GetNode(RMOLhd);

			auto rightSubPtr=GetNode(Right(ndHd));

			if(true!=SimpleDetach(RMOLhd))
			{
				printf("This SimpleDetach is supposed to succeed.\n");
			}

			auto leftSubPtr=GetNode(Left(ndHd));

			RMOLptr->up=ndPtr->up;
			if(nullptr==ndPtr->up)
			{
				root=RMOLptr;
			}
			else if(ndPtr->up->left==ndPtr)
			{
				ndPtr->up->left=RMOLptr;
			}
			else // if(ndPtr->up->right==ndPtr)
			{
				ndPtr->up->right=RMOLptr;
			}

			RMOLptr->right=rightSubPtr;
			if(nullptr!=rightSubPtr)
			{
				rightSubPtr->up=RMOLptr;
			}
			else
			{
				printf("Right Sub-Tree is not supposed to be empty.\n");
			}

			RMOLptr->left=leftSubPtr;
			if(nullptr!=leftSubPtr)
			{
				leftSubPtr->up=RMOLptr;
			}

			delete ndPtr;  // <- Was missing.  Thank you for catching the error!

			return true;
		}
	}

protected:
	NodeHandle FirstNode(Node *ptr) const
	{
		while(nullptr!=ptr && nullptr!=ptr->left)
		{
			ptr=ptr->left;
		}
		return MakeHandle(ptr);
	}

public:
	NodeHandle First(void) const
	{
		return FirstNode(root);
	}
	NodeHandle FindNext(NodeHandle ndHd) const
	{
		auto ptr=GetNode(ndHd);
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
					return Null();
				}
				else if(up->left==ptr)
				{
					return MakeHandle(up);
				}
				ptr=up;
			}
		}
		return Null();
	}
};


/* } */
#endif
