#ifndef _N_TREE_WITH_PARENT_H_
#define _N_TREE_WITH_PARENT_H_

#include "NTree.h"
#include <Core_TypeEmpty.h>

/*
template <class NTREE_BASE, class PARENT_TYPE = NTree>
class NTreeWithParent: public NTREE_BASE
{
protected:
	PARENT_TYPE * parent;
public:
	inline PARENT_TYPE * GetParent() const
	{
		return parent;
	}
	inline PARENT_TYPE * SetParent(PARENT_TYPE * in_parent)
	{
		PARENT_TYPE * rtn = parent;
		parent = in_parent;
		return rtn;
	}
};
*/

template <class NTREE_BASE = TypeEmpty, class PARENT_TYPE = TypeEmpty>
class NTreeWithParent: public NTREE_BASE
{
protected:
	PARENT_TYPE * parent;
public:
	NTreeWithParent() : parent(0) {};
	virtual ~NTreeWithParent() {};
	inline PARENT_TYPE * GetParent() const
	{
		return parent;
	}
	inline void SetParent(PARENT_TYPE * in_parent)
	{
		parent = in_parent;
	}
	
	template<class CAST_TYPE>
	CAST_TYPE * GetParentAs() const
	{
		return static_cast<CAST_TYPE *>(GetParent());
	}
};

template <class NTREE_BASE>
class NTreeWithParent<NTREE_BASE, TypeEmpty>: public NTREE_BASE
{
protected:
	NTreeWithParent<NTREE_BASE> * parent;
public:
	NTreeWithParent() : parent(0) {};
	inline NTreeWithParent<NTREE_BASE> * GetParent() const
	{
		return parent;
	}
	inline void SetParent(NTreeWithParent<NTREE_BASE> * in_parent)
	{
		parent = in_parent;
	}

	template<class CAST_TYPE>
	CAST_TYPE * GetParentAs() const
	{
		return static_cast<CAST_TYPE *>(GetParent());
	}
};

#endif