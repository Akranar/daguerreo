#ifndef _HAS_SET_PARENT_H_
#define _HAS_SET_PARENT_H_

//From: http://stackoverflow.com/questions/257288/is-it-possible-to-write-a-c-template-to-check-for-a-functions-existence


//Doesn't work for inherited SetParent();
template <class Type>
class TypeHasSetParent
{
    template <typename T, T> struct TypeCheck;

    typedef char Yes;
    typedef long No;

    template <typename T> struct SetParent
    {
        typedef void (T::*fptr)(T *);
    };

    template <typename T> static Yes HasSetParent(TypeCheck< typename SetParent<T>::fptr, &T::SetParent >*);
    template <typename T> static No  HasSetParent(...);

public:
    static bool const value = (sizeof(HasSetParent<Type>(0)) == sizeof(Yes));
};


#endif