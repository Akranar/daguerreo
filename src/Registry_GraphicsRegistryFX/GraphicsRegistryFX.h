#ifndef _GRAPHICS_REGISTRY_FX_H_
#define _GRAPHICS_REGISTRY_FX_H_

#include "ExLib_Catalogs.h"
#include "ExLib_RendererFX.h"


#include "FXLoaders.h"

template 
<
	class BASE_MANAGER,
	template <class, class> class FX_CLASS_CATALOG = RefCountCatalog,
	template <class, class> class FX_INSTANCE_CATALOG = RefCountCatalog
>
class GraphicsRegistryFX: public BASE_MANAGER
{
public:
	typedef FX_CLASS_CATALOG<VisualFXClass, FxClassLoader> VisualFXClassCatalog;
	typedef FX_INSTANCE_CATALOG<VisualFXInstance, FxInstanceLoader> VisualFXInstanceCatalog;

protected:
	VisualFXClassCatalog visualfxclass_catalog;
	VisualFXInstanceCatalog visualfxinstance_catalog;
public:

	//********************************************
	//	Constructor/Destructor
	//********************************************

	GraphicsRegistryFX();
	~GraphicsRegistryFX();

	//********************************************
	//	Catalog Get Methods.
	//********************************************

	inline VisualFXClassCatalog & GetVisualFXClassCatalog();
	inline VisualFXInstanceCatalog & GetVisualFXInstanceCatalog();

	//********************************************
	//	ClearAll Override
	//********************************************

	void ClearAll();

	//********************************************
	//	Retrieve Methods Overrides
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

#define FXMANAGER_TEMPLATE_ARGS \
template \
< \
	class BASE_MANAGER, \
	template <class, class> class FX_CLASS_CATALOG, \
	template <class, class> class FX_INSTANCE_CATALOG \
>

#define FXMANAGER_DECLARATION_SCOPE \
	GraphicsRegistryFX<BASE_MANAGER, FX_CLASS_CATALOG, FX_INSTANCE_CATALOG>

//********************************************
//	Constructor/Destructor
//********************************************

FXMANAGER_TEMPLATE_ARGS
FXMANAGER_DECLARATION_SCOPE::GraphicsRegistryFX()
{
	//FIX LATER
	//factory.Register("DescVisualFXClass", new ObjectCreator<BCR_VisualFXClass, Serializable>());
	//factory.Register("DescVisualFXInstance", new ObjectCreator<BCR_VisualFXInstance, Serializable>());
	visualfxclass_catalog.GetLoader().SetFactory(&BASE_MANAGER::GetFactory());
	visualfxinstance_catalog.GetLoader().SetFactory(&BASE_MANAGER::GetFactory());
}

FXMANAGER_TEMPLATE_ARGS
FXMANAGER_DECLARATION_SCOPE::~GraphicsRegistryFX()
{
	ClearAll();
}

//********************************************
//	Catalog Get Methods.
//********************************************

FXMANAGER_TEMPLATE_ARGS
inline typename FXMANAGER_DECLARATION_SCOPE::VisualFXClassCatalog & FXMANAGER_DECLARATION_SCOPE::GetVisualFXClassCatalog()
{
	return visualfxclass_catalog;
}

FXMANAGER_TEMPLATE_ARGS
inline typename FXMANAGER_DECLARATION_SCOPE::VisualFXInstanceCatalog & FXMANAGER_DECLARATION_SCOPE::GetVisualFXInstanceCatalog()
{
	return visualfxinstance_catalog;
}

//********************************************
//	ClearAll
//********************************************

template 
<
	class BASE_MANAGER,
	template <class, class> class FX_CLASS_CATALOG,
	template <class, class> class FX_INSTANCE_CATALOG
>
void GraphicsRegistryFX<BASE_MANAGER, FX_CLASS_CATALOG, FX_INSTANCE_CATALOG>::ClearAll()
{
	visualfxinstance_catalog.Clear(this);
	visualfxclass_catalog.Clear(this);
	BASE_MANAGER::ClearAll();
}

//********************************************
//	Retrieve Methods Overrides
//********************************************


//Retrieve
FXMANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
RESOURCE_TYPE * FXMANAGER_DECLARATION_SCOPE::Retrieve(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	return catalog.Retrieve(name, this)->GetResource();
}

FXMANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
RESOURCE_TYPE * FXMANAGER_DECLARATION_SCOPE::Retrieve(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	return catalog.Retrieve(name, this);
}

//Release By Resource
FXMANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void FXMANAGER_DECLARATION_SCOPE::Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource)
{
	catalog.Release(resource, this);
}

FXMANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void FXMANAGER_DECLARATION_SCOPE::Release(MultiSetCatalog<RESOURCE_TYPE, LOADER> & catalog, RESOURCE_TYPE * resource)
{
	catalog.Release(resource, this);
}

//Release By Name
FXMANAGER_TEMPLATE_ARGS
template<class RESOURCE_TYPE, class LOADER>
void FXMANAGER_DECLARATION_SCOPE::Release(RefCountCatalog<RESOURCE_TYPE, LOADER> & catalog, const char * name)
{
	catalog.Release(name, this);
}


#endif