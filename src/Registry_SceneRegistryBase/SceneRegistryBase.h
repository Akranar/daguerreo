#ifndef _SCENE_REGISTRY_BASE_H_
#define _SCENE_REGISTRY_BASE_H_

#include "ExLib_SceneBase.h"
#include "ExLib_Catalogs.h"
#include "ExLib_Serialization.h"
#include <Core_ObjectFactory.h>
#include <Core_Exception.h>
#include "NodeLoaders.h"
#include "ExLib_BoundingVolume.h"

//*************************************************************************************
//	Note:
//		SceneRegistryBase is a class Templated on:
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
	class BASE_MANAGER,
	template <class, class> class VISUAL_CATALOG = MultiSetCatalog
>
class SceneRegistryBase : public BASE_MANAGER
{
public:
	typedef VISUAL_CATALOG<Visual, VisualLoader> VisualCatalog;
private:
	VisualCatalog visual_catalog;
public:
	//********************************************
	//	Constructor/Destructors
	//********************************************
	SceneRegistryBase();
	~SceneRegistryBase();

	//********************************************
	//	Catalog Get Methods.
	//********************************************

	inline VisualCatalog & GetVisualCatalog();

	//********************************************
	//	ClearAll
	//********************************************

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

#define SCENE_REGISTRY_BASE_TEMPLATE_ARGS \
template <\
	class BASE_MANAGER, \
	template <class, class> class VISUAL_CATALOG \
>

#define SCENE_REGISTRY_BASE_DECLARATION_SCOPE \
SceneRegistryBase \
< \
	BASE_MANAGER,\
	VISUAL_CATALOG\
>


SCENE_REGISTRY_BASE_TEMPLATE_ARGS
SCENE_REGISTRY_BASE_DECLARATION_SCOPE::SceneRegistryBase()
{
	BASE_MANAGER::GetFactory().Register("DescVisual", new ObjectCreator<DescVisual, Serializable>());
	BASE_MANAGER::GetFactory().Register("AABB", new ObjectCreator<AABB, Serializable>());

	visual_catalog.GetLoader().SetFactory(&BASE_MANAGER::GetFactory());
}

SCENE_REGISTRY_BASE_TEMPLATE_ARGS
SCENE_REGISTRY_BASE_DECLARATION_SCOPE::~SceneRegistryBase()
{
	ClearAll();
}

//********************************************
//	Catalog Get Methods.
//********************************************

SCENE_REGISTRY_BASE_TEMPLATE_ARGS
inline typename SCENE_REGISTRY_BASE_DECLARATION_SCOPE::VisualCatalog & SCENE_REGISTRY_BASE_DECLARATION_SCOPE::GetVisualCatalog()
{
	return visual_catalog;
}

//***************************************
//	Clear All
//***************************************

SCENE_REGISTRY_BASE_TEMPLATE_ARGS
void SCENE_REGISTRY_BASE_DECLARATION_SCOPE::ClearAll()
{
	visual_catalog.Clear(this);
	BASE_MANAGER::ClearAll();
}

//********************************************
//	Retrieve Methods Overrides
//********************************************


//Retrieve
SCENE_REGISTRY_BASE_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
RESOURCE_TYPE * SCENE_REGISTRY_BASE_DECLARATION_SCOPE::Retrieve(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	return catalog.Retrieve(name, this)->GetResource();
}

SCENE_REGISTRY_BASE_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
RESOURCE_TYPE * SCENE_REGISTRY_BASE_DECLARATION_SCOPE::Retrieve(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	return catalog.Retrieve(name, this);
}

//Release By Resource
SCENE_REGISTRY_BASE_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void SCENE_REGISTRY_BASE_DECLARATION_SCOPE::Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource)
{
	catalog.Release(resource, this);
}

SCENE_REGISTRY_BASE_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void SCENE_REGISTRY_BASE_DECLARATION_SCOPE::Release(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource)
{
	catalog.Release(resource, this);
}

//Release By Name
SCENE_REGISTRY_BASE_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void SCENE_REGISTRY_BASE_DECLARATION_SCOPE::Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	catalog.Release(name, this);
}


#endif
