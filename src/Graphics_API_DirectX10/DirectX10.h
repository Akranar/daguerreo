#ifndef _DIRECT_X_10_H_
#define _DIRECT_X_10_H_

#include <d3dx10.h>
#include <D3D10Shader.h>

class DirectXDeviceManager
{
public:
	static ID3D10Device * active_device;
	static ID3D10RenderTargetView * active_render_target;
	static ID3D10DepthStencilView * active_depth_stencil;
	static IDXGISwapChain * active_swap_chain;
};

#endif