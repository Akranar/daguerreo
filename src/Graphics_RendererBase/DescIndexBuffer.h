#ifndef _DESC_INDEX_BUFFER_H_
#define _DESC_INDEX_BUFFER_H_

#include "ExLib_Serialization.h"

class DescIndexBuffer : public Serializable
{
public:
	unsigned int Usage;
	unsigned int ByteCount;
	unsigned int PrimitiveType;
	void * InitialData;

	inline unsigned int GetUsage() const;
	inline unsigned int GetByteCount() const;
	inline unsigned int GetPrimitiveType() const;
	inline void * GetInitialDataPointer() const;
	
	inline void SetUsage(unsigned int _usage);
	inline void SetByteCount(unsigned int _count);
	inline void SetPrimitiveType(unsigned int _stride);
	inline void SetInitialDataPointer(void * init_data);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline unsigned int DescIndexBuffer::GetUsage() const
{
	return Usage;
}
inline unsigned int DescIndexBuffer::GetByteCount() const
{
	return ByteCount;
}
inline unsigned int DescIndexBuffer::GetPrimitiveType() const
{
	return PrimitiveType;
}
inline void * DescIndexBuffer::GetInitialDataPointer() const
{
	return InitialData;
}

inline void DescIndexBuffer::SetUsage(unsigned int _usage)
{
	Usage = _usage;
}
inline void DescIndexBuffer::SetByteCount(unsigned int _count)
{
	ByteCount = _count;
}
inline void DescIndexBuffer::SetPrimitiveType(unsigned int _type)
{
	PrimitiveType = _type;
}
inline void DescIndexBuffer::SetInitialDataPointer(void * init_data)
{
	InitialData = init_data;
}

#endif