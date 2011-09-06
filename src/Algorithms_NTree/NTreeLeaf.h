#ifndef _NTREE_LEAF_H_
#define _NTREE_LEAF_H_

#include "NTree.h"

template <typename BASE = NTree, class CHILDREN = BASE>
class NTreeLeaf: public BASE
{
public:
	virtual inline CHILDREN * AttachChildAt(unsigned int index, CHILDREN * child)
	{
		return 0;
	}
	virtual inline int AttachChild(CHILDREN * child)
	{
		return -1;
	}
	virtual inline CHILDREN * DetachChildAt(unsigned int index)
	{
		return 0;
	}
	virtual inline CHILDREN * DetachChild(CHILDREN * child)
	{
		return 0;
	}
	virtual inline CHILDREN * GetChildAt(unsigned int index) const
	{
		return 0;
	}
	virtual inline unsigned int GetOrder() const
	{
		return 0;
	}
};

#endif