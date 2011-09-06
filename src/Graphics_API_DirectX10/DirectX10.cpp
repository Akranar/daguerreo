#include "DirectX10.h"

ID3D10Device * DirectXDeviceManager::active_device = 0;
ID3D10RenderTargetView * DirectXDeviceManager::active_render_target = 0;
ID3D10DepthStencilView * DirectXDeviceManager::active_depth_stencil = 0;
IDXGISwapChain * DirectXDeviceManager::active_swap_chain = 0;
