#ifndef _SHADER_REFLECTION_H_
#define _SHADER_REFLECTION_H_

#include <vector>
#include <string>

class ShaderReflection
{
public:
	std::vector<std::string> attribs;
	std::vector<std::string> uniforms;
	std::vector<std::string> uniform_blocks;
};

class InputLayoutEntry
{
public:
	unsigned int offset;
	unsigned short stride;
	unsigned char channel;
	unsigned char attrib_index;
};

class InputLayout
{
public:
	InputLayoutEntry entries[16];
};

class VertexElementDesc
{
public:
	unsigned int SemanticName;
	unsigned int SemanticIndex;
	unsigned int Format; //Type and Channel
	unsigned int InputSlot;
	unsigned int AlignedByteOffset; //Offset
	unsigned int InputSlotClass; //Vertex or Instance
	unsigned int InstanceDataStepRate; //0 For vertex
}

#endif