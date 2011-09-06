#include "CompareShaderSignature.h"
#include "DescShaderSignature.h"

/*
class DescShaderSignatureEntry
{
public:
	std::string name;
	unsigned char type;
	unsigned char channels;
	unsigned char usage_index;
};

class DescShaderSignature
{
public:
	FixedArray<ShaderSignatureEntry> signature_entries;
};
*/

bool CompareShaderSignature::operator() (const DescShaderSignature & a, const DescShaderSignature & b)
{
	unsigned int a_length = a.signature_entries.GetSize();
	unsigned int b_length = b.signature_entries.GetSize();
	if (a_length < b_length)
	{
		return true;
	}
	else if (a_length > b_length)
	{
		return false;
	}

	for (unsigned int i = 0; i < a_length; ++i)
	{
		const DescShaderSignatureEntry & entry_a = a.signature_entries[i];
		const DescShaderSignatureEntry & entry_b = b.signature_entries[i];

		int name_compare = entry_a.name.compare(entry_b.name);
		if (name_compare < 0)
		{
			return true;
		}
		if (name_compare > 0)
		{
			return false;
		}
		if (entry_a.type < entry_b.type)
		{
			return true;
		}
		if (entry_a.type > entry_b.type)
		{
			return false;
		}
		if (entry_a.channels < entry_b.channels)
		{
			return true;
		}
		if (entry_a.channels > entry_b.channels)
		{
			return false;
		}
		if (entry_a.usage_index < entry_b.usage_index)
		{
			return true;
		}
		if (entry_a.usage_index > entry_b.usage_index)
		{
			return false;
		}
	}
	return false;
}