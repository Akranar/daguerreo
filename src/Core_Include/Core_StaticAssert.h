#ifndef _STATIC_ASSERT_H_
#define _STATIC_ASSERT_H_

template<bool> struct CompileTimeChecker
{
   CompileTimeChecker(...);
};
template<> struct CompileTimeChecker<false> { };
#define STATIC_ASSERT(expr, msg) \
   {\
       class ERROR_##msg {}; \
       (void)sizeof(CompileTimeChecker<(expr) != 0>((ERROR_##msg())));\
   }



#endif