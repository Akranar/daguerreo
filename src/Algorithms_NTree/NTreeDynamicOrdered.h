#ifndef _N_TREE_DYNAMIC_ORDERED_H_
#define _N_TREE_DYNAMIC_ORDERED_H_

#include "NTree.h"
#include "HasSetParent.h"
#include "ExLib_MetaProgramming.h"
#include <vector>

template <class BASE = NTree, bool HAS_PARENT = TypeHasSetParent<BASE>::value>
class NTreeDynamicOrdered: public BASE
{
	typedef BASE CHILDREN;
protected:
	std::vector<CHILDREN *> children;
public:
	virtual ~NTreeDynamicOrdered()
	{
		/*
		for (unsigned int i = 0; i < children.size(); ++i)
		{
			delete children[i];
		}
		children.clear();
		*/
	}
	virtual inline CHILDREN * GetChildAt(unsigned int index) const
	{
		return children[index];
	}
	virtual inline unsigned int GetOrder() const
	{
		return children.size();
	}

	template<class CAST_TYPE>
	CAST_TYPE * GetChildAtAs(unsigned int index) const
	{
		return static_cast<CAST_TYPE *>(GetChildAt(index));
	}

	virtual inline CHILDREN * AttachChildAt(unsigned int index, CHILDREN * child)
	{
		CHILDREN * rtn = children[index];
		children[index] = child;
		return rtn;
	}
	virtual inline int AttachChild(CHILDREN * child)
	{
		children.push_back(child);
		return children.size() - 1;
	}
	virtual inline CHILDREN * DetachChildAt(unsigned int index)
	{
		CHILDREN * rtn = children[index];
		children[index] = 0;
		return rtn;
	}
	virtual inline CHILDREN * DetachChild(CHILDREN * child)
	{
		for (unsigned int i = 0; i < children.size(); ++i)
		{
			if (children[i] == child)
			{
				CHILDREN * rtn = children[i];
				if (i == children.size()-1)
				{
					children.pop_back();
				}
				else
				{
					children[i] = 0;
				}
				return rtn;
			}
		}
		return 0;
	}

	virtual inline void DetachAll()
	{
		children.clear();
	}
	
};


template <class BASE>
class NTreeDynamicOrdered<BASE, true>: public BASE
{
	typedef BASE CHILDREN;
protected:
	std::vector<CHILDREN *> children;
public:
	virtual ~NTreeDynamicOrdered()
	{
		/*
		for (unsigned int i = 0; i < children.size(); ++i)
		{
			delete children[i];
		}
		children.clear();
		*/
	}
	virtual inline CHILDREN * GetChildAt(unsigned int index) const
	{
		return children[index];
	}
	virtual inline unsigned int GetOrder() const
	{
		return children.size();
	}

	template<class CAST_TYPE>
	CAST_TYPE * GetChildAtAs(unsigned int index) const
	{
		return static_cast<CAST_TYPE *>(GetChildAt(index));
	}

	virtual inline CHILDREN * AttachChildAt(unsigned int index, CHILDREN * child)
	{
		CHILDREN * rtn = children[index];
		children[index] = child;
		child->SetParent(this);
		return rtn;
	}
	virtual inline int AttachChild(CHILDREN * child)
	{
		children.push_back(child);
		child->SetParent(this);
		return children.size() - 1;
	}
	virtual inline CHILDREN * DetachChildAt(unsigned int index)
	{
		CHILDREN * rtn = children[index];
		rtn->SetParent(0);
		children[index] = 0;
		return rtn;
	}
	virtual inline CHILDREN * DetachChild(CHILDREN * child)
	{
		for (unsigned int i = 0; i < children.size(); ++i)
		{
			if (children[i] == child)
			{
				CHILDREN * rtn = children[i];
				if (i == children.size()-1)
				{
					children.pop_back();
				}
				else
				{
					children[i] = 0;
				}
				child->SetParent(0);
				return rtn;
			}
		}
		return 0;
	}

	virtual inline void DetachAll()
	{
		children.clear();
	}
	
};


#endif