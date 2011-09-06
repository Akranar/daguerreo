#ifndef _COMPARE_SHADER_SIGNATURE_H_
#define _COMPARE_SHADER_SIGNATURE_H_

class DescShaderSignature;

class CompareShaderSignature
{
public:
	bool operator() (const DescShaderSignature & a, const DescShaderSignature & b);
};

#endif