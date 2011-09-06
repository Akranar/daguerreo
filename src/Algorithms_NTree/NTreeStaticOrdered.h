#ifndef _N_TREE_STATIC_ORDERED_H_
#define _N_TREE_STATIC_ORDERED_H_

#include "NTree.h"
#include "HasSetParent.h"
#include "ExLib_MetaProgramming.h"


template <unsigned int CHILDREN_COUNT = 2, class BASE = NTree, bool HAS_PARENT = TypeHasSetParent<BASE>::value>
class NTreeStaticOrdered: public BASE
{
	typedef BASE CHILDREN;
protected:
	CHILDREN * children[CHILDREN_COUNT];

public:
	NTreeStaticOrdered()
	{
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			children[i] = 0;
		}
	}
	virtual ~NTreeStaticOrdered()
	{
		/*
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			if (children[i]) delete children[i];
		}
		*/
	}
	virtual inline CHILDREN * AttachChildAt(unsigned int index, CHILDREN * child)
	{
		CHILDREN * rtn = children[index];
		children[index] = child;
		return rtn;
	}
	virtual inline int AttachChild(CHILDREN * child)
	{
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			if (children[i] == 0)
			{
				children[i] = child;
				return i;
			}
		}
		return -1;
	}
	virtual inline CHILDREN * DetachChildAt(unsigned int index)
	{
		CHILDREN * rtn = children[index];
		children[index] = 0;
		return rtn;
	}
	virtual inline CHILDREN * DetachChild(CHILDREN * child)
	{
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			if (children[i] == child)
			{
				CHILDREN * rtn = children[i];
				children[i] = 0;
				return rtn;
			}
		}
		return 0;
	}
	virtual inline void DetachAll()
	{
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			children[i] = 0;
		}
	}
	virtual inline CHILDREN * GetChildAt(unsigned int index) const
	{
		return children[index];
	}
	virtual inline unsigned int GetOrder() const
	{
		return CHILDREN_COUNT;
	}

	template<class CAST_TYPE>
	CAST_TYPE * GetChildAtAs(unsigned int index) const
	{
		return static_cast<CAST_TYPE *>(GetChildAt(index));
	}
};



template <unsigned int CHILDREN_COUNT, class BASE>
class NTreeStaticOrdered<CHILDREN_COUNT, BASE, true>: public BASE
{
	typedef BASE CHILDREN;
protected:
	CHILDREN * children[CHILDREN_COUNT];

public:
	NTreeStaticOrdered()
	{
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			children[i] = 0;
		}
	}
	virtual ~NTreeStaticOrdered()
	{
		/*
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			if (children[i]) delete children[i];
		}
		*/
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
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			if (children[i] == 0)
			{
				children[i] = child;
				child->SetParent(this);
				return i;
			}
		}
		return -1;
	}
	virtual inline CHILDREN * DetachChildAt(unsigned int index)
	{
		CHILDREN * rtn = children[index];
		children[index] = 0;
		rtn->SetParent(0);
		return rtn;
	}
	virtual inline CHILDREN * DetachChild(CHILDREN * child)
	{
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			if (children[i] == child)
			{
				CHILDREN * rtn = children[i];
				children[i] = 0;
				rtn->SetParent(0);
				return rtn;
			}
		}
		return 0;
	}
	virtual inline void DetachAll()
	{
		for (unsigned int i = 0; i < CHILDREN_COUNT; ++i)
		{
			children[i] = 0;
		}
	}
	virtual inline CHILDREN * GetChildAt(unsigned int index) const
	{
		return children[index];
	}
	virtual inline unsigned int GetOrder() const
	{
		return CHILDREN_COUNT;
	}

	template<class CAST_TYPE>
	CAST_TYPE * GetChildAtAs(unsigned int index) const
	{
		return static_cast<CAST_TYPE *>(GetChildAt(index));
	}
};

#endif