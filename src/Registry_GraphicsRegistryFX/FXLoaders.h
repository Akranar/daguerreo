#ifndef _FX_LOADERS_H_
#define _FX_LOADERS_H_

#include "Exlib_RendererFX.h"

class FxClassLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * rm, VisualFXClass * resource)
	{

	}
	template <class MANAGER_TYPE>
	void Unload(VisualFXClass * resource, MANAGER_TYPE * rm)
	{

	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};

class FxInstanceLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * rm, VisualFXInstance * resource)
	{
		
	}
	template <class MANAGER_TYPE>
	void Unload(VisualFXInstance * resource, MANAGER_TYPE * rm)
	{

	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};

#endif