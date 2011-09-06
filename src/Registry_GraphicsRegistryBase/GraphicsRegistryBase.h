#ifndef _GRAPHICS_REGISTRY_BASE_H_
#define _GRAPHICS_REGISTRY_BASE_H_

#include "ExLib_Catalogs.h"
#include "ResourceLoaders.h"
#include "ExLib_RendererBase.h"
#include <Core_ObjectFactory.h>
#include <Core_Exception.h>
#include "ExLib_Serialization.h"

//*************************************************************************************
//	Note:
//		GraphicsRegistryBase is a class Templated on:
//			-> BASE_RENDERER
//				-> The class of the renderer to Bind and Unbind the resources to.
//			-> Many CATALOGs
//				-> The templated class of the Catalog used for each resource.
//				-> Can be:
//					-> RefCountCatalog (Default)
//					-> MultiSetCatalog
//				-> The CATALOG template class is templated on two arguments:
//					-> RESOURCE_TYPE
//						-> The type of resource it holds.
//					-> LOADER
//						-> The class of the loader that will be used
//							-> If loading the resource from disk is required.
//*************************************************************************************
template 
<
	class BASE_RENDERER = RendererBase,
	template <class, class> class VERTEX_BUFFER_CATALOG = RefCountCatalog,
	template <class, class> class INDEX_BUFFER_CATALOG = RefCountCatalog,
	template <class, class> class TEXTURE_1D_CATALOG = RefCountCatalog,
	template <class, class> class TEXTURE_2D_CATALOG = RefCountCatalog,
	template <class, class> class TEXTURE_3D_CATALOG = RefCountCatalog,
	template <class, class> class TEXTURE_CUBE_CATALOG = RefCountCatalog,
	template <class, class> class SHADER_PROGRAM_CATALOG = RefCountCatalog
>
class GraphicsRegistryBase
{
public:
	typedef VERTEX_BUFFER_CATALOG<VertexBuffer, VertexBufferLoader> VertexBufferCatalog;
	typedef INDEX_BUFFER_CATALOG<IndexBuffer, IndexBufferLoader> IndexBufferCatalog;
	typedef TEXTURE_1D_CATALOG<Texture1D, Texture1DLoader> Texture1DCatalog;
	typedef TEXTURE_2D_CATALOG<Texture2D, Texture2DLoader> Texture2DCatalog;
	typedef TEXTURE_3D_CATALOG<Texture3D, Texture3DLoader> Texture3DCatalog;
	typedef TEXTURE_CUBE_CATALOG<TextureCube, TextureCubeLoader> TextureCubeCatalog;
	typedef SHADER_PROGRAM_CATALOG<ShaderProgram, ShaderProgramLoader> ShaderProgramCatalog;
	
protected:
	VertexBufferCatalog vertexbuffer_catalog;
	IndexBufferCatalog indexbuffer_catalog;
	Texture1DCatalog texture1d_catalog;
	Texture2DCatalog texture2d_catalog;
	Texture3DCatalog texture3d_catalog;
	TextureCubeCatalog texturecube_catalog;
	ShaderProgramCatalog shaderprogram_catalog;

	ObjectFactory<Serializable> factory;

	BASE_RENDERER * renderer;
public:
	//********************************************
	//	Constructor/Destructors
	//********************************************
	GraphicsRegistryBase();
	~GraphicsRegistryBase();

	//********************************************
	//	BASE_RENDERER
	//********************************************

	//Set/Get Renderer
	inline void SetRenderer(BASE_RENDERER * in);
	inline BASE_RENDERER * GetRenderer() const;
	inline ObjectFactory<Serializable> & GetFactory();

	//********************************************
	//	Catalog Get Methods.
	//********************************************

	inline VertexBufferCatalog & GetVertexBufferCatalog();
	inline IndexBufferCatalog & GetIndexBufferCatalog();
	inline Texture1DCatalog & GetTexture1DCatalog();
	inline Texture2DCatalog & GetTexture2DCatalog();
	inline Texture3DCatalog & GetTexture3DCatalog();
	inline TextureCubeCatalog & GetTextureCubeCatalog();
	inline ShaderProgramCatalog & GetShaderProgramCatalog();

	//********************************************
	//	BindAll / UnbindAll / ClearAll
	//********************************************

	void BindAll();
	void UnbindAll();
	void ClearAll();

	//********************************************
	//	Templated Interface On Catalogs
	//********************************************

	//Retrieve
	template<class RESOURCE_TYPE, class LOADER>
	RESOURCE_TYPE * Retrieve(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name);

	template<class RESOURCE_TYPE, class LOADER>
	RESOURCE_TYPE * Retrieve(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name);

	//Release By Resource
	template<class RESOURCE_TYPE, class LOADER>
	void Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource);

	template<class RESOURCE_TYPE, class LOADER>
	void Release(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource);

	//Release By Name
	template<class RESOURCE_TYPE, class LOADER>
	void Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name);
};

//********************************************
//	Constructor/Destructors
//********************************************

#define RESOURCE_MANAGER_TEMPLATE_ARGS \
template <\
	class BASE_RENDERER, \
	template <class, class> class VERTEX_BUFFER_CATALOG, \
	template <class, class> class INDEX_BUFFER_CATALOG, \
	template <class, class> class TEXTURE_1D_CATALOG, \
	template <class, class> class TEXTURE_2D_CATALOG, \
	template <class, class> class TEXTURE_3D_CATALOG, \
	template <class, class> class TEXTURE_CUBE_CATALOG, \
	template <class, class> class SHADER_PROGRAM_CATALOG\
>

#define RESOURCE_MANAGER_DECLARATION_SCOPE \
GraphicsRegistryBase \
< \
	BASE_RENDERER,\
	VERTEX_BUFFER_CATALOG,\
	INDEX_BUFFER_CATALOG, \
	TEXTURE_1D_CATALOG,\
	TEXTURE_2D_CATALOG,\
	TEXTURE_3D_CATALOG,\
	TEXTURE_CUBE_CATALOG, \
	SHADER_PROGRAM_CATALOG\
>


RESOURCE_MANAGER_TEMPLATE_ARGS
RESOURCE_MANAGER_DECLARATION_SCOPE::GraphicsRegistryBase()
:
renderer(0)
{
	factory.Register("DescVertexFormat", new ObjectCreator<DescVertexFormat, Serializable>());
	factory.Register("DescVertexBuffer", new ObjectCreator<DescVertexBuffer, Serializable>());
	factory.Register("DescIndexBuffer", new ObjectCreator<DescIndexBuffer, Serializable>());

	vertexbuffer_catalog.GetLoader().SetFactory(&factory);
	indexbuffer_catalog.GetLoader().SetFactory(&factory);
	texture1d_catalog.GetLoader().SetFactory(&factory);
	texture2d_catalog.GetLoader().SetFactory(&factory);
	texture3d_catalog.GetLoader().SetFactory(&factory);
	texturecube_catalog.GetLoader().SetFactory(&factory);
	shaderprogram_catalog.GetLoader().SetFactory(&factory);
}

RESOURCE_MANAGER_TEMPLATE_ARGS
RESOURCE_MANAGER_DECLARATION_SCOPE::~GraphicsRegistryBase()
{
	ClearAll();
}

//********************************************
//	BASE_RENDERER
//********************************************

RESOURCE_MANAGER_TEMPLATE_ARGS
inline void RESOURCE_MANAGER_DECLARATION_SCOPE::SetRenderer(BASE_RENDERER * in)
{
	renderer = in;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline BASE_RENDERER * RESOURCE_MANAGER_DECLARATION_SCOPE::GetRenderer() const
{
	CONDITIONAL_EXCEPTION(!renderer, "Renderer was not initialized in GraphicsRegistryBase.");
	return renderer;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline ObjectFactory<Serializable> & RESOURCE_MANAGER_DECLARATION_SCOPE::GetFactory()
{
	return factory;
}

//********************************************
//	Catalog Get Methods.
//********************************************

RESOURCE_MANAGER_TEMPLATE_ARGS
inline typename RESOURCE_MANAGER_DECLARATION_SCOPE::VertexBufferCatalog & RESOURCE_MANAGER_DECLARATION_SCOPE::GetVertexBufferCatalog()
{
	return vertexbuffer_catalog;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline typename RESOURCE_MANAGER_DECLARATION_SCOPE::IndexBufferCatalog & RESOURCE_MANAGER_DECLARATION_SCOPE::GetIndexBufferCatalog()
{
	return indexbuffer_catalog;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline typename RESOURCE_MANAGER_DECLARATION_SCOPE::Texture1DCatalog & RESOURCE_MANAGER_DECLARATION_SCOPE::GetTexture1DCatalog()
{
	return texture1d_catalog;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline typename RESOURCE_MANAGER_DECLARATION_SCOPE::Texture2DCatalog & RESOURCE_MANAGER_DECLARATION_SCOPE::GetTexture2DCatalog()
{
	return texture2d_catalog;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline typename RESOURCE_MANAGER_DECLARATION_SCOPE::Texture3DCatalog & RESOURCE_MANAGER_DECLARATION_SCOPE::GetTexture3DCatalog()
{
	return texture3d_catalog;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline typename RESOURCE_MANAGER_DECLARATION_SCOPE::TextureCubeCatalog & RESOURCE_MANAGER_DECLARATION_SCOPE::GetTextureCubeCatalog()
{
	return texturecube_catalog;
}

RESOURCE_MANAGER_TEMPLATE_ARGS
inline typename RESOURCE_MANAGER_DECLARATION_SCOPE::ShaderProgramCatalog & RESOURCE_MANAGER_DECLARATION_SCOPE::GetShaderProgramCatalog()
{
	return shaderprogram_catalog;
}

//********************************************
//	ClearAll
//********************************************



RESOURCE_MANAGER_TEMPLATE_ARGS
void RESOURCE_MANAGER_DECLARATION_SCOPE::ClearAll()
{
	vertexbuffer_catalog.Clear(this);
	indexbuffer_catalog.Clear(this);
	texture1d_catalog.Clear(this);
	texture2d_catalog.Clear(this);
	texture3d_catalog.Clear(this);
	texturecube_catalog.Clear(this);
	shaderprogram_catalog.Clear(this);
}

//********************************************
//	Templated Interface On Catalogs
//********************************************

//Retrieve
RESOURCE_MANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
RESOURCE_TYPE * RESOURCE_MANAGER_DECLARATION_SCOPE::Retrieve(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	return catalog.Retrieve(name, this)->GetResource();
}

RESOURCE_MANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
RESOURCE_TYPE * RESOURCE_MANAGER_DECLARATION_SCOPE::Retrieve(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	return catalog.Retrieve(name, this);
}

//Release
RESOURCE_MANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void RESOURCE_MANAGER_DECLARATION_SCOPE::Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource)
{
	catalog.Release(resource, this);
}

RESOURCE_MANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void RESOURCE_MANAGER_DECLARATION_SCOPE::Release(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource)
{
	catalog.Release(resource, this);
}

RESOURCE_MANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void RESOURCE_MANAGER_DECLARATION_SCOPE::Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	catalog.Release(name, this);
}

#endif