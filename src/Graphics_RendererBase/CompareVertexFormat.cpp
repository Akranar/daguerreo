#include "CompareVertexFormat.h"
#include "DescVertexFormat.h"
/*
class DescVertexFormatEntry
{
public:
	std::string name;
	unsigned int offset;
	unsigned int byte_stride;
	unsigned char stream_index;
	unsigned char type;
};

class DescVertexFormat
{
public:
	FixedArray<DescVertexFormatEntry> format_entries;
};
*/

bool CompareVertexFormat::operator() (const DescVertexFormat & a, const DescVertexFormat & b)
{
	unsigned int a_length = a.format_entries.GetSize();
	unsigned int b_length = b.format_entries.GetSize();
	if (a_length < b_length)
	{
		return true;
	}
	else if (b_length > a_length)
	{
		return false;
	}

	for (unsigned int i = 0; i < a_length; ++i)
	{
		const DescVertexFormatEntry & entry_a = a.format_entries[i];
		const DescVertexFormatEntry & entry_b = b.format_entries[i];

		int name_compare = entry_a.GetName().compare(entry_b.GetName());
		if (name_compare < 0)
		{
			return true;
		}
		if (name_compare > 0)
		{
			return false;
		}
		if (entry_a.GetSemanticIndex() < entry_b.GetSemanticIndex())
		{
			return true;
		}
		if (entry_a.GetSemanticIndex() > entry_b.GetSemanticIndex())
		{
			return false;
		}
		if (entry_a.GetChannels() < entry_b.GetChannels())
		{
			return true;
		}
		if (entry_a.GetChannels() > entry_b.GetChannels())
		{
			return false;
		}
		if (entry_a.GetStride() < entry_b.GetStride())
		{
			return true;
		}
		if (entry_a.GetStride() > entry_b.GetStride())
		{
			return false;
		}
		if (entry_a.GetType() < entry_b.GetType() )
		{
			return true;
		}
		if (entry_a.GetType()  > entry_b.GetType() )
		{
			return false;
		}
		if (entry_a.GetOffset() < entry_b.GetOffset())
		{
			return true;
		}
		if (entry_a.GetOffset() > entry_b.GetOffset())
		{
			return false;
		}
		if (entry_a.GetStreamIndex() < entry_b.GetStreamIndex())
		{
			return true;
		}
		if (entry_a.GetStreamIndex() > entry_b.GetStreamIndex())
		{
			return false;
		}
	}
	return false;
}