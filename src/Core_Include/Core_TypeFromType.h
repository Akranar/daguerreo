#ifndef _TYPE_FROM_TYPE_H_
#define _TYPE_FROM_TYPE_H_

template <typename TYPE>
struct TypeFromType
{
   typedef TYPE original_type;
};

#endif