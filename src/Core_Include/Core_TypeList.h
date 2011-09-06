#ifndef _TYPE_LIST_H_
#define _TYPE_LIST_H_

class NullType
{
};

template <class HEAD_TYPE, class TAIL_TYPE>
struct TypeList
{
	typedef HEAD_TYPE Head;
	typedef TAIL_TYPE Tail;
};


namespace TypeListOperations
{

	template <class TYPE_LIST> struct Length;
	template <> struct Length<NullType>
	{
		enum
		{
			value = 0
		};
	};
	template <class T, class U>
	struct Length< TypeList<T, U> >
	{
		enum
		{
			value = 1 + Length<U>::value
		};
	};

	template <class TList, unsigned int index> struct TypeAt;
	template <class Head, class Tail>
	struct TypeAt<TypeList<Head, Tail>, 0>
	{
	   typedef Head Result;
	};
	template <class Head, class Tail, unsigned int i>
	struct TypeAt<TypeList<Head, Tail>, i>
	{
	   typedef typename TypeAt<Tail, i - 1>::Result Result;
	};


	#define TYPELIST_1(T1) TypeList<T1, NullType>
	#define TYPELIST_2(T1, T2) TypeList<T1, TYPELIST_1(T2) >
	#define TYPELIST_3(T1, T2, T3) TypeList<T1, TYPELIST_2(T2, T3) >
	#define TYPELIST_4(T1, T2, T3, T4) TypeList<T1, TYPELIST_3(T2, T3, T4) >
	#define TYPELIST_5(T1, T2, T3, T4, T5) TypeList<T1, TYPELIST_4(T2, T3, T4, T5) >
	#define TYPELIST_6(T1, T2, T3, T4, T5, T6) TypeList<T1, TYPELIST_5(T2, T3, T4, T5, T6) >
	#define TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) TypeList<T1, TYPELIST_6(T2, T3, T4, T5, T6, T7) >

	#define TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
		TypeList<T1, TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >
	#define TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
		TypeList<T1, TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >
	#define TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
		TypeList<T1, TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >
	#define TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
		TypeList<T1, TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >
	#define TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
		TypeList<T1, TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) >
	#define TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
		TypeList<T1, TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) >
	#define TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) \
		TypeList<T1, TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14) >
	#define TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) \
		TypeList<T1, TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15) >
	#define TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) \
		TypeList<T1, TYPELIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16) >

}


#endif