#include "../RendererBase.h"
#include "RendererBasePimpl.h"
#include "../DescVertexFormat.h"
#include <Core_Assert.h>
#include <Core_Exception.h>

#include "../Interface_ShaderProgram.h"

void RendererBase::SetInputLayout(unsigned int vertex_format_hash, unsigned int shader_signature_hash)
{
	std::map<unsigned int, std::map<unsigned int, ID3D10InputLayout*> >::iterator result0 = pimpl->input_layouts.find(shader_signature_hash);
	Assert(result0 != pimpl->input_layouts.end(), "Shader Signature Hash Not Found.");
	std::map<unsigned int, ID3D10InputLayout*>::iterator result1 = result0->second.find(vertex_format_hash);
	Assert(result1 != result0->second.end(), "Vertex Format Hash Not Found.");
	pimpl->d3d_device->IASetInputLayout(result1->second);
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

	D3D10_INPUT_ELEMENT_DESC elements[20];
	
	unsigned int input_count = desc_vf.GetAttributeCount();

	for (unsigned int i = 0; i < input_count; ++i)
	{
		elements[i].SemanticName = desc_vf[i].GetName().c_str();
		elements[i].SemanticIndex = desc_vf[i].GetSemanticIndex();
		switch (desc_vf.format_entries[i].channels)
		{
		case 1:
			elements[i].Format = DXGI_FORMAT_R32_FLOAT;
			break;
		case 2:
			elements[i].Format = DXGI_FORMAT_R32G32_FLOAT;
			break;
		case 3:
			elements[i].Format = DXGI_FORMAT_R32G32B32_FLOAT;
			break;
		case 4:
			elements[i].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
			break;
		}
		elements[i].AlignedByteOffset = desc_vf[i].GetOffset();
		elements[i].InputSlot = desc_vf[i].GetStreamIndex();
		elements[i].InputSlotClass = D3D10_INPUT_PER_VERTEX_DATA;
		elements[i].InstanceDataStepRate = 0;
	}


	ID3D10Blob * vertex_sig = (ID3D10Blob *) desc_ss.PRIVATE;

	std::pair<std::map<unsigned int, std::map<unsigned int, ID3D10InputLayout *> >::iterator, bool> insert_result =
		pimpl->input_layouts.insert(std::make_pair(shader_signature_hash, std::map<unsigned int, ID3D10InputLayout *>()));

	std::pair<std::map<unsigned int, ID3D10InputLayout *>::iterator, bool> insert_result1 = insert_result.first->second.insert(std::make_pair(vertex_format_hash, (ID3D10InputLayout *) NULL));
	if (insert_result1.second)
	{
		HRESULT input_layout_creation = pimpl->d3d_device->CreateInputLayout(elements, input_count, vertex_sig->GetBufferPointer(), vertex_sig->GetBufferSize(), &insert_result1.first->second);
		CONDITIONAL_EXCEPTION(FAILED(input_layout_creation), "InputLayout Creation Failed");
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

void RendererBase::DestroyHashers()
{
	vf_hasher.clear();

	for (
		std::map<DescShaderSignature, unsigned int, CompareShaderSignature>::iterator it = ss_hasher.begin();
		it != ss_hasher.end();
		++it)
		{
			if (it->first.PRIVATE)
			{
				((ID3D10Blob *) it->first.PRIVATE)->Release();
			}
		}
	ss_hasher.clear();


	for (std::map<unsigned int, std::map<unsigned int, ID3D10InputLayout *> >::iterator it = pimpl->input_layouts.begin(); it != pimpl->input_layouts.end(); ++it)
	{
		for (std::map<unsigned int, ID3D10InputLayout *>::iterator it1 = it->second.begin(); it1 != it->second.end(); ++it1)
		{
			it1->second->Release();
		}
		it->second.clear();
	}
	pimpl->input_layouts.clear();
}