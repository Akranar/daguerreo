#ifndef _UNIFORM_BUFFER_H_
#define _UNIFORM_BUFFER_H_

#include <cstring>

class UniformBuffer
{
public:
	unsigned int byte_count;
	unsigned int index;
	unsigned int domain;
	void * PRIVATE;
};

#endif