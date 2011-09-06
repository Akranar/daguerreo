#ifndef _DESC_UNIFORM_BUFFER_H_
#define _DESC_UNIFORM_BUFFER_H_

#include "ExLib_Serialization.h"

class DescUniformBuffer
{
public:
	unsigned int byte_count;
	unsigned int index;
	unsigned int domain;
};

#endif