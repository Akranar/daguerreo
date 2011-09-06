#ifndef _INTERFACE_INDEX_BUFFER_H_
#define _INTERFACE_INDEX_BUFFER_H_

class IndexBuffer
{
public:
	unsigned int byte_count;
	unsigned int offset;
	unsigned int usage;
	unsigned int primitive_type;
	void * PRIVATE;
	inline IndexBuffer() : PRIVATE(0) {};
};




#endif