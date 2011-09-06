#include "../RendererBase.h"
#include "RendererBasePimpl.h"
#include "../DescVertexFormat.h"
#include <Core_Assert.h>

#include "OglInputLayout.h"
#include "../Interface_ShaderProgram.h"

#include <Core_FixedArray.h>
#include <Core_Exception.h>

void RendererBase::SetInputLayout(unsigned int vertex_format_hash, unsigned int shader_signature_hash)
{
	
	std::map<unsigned int, std::map<unsigned int, OglInputLayout> >::iterator result0 = pimpl->input_layouts.find(shader_signature_hash);
	Assert(result0 != pimpl->input_layouts.end(), "Shader Signature Hash Not Found.");
	std::map<unsigned int, OglInputLayout>::iterator result1 = result0->second.find(vertex_format_hash);
	Assert(result1 != result0->second.end(), "Vertex Format Hash Not Found.");

	if (pimpl->bound_layout)
	{
		pimpl->bound_layout->End();
	}

	pimpl->bound_layout = &result1->second;

	if (pimpl->bound_vertex_buffer)
	{
		pimpl->bound_layout->Begin();
	}
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

	OglInputLayout layout;
	layout.attribs.Set(desc_ss.signature_entries.GetSize());

	unsigned int match_count = 0;

	for (unsigned int i = 0; i < desc_ss.signature_entries.GetSize(); ++i)
	{
		for (unsigned int j = 0; j < input_count; ++j)
		{
			if (desc_vf[j].GetSemanticIndex() == desc_ss.signature_entries[i].usage_index)
			{
				int name_compare = desc_ss.signature_entries[i].name.compare(desc_vf.format_entries[j].name);
				if (name_compare == 0)
				{
					const DescVertexFormatEntry & current_format_entry = desc_vf[j];
					layout.attribs[i].channels = current_format_entry.GetChannels();
					layout.attribs[i].offset = current_format_entry.GetOffset();
					layout.attribs[i].stride = current_format_entry.GetStride();
					layout.attribs[i].type = current_format_entry.GetType();
					match_count += 1;
					break;
				}
			}
		}
	}
	CONDITIONAL_EXCEPTION(match_count != desc_ss.signature_entries.GetSize(), "VertexFormat does not provide all attributes needed in shader.");

	std::pair<std::map<unsigned int, std::map<unsigned int, OglInputLayout> >::iterator, bool> insert_result =
		pimpl->input_layouts.insert(std::make_pair(shader_signature_hash, std::map<unsigned int, OglInputLayout>()));

	
	std::pair<std::map<unsigned int, OglInputLayout>::iterator, bool> insert_result1 = insert_result.first->second.insert(std::make_pair(vertex_format_hash, layout));
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
	
	for (std::map<unsigned int, std::map<unsigned int, OglInputLayout> >::iterator it = pimpl->input_layouts.begin(); it != pimpl->input_layouts.end(); ++it)
	{
		it->second.clear();
	}
	pimpl->input_layouts.clear();
	
}