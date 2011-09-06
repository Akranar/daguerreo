/*
#include "BCR_VisualFXClass.h"

BCR_VisualFXClass::BCR_VisualFXClass() 
: 
technique_count(0), 
techniques(0) 
{
};

BCR_VisualFXClass::~BCR_VisualFXClass()
{
	if (techniques) delete [] techniques;
}
void BCR_VisualFXClass::SetTechniqueCount(unsigned int count)
{
	if (!techniques && count)
	{
		technique_count = count;
		techniques = new Technique[count];
	}
}
void BCR_VisualFXClass::SetTechnique(unsigned int technique_index, unsigned int pass_count)
{
	if (!techniques[technique_index].passes && pass_count)
	{
		techniques[technique_index].pass_count = pass_count;
		techniques[technique_index].passes = new Pass[pass_count];
	}
}
void BCR_VisualFXClass::SetPass(unsigned int tech, unsigned int pass, const std::string & program_file, RenderStateList & src_states)
{
	techniques[tech].passes[pass].program_filename = program_file;
	techniques[tech].passes[pass].render_states.TransferStatesFrom(src_states);
}

//Serialization

void BCR_VisualFXClass::Serialize(Stream & stream) const
{
	STLSerializer<std::string> string_serializer(stream);
	stream.Save(technique_count);
	for (unsigned int i = 0; i < technique_count; ++i)
	{
		stream.Save(techniques[i].pass_count);
		for (unsigned int j = 0; j < techniques[i].pass_count; ++j)
		{
			string_serializer.Save(techniques[i].passes[j].program_filename);
			stream.Save(techniques[i].passes[j].render_states);
		}
	}
}
void BCR_VisualFXClass::Unserialize(Stream & stream)
{
	STLSerializer<std::string> string_serializer(stream);
	stream.Load(technique_count);
	SetTechniqueCount(technique_count);
	for (unsigned int i = 0; i < technique_count; ++i)
	{
		unsigned int pass_count = 0;
		stream.Load(pass_count);
		SetTechnique(i, pass_count);
		for (unsigned int j = 0; j < pass_count; ++j)
		{
			string_serializer.Load(techniques[i].passes[j].program_filename);
			stream.Load(techniques[i].passes[j].render_states);
		}
	}
}
void BCR_VisualFXClass::SerializeTypeId(Stream & stream) const
{
	stream.Save("BCR_VisualFXClass");
}
*/