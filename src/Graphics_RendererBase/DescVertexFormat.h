#ifndef _DESC_VERTEX_FORMAT_H_
#define _DESC_VERTEX_FORMAT_H_

#include <Core_FixedArray.h>
#include <string>
#include "ExLib_Serialization.h"

class DescVertexFormatEntry : public Serializable
{
public:
	std::string name;
	unsigned int semantic_index;
	unsigned int offset;
	unsigned int byte_stride;
	unsigned char stream_index;
	unsigned char type;
	unsigned char channels;

	inline const std::string & GetName() const;
	inline unsigned int GetSemanticIndex() const;
	inline unsigned int GetOffset() const;
	inline unsigned int GetStride() const;
	inline unsigned int GetStreamIndex() const;
	inline unsigned int GetType() const;
	inline unsigned int GetChannels() const;

	inline void SetName(const std::string & in);
	inline void SetSemanticIndex(unsigned int in);
	inline void SetOffset(unsigned int in);
	inline void SetStride(unsigned int in);
	inline void SetStreamIndex(unsigned int in);
	inline void SetType(unsigned int in);
	inline void SetChannels(unsigned int in);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline const std::string & DescVertexFormatEntry::GetName() const
{
	return name;
}
inline unsigned int DescVertexFormatEntry::GetSemanticIndex() const
{
	return semantic_index;
}
inline unsigned int DescVertexFormatEntry::GetOffset() const
{
	return offset;
}
inline unsigned int DescVertexFormatEntry::GetStride() const
{
	return byte_stride;
}
inline unsigned int DescVertexFormatEntry::GetStreamIndex() const
{
	return stream_index;
}
inline unsigned int DescVertexFormatEntry::GetType() const
{
	return type;
}
inline unsigned int DescVertexFormatEntry::GetChannels() const
{
	return channels;
}

inline void DescVertexFormatEntry::SetName(const std::string & in)
{
	name.assign(in);
}
inline void DescVertexFormatEntry::SetSemanticIndex(unsigned int in)
{
	semantic_index = in;
}
inline void DescVertexFormatEntry::SetOffset(unsigned int in)
{
	offset = in;
}
inline void DescVertexFormatEntry::SetStride(unsigned int in)
{
	byte_stride = in;
}
inline void DescVertexFormatEntry::SetStreamIndex(unsigned int in)
{
	stream_index = in;
}
inline void DescVertexFormatEntry::SetType(unsigned int in)
{
	type = in;
}
inline void DescVertexFormatEntry::SetChannels(unsigned int in)
{
	channels = in;
}

class DescVertexFormat : public Serializable
{
public:
	FixedArray<DescVertexFormatEntry> format_entries;

	inline void Initialize(unsigned int attrib_count);
	inline const DescVertexFormatEntry & operator[] (unsigned int index) const;
	inline DescVertexFormatEntry & operator[] (unsigned int index);
	inline unsigned int GetAttributeCount() const;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline void DescVertexFormat::Initialize(unsigned int attrib_count)
{
	format_entries.Set(attrib_count);
}
inline const DescVertexFormatEntry & DescVertexFormat::operator[] (unsigned int index) const
{
	return format_entries[index];
}
inline DescVertexFormatEntry & DescVertexFormat::operator[] (unsigned int index)
{
	return format_entries[index];
}
inline unsigned int DescVertexFormat::GetAttributeCount() const
{
	return format_entries.GetSize();
}


#endif