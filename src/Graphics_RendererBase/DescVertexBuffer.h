#ifndef _DESC_VERTEX_BUFFER_H_
#define _DESC_VERTEX_BUFFER_H_

#include "ExLib_Serialization.h"

class DescVertexBuffer : public Serializable
{
public:
	unsigned int Usage;
	unsigned int ByteCount;
	unsigned int ByteStride;
	void * InitialData;

	inline unsigned int GetUsage() const;
	inline unsigned int GetByteCount() const;
	inline unsigned int GetStride() const;
	inline void * GetInitialDataPointer() const;
	
	inline void SetUsage(unsigned int _usage);
	inline void SetByteCount(unsigned int _count);
	inline void SetStride(unsigned int _stride);
	inline void SetInitialDataPointer(void * init_data);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline unsigned int DescVertexBuffer::GetUsage() const
{
	return Usage;
}
inline unsigned int DescVertexBuffer::GetByteCount() const
{
	return ByteCount;
}
inline unsigned int DescVertexBuffer::GetStride() const
{
	return ByteStride;
}
inline void * DescVertexBuffer::GetInitialDataPointer() const
{
	return InitialData;
}

inline void DescVertexBuffer::SetUsage(unsigned int _usage)
{
	Usage = _usage;
}
inline void DescVertexBuffer::SetByteCount(unsigned int _count)
{
	ByteCount = _count;
}
inline void DescVertexBuffer::SetStride(unsigned int _stride)
{
	ByteStride = _stride;
}
inline void DescVertexBuffer::SetInitialDataPointer(void * init_data)
{
	InitialData = init_data;
}


#endif