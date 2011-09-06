#ifndef _TYPE_SELECT_H_
#define _TYPE_SELECT_H_

template <bool flag, typename T, typename U>
struct TypeSelect
{
   typedef T Result;
};
template <typename T, typename U>
struct TypeSelect<false, T, U>
{
   typedef U Result;
};

#endif