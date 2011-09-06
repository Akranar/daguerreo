#ifndef _RENDER_CONTEXT_PIMPL_WINDOWS_H_
#define _RENDER_CONTEXT_PIMPL_WINDOWS_H_

//#include <windows.h>
#include <d3dx10.h>

class WindowData;
class WindowPixelFormat;
class RenderContext;

struct IDXGISwapChain;
struct ID3D10RenderTargetView;
struct ID3D10Device;

class RenderContextPimpl
{
	
	IDXGISwapChain * swap_chain;
	ID3D10RenderTargetView * render_target_view;
	ID3D10Texture2D * depth_stencil;
	ID3D10DepthStencilView * depth_stencil_view;
	ID3D10Device* d3d_device;
public:
	RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format);
	RenderContextPimpl(WindowData * window_data, const RenderContext * render_context);
	~RenderContextPimpl();

	inline IDXGISwapChain * GetSwapChain() const;
	inline ID3D10RenderTargetView * GetRenderTargetView() const;
	inline ID3D10DepthStencilView * GetDepthStencilView() const;
	inline ID3D10Device * GetDevice() const;
};


inline IDXGISwapChain * RenderContextPimpl::GetSwapChain() const
{
	return swap_chain;
}
inline ID3D10RenderTargetView * RenderContextPimpl::GetRenderTargetView() const
{
	return render_target_view;
}
inline ID3D10DepthStencilView * RenderContextPimpl::GetDepthStencilView() const
{
	return depth_stencil_view;
}
inline ID3D10Device * RenderContextPimpl::GetDevice() const
{
	return d3d_device;
}



#endif

