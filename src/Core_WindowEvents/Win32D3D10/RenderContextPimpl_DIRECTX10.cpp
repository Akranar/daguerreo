#include "RenderContextPimpl_DIRECTX10.h"
#include "WindowData_DIRECTX10.h"
#include "../WindowPixelFormat.h"
#include "../RenderContext.h"
#include <iostream>


RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format)
{
	//	Initializing Direct3D
	//		1. Create ID3D10Device and IDXGISwapChain with D3D10CreateDeviceAndSwapChain.
	//		2. Create the Render Target View To Back Buffer.
	//		3. Set Viewport.
	//		4. Set Projection and View Matrices.
	
	//int width = 800;
	//int height = 600;
	int width = 1920;
	int height = 1080;

	//1. Create ID3D10Device and IDXGISwapChain with D3D10CreateDeviceAndSwapChain {
	DXGI_SWAP_CHAIN_DESC swap_chain_description;

	//Back buffer descriptions
	swap_chain_description.BufferDesc.Width = width; // use window’s client area dims
	swap_chain_description.BufferDesc.Height = height;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Sampling descriptions
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;

	// Swap chain descriptions
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.BufferCount = 1;
	swap_chain_description.OutputWindow = window_data->GetWindowHandle();
	swap_chain_description.Windowed = true;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_description.Flags = 0;


	UINT createDeviceFlags = 0;
	D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, D3D10_SDK_VERSION, 
	&swap_chain_description, &swap_chain, &d3d_device);

	
	//1. Create ID3D10Device and IDXGISwapChain with D3D10CreateDeviceAndSwapChain. }
	//2. Create the Render Target View To Back Buffer. {
	

	
	ID3D10Texture2D* back_buffer;
	swap_chain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&back_buffer);
	d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
	back_buffer->Release();

	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	
	d3d_device->CreateTexture2D(&descDepth, NULL, &depth_stencil);

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	d3d_device->CreateDepthStencilView(depth_stencil, &descDSV, &depth_stencil_view);

	d3d_device->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	
	//2. Create the Render Target View To Back Buffer. }
	//3. Set Viewport. {
	

	D3D10_VIEWPORT view_port;
	view_port.Width = width;
	view_port.Height = height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	 
	d3d_device->RSSetViewports(1, &view_port);

	//3. Set Viewport. }
}

RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const RenderContext * render_context)
{
	//	Initializing Direct3D
	//		1. Create ID3D10Device and IDXGISwapChain with D3D10CreateDeviceAndSwapChain.
	//		2. Create the Render Target View To Back Buffer.
	//		3. Set Viewport.
	//		4. Set Projection and View Matrices.
	
	//int width = 800;
	//int height = 600;
	int width = 1024;
	int height = 768;

	//1. Create ID3D10Device and IDXGISwapChain with D3D10CreateDeviceAndSwapChain {
	DXGI_SWAP_CHAIN_DESC swap_chain_description;

	//Back buffer descriptions
	swap_chain_description.BufferDesc.Width = width; // use window’s client area dims
	swap_chain_description.BufferDesc.Height = height;
	swap_chain_description.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_description.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_description.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_description.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swap_chain_description.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	// Sampling descriptions
	swap_chain_description.SampleDesc.Count = 1;
	swap_chain_description.SampleDesc.Quality = 0;

	// Swap chain descriptions
	swap_chain_description.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_description.BufferCount = 1;
	swap_chain_description.OutputWindow = window_data->GetWindowHandle();
	swap_chain_description.Windowed = true;
	swap_chain_description.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swap_chain_description.Flags = 0;


	UINT createDeviceFlags = 0;
	D3D10CreateDeviceAndSwapChain(0, D3D10_DRIVER_TYPE_HARDWARE, 0, createDeviceFlags, D3D10_SDK_VERSION, 
	&swap_chain_description, &swap_chain, &d3d_device);

	
	//1. Create ID3D10Device and IDXGISwapChain with D3D10CreateDeviceAndSwapChain. }
	//2. Create the Render Target View To Back Buffer. {
	

	
	ID3D10Texture2D* back_buffer;
	swap_chain->GetBuffer(0, __uuidof(ID3D10Texture2D), (LPVOID*)&back_buffer);
	d3d_device->CreateRenderTargetView(back_buffer, NULL, &render_target_view);
	back_buffer->Release();
	


	D3D10_TEXTURE2D_DESC descDepth;
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D10_USAGE_DEFAULT;
	descDepth.BindFlags = D3D10_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;

	
	d3d_device->CreateTexture2D(&descDepth, NULL, &depth_stencil);

	D3D10_DEPTH_STENCIL_VIEW_DESC descDSV;
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D10_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;

	d3d_device->CreateDepthStencilView(depth_stencil, &descDSV, &depth_stencil_view);

	d3d_device->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);

	
	//2. Create the Render Target View To Back Buffer. }
	//3. Set Viewport. {
	

	D3D10_VIEWPORT view_port;
	view_port.Width = width;
	view_port.Height = height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	 
	d3d_device->RSSetViewports(1, &view_port);


	//3. Set Viewport. }
}

RenderContextPimpl::~RenderContextPimpl()
{
	d3d_device->Release();
	swap_chain->Release();
	render_target_view->Release();
	depth_stencil_view->Release();
}

