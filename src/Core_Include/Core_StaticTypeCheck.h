#ifndef _CONVERSION_EXISTS_H_
#define _CONVERSION_EXISTS_H_

template <class T, class U>
class ConversionExists
{
   typedef char Small; //sizeof(Small) is 1.
   class Big { char dummy[2]; }; //sizeof(Big) > 1.
   static Small Test(U);
   static Big Test(...); //If this gets matched by Test(MakeT()), No conversion since sizeof(Small) != sizeof(Big)
   static T MakeT();
public:
   enum { Result = 
	   sizeof(Test(MakeT())) == sizeof(Small) };
};


template <class T, class U>
class SameType
{
public:
	enum { Result = false };
};

template <class T>
class SameType<T, T>
{
public:
	enum { Result = true };
};


#define SUPERSUBCLASS(T, U) \
   (ConversionExists<const U*, const T*>::Result && \
   !SameType<const T*, const void*>::Result)

#define SUPERSUBCLASS_STRICT(T, U) \
   (SUPERSUBCLASS(T, U) && \
   !SameType<const T, const U>::Result)


#endif