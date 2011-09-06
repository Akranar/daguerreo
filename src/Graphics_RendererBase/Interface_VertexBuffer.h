#ifndef _INTERFACE_VERTEX_BUFFER_H_
#define _INTERFACE_VERTEX_BUFFER_H_

class VertexBuffer
{
public:
	unsigned int byte_count;
	unsigned int byte_stride;
	unsigned int offset;
	unsigned int usage;
	void * PRIVATE;
	unsigned int format_hash;
	inline VertexBuffer() : PRIVATE(0), format_hash(-1) {};
};


#endif