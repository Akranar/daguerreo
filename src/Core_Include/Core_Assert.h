#ifndef _ASSERT_H
#define _ASSERT_H

/*
	Custom Assert:
		-> Defined if DO_ASSERT is defined.
		-> evaluates "exp"
			-> if "exp" false,
				-> Evaluates MyAssert
					-> Returns true to break into debugger
						-> the line: _asm int 3;
							-> Causes PC to break into debugger
							-> Ignored if no debugger.
					-> False and program continues.
*/

#include <iostream>

bool MyAssert(bool value, const char * text, const char * file, int line);

#ifdef _DEBUG
#define DO_ASSERT
#endif

#ifdef DO_ASSERT
#define Assert(exp, text) \
	if (!(exp) && MyAssert(exp, text,__FILE__, __LINE__)) \
		_asm int 3;
#else
#define Assert(exp, text)
#endif



#endif
