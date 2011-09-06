#ifndef _RENDERER_BASE_PIMPL_H_
#define _RENDERER_BASE_PIMPL_H_

#include <d3d10.h>
class WindowEvents;
#include <map>

class IndexBuffer;
class RendererBase;

class RendererBasePimpl
{
public:
	friend class RendererBase;
	IDXGISwapChain * swap_chain;
	ID3D10RenderTargetView * render_target_view;
	ID3D10Texture2D * depth_stencil;
	ID3D10DepthStencilView * depth_stencil_view;
	ID3D10Device* d3d_device;

	FLOAT clear_color[4];
	float clear_depth;
	int clear_stencil;

	IndexBuffer * bound_index_buffer;

	RendererBasePimpl()
		: clear_depth(1.0f), clear_stencil(0), bound_index_buffer(0)
	{};
	RendererBasePimpl(WindowEvents * window);
	~RendererBasePimpl();

	std::map<unsigned int, std::map<unsigned int, ID3D10InputLayout *> > input_layouts;

	void InputLayoutHelper_AppendZeroIfLastAlpha(std::string & name);
	bool InputLayoutHelper_NameToUsage(const std::string & name, unsigned int & rtn_usage_length, unsigned int & rtn_usage_index);
};

#endif