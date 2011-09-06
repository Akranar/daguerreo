/*
#include "BCR_VisualFXInstance.h"

BCR_VisualFXInstance::BCR_VisualFXInstance()
{

}
BCR_VisualFXInstance::~BCR_VisualFXInstance()
{

}

void BCR_VisualFXInstance::SetPassCount(unsigned int in_pass_count)
{
	passes.clear();
	while (passes.size() < in_pass_count)
	{
		passes.push_back(BCR_VisualFXInstance::Pass());
	}
}
void BCR_VisualFXInstance::SetDomainCount(unsigned int pass_index, unsigned int domain_count)
{
	passes[pass_index].domains.clear();
	while (passes[pass_index].domains.size() < domain_count)
	{
		passes[pass_index].domains.push_back(BCR_VisualFXInstance::Domain());
	}
}
void BCR_VisualFXInstance::SetTextureCount(unsigned int pass_index, unsigned int domain_index, unsigned int texture_count)
{
	passes[pass_index].domains[domain_index].texture_files.clear();
	while (passes[pass_index].domains[domain_index].texture_files.size() < texture_count)
	{
		passes[pass_index].domains[domain_index].texture_files.push_back(BCR_VisualFXInstance::TextureFile());
	}
}
void BCR_VisualFXInstance::SetParamCount(unsigned int pass_index, unsigned int domain_index, unsigned int param_count)
{
	passes[pass_index].domains[domain_index].uniform_params.clear();
	while (passes[pass_index].domains[domain_index].uniform_params.size() < param_count)
	{
		passes[pass_index].domains[domain_index].uniform_params.push_back(BCR_VisualFXInstance::UniformParam());
	}
}
void BCR_VisualFXInstance::SetTextureFile(unsigned int pass_index, unsigned domain_index, unsigned int texture_index, const std::string & in_filename)
{
	passes[pass_index].domains[domain_index].texture_files[texture_index].filename = in_filename;
}
void BCR_VisualFXInstance::SetTextureType(unsigned int pass_index, unsigned domain_index, unsigned int texture_index, TextureType type)
{
	passes[pass_index].domains[domain_index].texture_files[texture_index].type = type;
}

//Serialization
//***********************************
//	BCR_VisualFXInstance
//***********************************
void BCR_VisualFXInstance::Serialize(Stream & stream) const
{
	STLSerializer<std::vector<Pass> > pass_serializer(stream);
	STLSerializer<std::string> string_serializer(stream);

	string_serializer.Save(class_file);
	stream.Save(technique_index);
	pass_serializer.Save(passes);
}
void BCR_VisualFXInstance::Unserialize(Stream & stream)
{
	STLSerializer<std::vector<Pass> > pass_serializer(stream);
	STLSerializer<std::string> string_serializer(stream);

	string_serializer.Load(class_file);
	stream.Load(technique_index);
	pass_serializer.Load(passes);
}
void BCR_VisualFXInstance::SerializeTypeId(Stream & stream) const
{
	stream.Save("BCR_VisualFXInstance");
}
//***********************************
//	BCR_VisualFXInstance::Pass
//***********************************
void BCR_VisualFXInstance::Pass::Serialize(Stream & stream) const
{
	STLSerializer<std::vector<Domain> > domain_serializer(stream);
	domain_serializer.Save(domains);
}
void BCR_VisualFXInstance::Pass::Unserialize(Stream & stream)
{
	STLSerializer<std::vector<Domain> > domain_serializer(stream);
	domain_serializer.Load(domains);
}
void BCR_VisualFXInstance::Pass::SerializeTypeId(Stream & stream) const
{
	//Never called
}
//**************************************
//	BCR_VisualFXInstance::Domain
//**************************************
void BCR_VisualFXInstance::Domain::Serialize(Stream & stream) const
{
	STLSerializer<std::vector<TextureFile> > tex_serializer(stream);
	tex_serializer.Save(texture_files);

	STLSerializer<std::vector<UniformParam> > param_serializer(stream);
	param_serializer.Save(uniform_params);
}
void BCR_VisualFXInstance::Domain::Unserialize(Stream & stream)
{
	STLSerializer<std::vector<TextureFile> > tex_serializer(stream);
	tex_serializer.Load(texture_files);

	STLSerializer<std::vector<UniformParam> > param_serializer(stream);
	param_serializer.Load(uniform_params);
}
void BCR_VisualFXInstance::Domain::SerializeTypeId(Stream & stream) const
{
	//Never called
}
//**************************************
//	BCR_VisualFXInstance::TextureFile
//**************************************
void BCR_VisualFXInstance::TextureFile::Serialize(Stream & stream) const
{
	STLSerializer<std::string> string_serializer(stream);
	stream.Save(type);
	string_serializer.Save(filename);
}
void BCR_VisualFXInstance::TextureFile::Unserialize(Stream & stream)
{
	STLSerializer<std::string> string_serializer(stream);
	stream.Load(type);
	string_serializer.Load(filename);
}
void BCR_VisualFXInstance::TextureFile::SerializeTypeId(Stream & stream) const
{
	//Never called
}
//**************************************
//	BCR_VisualFXInstance::UniformParam
//**************************************
void BCR_VisualFXInstance::UniformParam::Serialize(Stream & stream) const
{
	stream.Save(rows);
	stream.Save(columns);
	stream.Save(column_major);
	stream.Save(type);
	switch (type)
	{
		case DOUBLE:
			stream.Save(data.double_data, rows * columns);
			break;
		case FLOAT:
			stream.Save(data.float_data, rows * columns);
			break;
		case INT:
			stream.Save(data.int_data, rows * columns);
			break;
		default:
			throw 1;
			break;
	}
}
void BCR_VisualFXInstance::UniformParam::Unserialize(Stream & stream)
{
	stream.Load(rows);
	stream.Load(columns);
	stream.Load(column_major);
	stream.Load(type);
	switch (type)
	{
		case DOUBLE:
			stream.Load(data.double_data, rows * columns);
			break;
		case FLOAT:
			stream.Load(data.float_data, rows * columns);
			break;
		case INT:
			stream.Load(data.int_data, rows * columns);
			break;
		default:
			throw 1;
			break;
	}
}
void BCR_VisualFXInstance::UniformParam::SerializeTypeId(Stream & stream) const
{
	//Never called
}
*/