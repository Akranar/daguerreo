#include "../RendererBase.h"
#include "RendererBasePimpl.h"
#include "../DescVertexFormat.h"
#include <Core_Assert.h>

#include "../Interface_ShaderProgram.h"

void RendererBase::SetInputLayout(unsigned int vertex_format_hash, unsigned int shader_signature_hash)
{
	std::map<unsigned int, IDirect3DVertexDeclaration9*>::iterator result = pimpl->input_layouts.find(vertex_format_hash);
	Assert(result != pimpl->input_layouts.end(), "Vertex Format Hash Not Found.");
	pimpl->d3d_device->SetVertexDeclaration(result->second);
	
}
void RendererBase::CreateInputLayout(unsigned int vertex_format_hash, ShaderProgram * program)
{
	/*
		std::string name;
		unsigned int offset;
		unsigned int byte_stride;
		unsigned char stream_index;
		unsigned char type;
		unsigned char channels;
	*/
	const DescVertexFormat & desc_vf = *hash_to_format[vertex_format_hash];
	DescShaderSignature & desc_ss = *program->signature_desc;
	unsigned int shader_signature_hash = program->signature_hash;
	unsigned int input_count = desc_vf.GetAttributeCount();
	D3DVERTEXELEMENT9 vertex_element[20];

	unsigned int i = 0;
	for (; i < input_count; ++i)
	{
		pimpl->InputLayoutHelper_NameToUsage(desc_vf[i].GetName(), vertex_element[i].Usage, vertex_element[i].UsageIndex);
		vertex_element[i].Method = D3DDECLMETHOD_DEFAULT;
		switch (desc_vf[i].GetType())
		{
		case DT_FLOAT:
			{
				switch (desc_vf[i].GetChannels())
				{
				case 1:
					vertex_element[i].Type = D3DDECLTYPE_FLOAT1;
					break;
				case 2:
					vertex_element[i].Type = D3DDECLTYPE_FLOAT2;
					break;
				case 3:
					vertex_element[i].Type = D3DDECLTYPE_FLOAT3;
					break;
				case 4:
					vertex_element[i].Type = D3DDECLTYPE_FLOAT4;
					break;
				}
			}
			break;
		}
		vertex_element[i].Offset = desc_vf[i].GetOffset();
		vertex_element[i].Stream = desc_vf[i].GetStreamIndex();
	}
	D3DVERTEXELEMENT9 end = D3DDECL_END();
	vertex_element[i] = end;

	std::pair<std::map<unsigned int, IDirect3DVertexDeclaration9 *>::iterator, bool> insert_result = pimpl->input_layouts.insert(std::make_pair(vertex_format_hash, (IDirect3DVertexDeclaration9 *) NULL));
	if (insert_result.second)
	{
		IDirect3DVertexDeclaration9 * vd = 0;
		HRESULT result = pimpl->d3d_device->CreateVertexDeclaration(vertex_element, &vd);
		if (vd)
		{
			insert_result.first->second = vd;
		}
		else
		{
			pimpl->input_layouts.erase(insert_result.first);
		}
	}
	
}

unsigned int RendererBase::GetVertexFormatHash(DescVertexFormat & desc_vf)
{
	std::pair<std::map<DescVertexFormat, unsigned int, CompareVertexFormat>::iterator, bool> result = vf_hasher.insert(std::make_pair(desc_vf, next_vf_hash));
	if (result.second) //successfully added.
	{
		hash_to_format.push_back(&result.first->first);
		next_vf_hash += 1;
	}
	return result.first->second;
}

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

void RendererBase::DestroyHashers()
{
	vf_hasher.clear();
	ss_hasher.clear();

	for (std::map<unsigned int, IDirect3DVertexDeclaration9 *>::iterator it = pimpl->input_layouts.begin(); it != pimpl->input_layouts.end(); ++it)
	{
		it->second->Release();
	}
	pimpl->input_layouts.clear();
}