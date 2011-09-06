#ifndef _TYPE_FROM_INT_H_
#define _TYPE_FROM_INT_H_

template <int INT_VALUE>
struct TypeFromInt
{
   enum { value = INT_VALUE };
};

#endif