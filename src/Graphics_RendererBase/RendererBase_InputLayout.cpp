#include "RendererBase.h"

#include "DescVertexFormat.h"

/*
unsigned int RendererBase::GetVertexFormatHash(DescVertexFormat & desc_vf)
{
	std::pair<std::map<DescVertexFormat, unsigned int, CompareVertexFormat>::iterator, bool> result = vf_hasher.insert(std::make_pair(desc_vf, next_vf_hash));
	if (result.second) //successfully added.
	{
		next_vf_hash += 1;
	}
	return result.first->second;
}
//unsigned int RendererBase::GetShaderSignatureHash(DescShaderSignature & desc_ss)
DescShaderSignature & RendererBase::GetShaderSignatureHash(DescShaderSignature & desc_ss, unsigned int & out_hash)
{
	std::pair<std::map<DescShaderSignature, unsigned int, CompareShaderSignature>::iterator, bool> result = ss_hasher.insert(std::make_pair(desc_ss, next_ss_hash));
	if (result.second) //successfully added.
	{
		next_ss_hash += 1;
	}
	out_hash = result.first->second;
	return const_cast<DescShaderSignature &>(result.first->first);
}
*/