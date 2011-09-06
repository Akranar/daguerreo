#ifndef _RENDERER_BASE_PIMPL_H_
#define _RENDERER_BASE_PIMPL_H_

#include "../ExLib_WindowEvents.h"

#include <d3d9.h>
#include <map>
#include <string>

class RendererBasePimpl
{
friend class RendererBase;
	//D3D9
public:
	LPDIRECT3D9 d3d; 
	LPDIRECT3DDEVICE9 d3d_device; 
	LPDIRECT3DSURFACE9 d3d_backbuffer;

	D3DCOLOR clear_color;
	float clear_depth;
	int clear_stencil;
	bool RasterizerState_FrontCounterClockwise;

	std::map<unsigned int, IDirect3DVertexDeclaration9 *> input_layouts;

	RendererBasePimpl(WindowEvents * window);

	inline LPDIRECT3D9 GetD3D9() const;
	inline LPDIRECT3DDEVICE9 GetDevice() const;
	inline LPDIRECT3DSURFACE9 GetBackBuffer() const;

	std::map<std::string, D3DDECLUSAGE> map_name_to_usage;
	bool InputLayoutHelper_NameToUsage(const std::string & name, BYTE & rtn_usage, BYTE & rtn_usage_index);
};

inline LPDIRECT3D9 RendererBasePimpl::GetD3D9() const
{
	return d3d;
}
inline LPDIRECT3DDEVICE9 RendererBasePimpl::GetDevice() const
{
	return d3d_device;
}
inline LPDIRECT3DSURFACE9 RendererBasePimpl::GetBackBuffer() const
{
	return d3d_backbuffer;
}

#endif