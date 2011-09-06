#ifndef _DESC_SHADER_SIGNATURE_H_
#define _DESC_SHADER_SIGNATURE_H_

#include <Core_FixedArray.h>
#include <string>


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
	FixedArray<DescShaderSignatureEntry> signature_entries;
	void * PRIVATE;

	DescShaderSignature() : PRIVATE(0) {};
};

#endif