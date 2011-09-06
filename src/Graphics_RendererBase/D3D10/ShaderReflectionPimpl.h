#ifndef _SHADER_REFLECTION_PIMPL_H_
#define _SHADER_REFLECTION_PIMPL_H_

#include <d3d10.h>
#include <d3dx10.h>

#include <vector>
#include <string>

class ShaderReflectionPimpl
{
public:
	std::vector<std::string> uniform_names;
	std::vector<int> uniform_locations;
	std::vector<int> uniform_types;
	std::vector<int> uniform_sizes;

	std::vector<std::string> attrib_names;
	std::vector<int> attrib_locations;
	std::vector<int> attrib_types;
	std::vector<int> attrib_sizes;

	std::vector<std::string> sampler_names;
	std::vector<int> sampler_locations;
	std::vector<int> sampler_types;
	std::vector<int> sampler_units;
	std::vector<int> sampler_sizes;

	void PrintReflection(ID3D10ShaderReflection * reflection);
};

#endif