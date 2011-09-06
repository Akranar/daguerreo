#include "RenderContextPimpl_DIRECTX9.h"
#include "WindowData_DIRECTX9.h"
#include "../WindowPixelFormat.h"
#include "../RenderContext.h"
#include <iostream>

#include <d3d9.h>
//#include "ExLib_DirectX9.h"

RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const WindowPixelFormat * pixel_format)
{
	int width = 1920;
	int height = 1080;
	

	//initialize Direct3D
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d == NULL)
    {
        MessageBox(window_data->GetWindowHandle(), "Error initializing Direct3D", "Error", MB_OK);
        throw 1;
    }

    //set Direct3D presentation parameters
    D3DPRESENT_PARAMETERS d3dpp; 
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_COPY;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
    d3dpp.hDeviceWindow = window_data->GetWindowHandle();

    //create Direct3D device
    d3d->CreateDevice(
        D3DADAPTER_DEFAULT, 
        D3DDEVTYPE_HAL, 
        window_data->GetWindowHandle(),
        D3DCREATE_HARDWARE_VERTEXPROCESSING,
        &d3dpp, 
        &d3ddev);

    if (d3ddev == NULL)
    {
        MessageBox(window_data->GetWindowHandle(), "Error creating Direct3D device", "Error", MB_OK);
        throw 1;
    }

    //clear the backbuffer to black
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

    //create pointer to the back buffer
    d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
	d3ddev->CreateDepthStencilSurface(width, height, D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE, &depthstencil, 0);
	d3ddev->SetDepthStencilSurface(depthstencil);
}

RenderContextPimpl::RenderContextPimpl(WindowData * window_data, const RenderContext * render_context)
{
	int width = 800;
	int height = 600;
	

	//initialize Direct3D
    d3d = Direct3DCreate9(D3D_SDK_VERSION);
    if (d3d == NULL)
    {
        MessageBox(window_data->GetWindowHandle(), "Error initializing Direct3D", "Error", MB_OK);
        throw 1;
    }

    //set Direct3D presentation parameters
    
    ZeroMemory(&d3dpp, sizeof(d3dpp));

    d3dpp.Windowed = true;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_X8R8G8B8;
    d3dpp.BackBufferCount = 1;
    d3dpp.BackBufferWidth = width;
    d3dpp.BackBufferHeight = height;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
    d3dpp.hDeviceWindow = window_data->GetWindowHandle();

    //create Direct3D device
    d3d->CreateDevice(
        D3DADAPTER_DEFAULT, 
        D3DDEVTYPE_HAL, 
        window_data->GetWindowHandle(),
        D3DCREATE_SOFTWARE_VERTEXPROCESSING,
        &d3dpp, 
        &d3ddev);

    if (d3ddev == NULL)
    {
        MessageBox(window_data->GetWindowHandle(), "Error creating Direct3D device", "Error", MB_OK);
        throw 1;
    }

    //clear the backbuffer to black
    d3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0,0,0), 1.0f, 0);

    //create pointer to the back buffer
    d3ddev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backbuffer);
}

RenderContextPimpl::~RenderContextPimpl()
{
	backbuffer->Release();
	d3ddev->Release();
	d3d->Release(); 
}

