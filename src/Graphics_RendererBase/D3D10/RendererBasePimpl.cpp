#include "RendererBasePimpl.h"
#include "ExLibPrivate_WindowEvents.h"

#include <d3d9.h>
#include "../RendererBase.h"

RendererBasePimpl::RendererBasePimpl(WindowEvents * window)
	: clear_depth(1.0f), clear_stencil(0), bound_index_buffer(0)
{
	
	swap_chain = window->GetRenderContext()->GetPimpl()->GetSwapChain();
	render_target_view = window->GetRenderContext()->GetPimpl()->GetRenderTargetView();
	depth_stencil_view = window->GetRenderContext()->GetPimpl()->GetDepthStencilView();
	d3d_device = window->GetRenderContext()->GetPimpl()->GetDevice();

}

RendererBasePimpl::~RendererBasePimpl()
{
	
}

void RendererBasePimpl::InputLayoutHelper_AppendZeroIfLastAlpha(std::string & name)
{
	int substr_length = name.length();
	if (isalpha(name[substr_length-1]))
	{
		name.push_back('0');
	}
}

bool RendererBasePimpl::InputLayoutHelper_NameToUsage(const std::string & name, unsigned int & rtn_usage_length, unsigned int & rtn_usage_index)
{
	rtn_usage_length = name.length();
	while (isdigit(name[rtn_usage_length-1]))
	{
		rtn_usage_length -= 1;
	}
	if (rtn_usage_length == name.length())
	{
		rtn_usage_index = 0;
	}
	rtn_usage_index = atoi(name.c_str()+rtn_usage_length);//atoi(name.substr(rtn_usage_length, name.length() - rtn_usage_length).c_str());
	return true;
}