#ifndef _RENDERER_BASE_H_
#define _RENDERER_BASE_H_

#include "Enumerants.h"

class DescVertexBuffer;
class DescIndexBuffer;
class DescTexture1D;
class DescTexture2D;
class DescTexture3D;
class DescTextureCube;
class DescShaderProgram;
class DescBlendState;
class DescDepthStencilState;
class DescRasterizerState;
class DescSamplerState;
class DescVertexFormat;
class DescUniformBuffer;

class VertexBuffer;
class IndexBuffer;
class Texture1D;
class Texture2D;
class Texture3D;
class TextureCube;
class ShaderProgram;
class BlendState;
class DepthStencilState;
class RasterizerState;
class SamplerState;
class UniformBuffer;

class UniformUpdater;

class WindowEvents;
class RendererBasePimpl;

class ShaderReflection;

#include <map>
#include <vector>

#include "DescVertexFormat.h"
#include "DescShaderSignature.h"
#include "CompareVertexFormat.h"
#include "CompareShaderSignature.h"

#include "Interface_BlendState.h"
#include "Interface_DepthStencilState.h"
#include "Interface_RasterizerState.h"
#include "Interface_SamplerState.h"


class RendererBase
{
	void PlatformIndependentInit();
	BlendState DefaultBlend;
	DepthStencilState DefaultDepthStencil;
	RasterizerState DefaultRasterizer;
	SamplerState DefaultSampler;
public:
	RendererBasePimpl * pimpl;
	RendererBase();
	~RendererBase();
	void Initialize(WindowEvents * window);

	//Render
	void PreRender();
	void SetPrimitiveType(unsigned int primitive_type);
	void Draw();
	void PostRender();

	//VertexBuffer
	void CreateResource(const DescVertexBuffer & desc, VertexBuffer * resource);
	void Delete(VertexBuffer * resource);
	void BindVertexBuffer(VertexBuffer * resource);
	void * Map(VertexBuffer * resource, MapAccess access);
	void Unmap(VertexBuffer * resource);

	//IndexBuffer
	void CreateResource(const DescIndexBuffer & desc, IndexBuffer * resource);
	void Delete(IndexBuffer * resource);
	void BindIndexBuffer(IndexBuffer * resource);
	void * Map(IndexBuffer * resource, MapAccess access);
	void Unmap(IndexBuffer * resource);

	//UniformBuffer
	void CreateResource(const DescUniformBuffer & desc, UniformBuffer * resource);
	void Delete(UniformBuffer * resource);
	void BindUniformBuffer(UniformBuffer * resource, ShaderProgram * program);
	unsigned char * Map(UniformBuffer * resource);
	void Unmap(UniformBuffer * resource);

	//ShaderProgram
	void CreateResource(const DescShaderProgram & desc, ShaderProgram * resource);
	void Delete(ShaderProgram * resource);
	void BindShaderProgram(ShaderProgram * resource);
	void CreateUniformUpdater(ShaderProgram * program, unsigned int domain, UniformUpdater * updater);
	void DestroyUniformUpdater(UniformUpdater * updater);
	void SetUniformf(ShaderProgram * resource, unsigned int domain, unsigned int index, unsigned int count, bool transpose, float * data);

	//ShaderReflection
	void CreateShaderReflection(ShaderProgram * resource, ShaderReflection * reflection);
	void DeleteShaderReflection(ShaderReflection * reflection);

	//Texture2D
	void CreateResource(const DescTexture2D & desc, Texture2D * resource);
	void Delete(Texture2D * resource);
	void Bind(ShaderProgram * program, unsigned int unit, Texture2D * resource);
	void * Map(Texture2D * resource, MapAccess access);
	void Unmap(Texture2D * resource);

	//TextureCube
	void CreateResource(const DescTextureCube & desc, TextureCube * resource);
	void Delete(TextureCube * resource);
	void Bind(ShaderProgram * program, unsigned int unit, TextureCube * resource);
	void * Map(TextureCube * resource, MapAccess access, unsigned int face);
	void Unmap(TextureCube * resource, unsigned int face);

	void CreateResource(const DescTexture1D & desc, Texture1D * resource);
	void CreateResource(const DescTexture3D & desc, Texture3D * resource);


	
	bool ValidateInputLayout(ShaderProgram * program, VertexBuffer * buffer);

	void ClearAllBuffer();
	void ClearColorBuffer();
	void ClearDepthBuffer();
	void ClearStencilBuffer();
	void ClearDepthStencilBuffer();
	void ClearColor(float r, float g, float b, float a);
	void ClearDepth(double depth);
	void ClearStencil(int stencil);




	//RenderStates
	void CreateDefaultStates(BlendState * blend_state, DepthStencilState * depth_stencil_state, RasterizerState * rasterizer_state, SamplerState * sampler_state);

	void CreateState(const DescBlendState & desc, BlendState * state);
	void CreateState(const DescDepthStencilState & desc, DepthStencilState * state);
	void CreateState(const DescRasterizerState & desc, RasterizerState * state);
	void CreateState(const DescSamplerState & desc, SamplerState * state);

	void SetState(const BlendState * state);
	void SetState(const DepthStencilState * state);
	void SetState(const RasterizerState * state);
	void SetState(const SamplerState * state, unsigned int unit);

	void DestroyState(BlendState * state);
	void DestroyState(DepthStencilState * state);
	void DestroyState(RasterizerState * state);
	void DestroyState(SamplerState * state);

	//Viewport
	void OnViewportUpdate(int x, int y, int width, int height);



	//
	unsigned int next_vf_hash;
	unsigned int next_ss_hash;
	std::map<DescVertexFormat, unsigned int, CompareVertexFormat> vf_hasher;
	std::map<DescShaderSignature, unsigned int, CompareShaderSignature> ss_hasher;
	std::vector<const DescVertexFormat *> hash_to_format;
	
	unsigned int GetVertexFormatHash(DescVertexFormat & desc_vf);
	DescShaderSignature & GetShaderSignatureHash(DescShaderSignature & desc_ss, unsigned int & out_hash);
	void DestroyHashers();

	void SetInputLayout(unsigned int vertex_format_hash, unsigned int shader_signature_hash);
	void CreateInputLayout(unsigned int vertex_format_hash, ShaderProgram * program);

};


#endif