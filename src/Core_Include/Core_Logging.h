#ifndef _CORE_LOGGING_H_
#define _CORE_LOGGING_H_

#include <iostream>

class Logger
{
public:

};

#define CONDITIONAL_LOG(exp, out) \
	if (exp) \
	{ \
		std::cout << out << std::endl; \
	}

#endif