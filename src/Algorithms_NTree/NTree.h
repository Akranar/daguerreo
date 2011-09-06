#ifndef _N_TREE_H_
#define _N_TREE_H_

#include "ExLib_MetaProgramming.h"


class NTree
{
public:
	virtual ~NTree() {};
	virtual inline NTree * AttachChildAt(unsigned int index, NTree * child) = 0;
	virtual inline int AttachChild(NTree * child) = 0;
	virtual inline NTree * DetachChildAt(unsigned int index) = 0;
	virtual inline NTree * DetachChild(NTree * child) = 0;
	virtual inline void DetachAll() = 0;
	virtual inline NTree * GetChildAt(unsigned int index) const = 0;
	virtual inline unsigned int GetOrder() const = 0;
};


/*
template <class TYPE>
class NTree
{
public:
	virtual inline TYPE * AttachChildAt(unsigned int index, TYPE * child) = 0;
	virtual inline int AttachChild(TYPE * child) = 0;
	virtual inline TYPE * DetachChildAt(unsigned int index) = 0;
	virtual inline TYPE * DetachChild(TYPE * child) = 0;
	virtual inline TYPE * GetChildAt(unsigned int index) const = 0;
	virtual inline unsigned int GetOrder() const = 0;
};
*/


#endif