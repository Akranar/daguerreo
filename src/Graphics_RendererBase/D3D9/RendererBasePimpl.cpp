#include "RendererBasePimpl.h"
#include "ExLibPrivate_WindowEvents.h"

#include <ctype.h>
#include <cstdlib>

RendererBasePimpl::RendererBasePimpl(WindowEvents * window)
: clear_depth(1.0f), clear_stencil(0), RasterizerState_FrontCounterClockwise(true)
{
	d3d_backbuffer = window->GetRenderContext()->GetPimpl()->GetBackBuffer();
	d3d_device = window->GetRenderContext()->GetPimpl()->GetDevice();
	d3d = window->GetRenderContext()->GetPimpl()->GetD3D9();

	map_name_to_usage["POSITION"] = D3DDECLUSAGE_POSITION;
	map_name_to_usage["BLENDWEIGHT"] = D3DDECLUSAGE_BLENDWEIGHT;	
	map_name_to_usage["BLENDINDICES"] = D3DDECLUSAGE_BLENDINDICES;	
	map_name_to_usage["NORMAL"] = D3DDECLUSAGE_NORMAL;
	map_name_to_usage["PSIZE"] = D3DDECLUSAGE_PSIZE;
	map_name_to_usage["TEXCOORD"] = D3DDECLUSAGE_TEXCOORD;	
	map_name_to_usage["TANGENT"] = D3DDECLUSAGE_TANGENT;	
	map_name_to_usage["BINORMAL"] = D3DDECLUSAGE_BINORMAL;	
	map_name_to_usage["TESSFACTOR"] = D3DDECLUSAGE_TESSFACTOR;		
	map_name_to_usage["POSITIONT"] = D3DDECLUSAGE_POSITIONT;	
	map_name_to_usage["COLOR"] = D3DDECLUSAGE_COLOR;	
	map_name_to_usage["FOG"] = D3DDECLUSAGE_FOG;			
	map_name_to_usage["DEPTH"] = D3DDECLUSAGE_DEPTH;	
	map_name_to_usage["SAMPLE"] = D3DDECLUSAGE_SAMPLE;

}

bool RendererBasePimpl::InputLayoutHelper_NameToUsage(const std::string & name, BYTE & rtn_usage, BYTE & rtn_usage_index)
{
	int substr_length = name.length();
	while (isdigit(name[substr_length-1]))
	{
		substr_length -= 1;
	}
	std::map<std::string, D3DDECLUSAGE>::iterator find_result = map_name_to_usage.find(name.substr(0, substr_length));
	if (find_result == map_name_to_usage.end())
	{
		return false;
	}
	rtn_usage = find_result->second;
	if (substr_length == name.length())
	{
		rtn_usage_index = 0;
	}
	rtn_usage_index = atoi(name.substr(substr_length, name.length() - substr_length).c_str());
	return true;
}