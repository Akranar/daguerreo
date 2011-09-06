#ifndef _REF_COUNT_CATALOG_H_
#define _REF_COUNT_CATALOG_H_

#include "ResourceRef.h"
#include <map>

template<class RESOURCE_TYPE, class LOADER>
class RefCountCatalog
{
public:
	typedef typename std::map<std::string, ResourceRef<RESOURCE_TYPE> >::iterator CatalogIterator;
private:
	typedef std::map<std::string, ResourceRef<RESOURCE_TYPE> > NameToRefMap;
	typedef std::map<RESOURCE_TYPE *, typename NameToRefMap::iterator > RefToNameMap;

	NameToRefMap name_map;
	RefToNameMap ref_map;
	LOADER loader;
public:
	inline unsigned int Size() const;
	inline LOADER & GetLoader();
	inline bool Add(const char * name, RESOURCE_TYPE * resource);

	inline ResourceRef<RESOURCE_TYPE> * Find(const char * name);
	inline ResourceRef<RESOURCE_TYPE> * Find(RESOURCE_TYPE * resource);

	inline CatalogIterator Begin();
	inline CatalogIterator End();

	template <class MANAGER_TYPE>
	inline ResourceRef<RESOURCE_TYPE> * Retrieve(const char * name, MANAGER_TYPE * manager);

	template <class MANAGER_TYPE>
	inline bool Release(RESOURCE_TYPE * resource, MANAGER_TYPE * manager);

	template <class MANAGER_TYPE>
	inline RESOURCE_TYPE * Release(const char * name, MANAGER_TYPE * manager);

	template <class MANAGER_TYPE>
	inline void Clear(MANAGER_TYPE * manager);
	
};

template <class RESOURCE_TYPE, class LOADER>
inline unsigned int RefCountCatalog<RESOURCE_TYPE, LOADER>::Size() const
{
	return name_map.size();
}

template <class RESOURCE_TYPE, class LOADER>
inline LOADER & RefCountCatalog<RESOURCE_TYPE, LOADER>::GetLoader()
{
	return loader;
}
template <class RESOURCE_TYPE, class LOADER>
inline bool RefCountCatalog<RESOURCE_TYPE, LOADER>::Add(const char * name, RESOURCE_TYPE * resource)
{
	std::pair<typename NameToRefMap::iterator, bool> insert_result = name_map.insert(std::make_pair(name, ResourceRef<RESOURCE_TYPE>(1, resource)));
	if (insert_result.second)
		//New entry added.
	{
		ref_map.insert(std::make_pair(resource, insert_result.first));
	}
	return insert_result.second;
}

template <class RESOURCE_TYPE, class LOADER>
inline ResourceRef<RESOURCE_TYPE> * RefCountCatalog<RESOURCE_TYPE, LOADER>::Find(const char * name)
{
	//NameToMap Look Up.
	typename NameToRefMap::iterator find_result = name_map.find(name);
	if (find_result == name_map.end())
	{
		return 0;
	}
	return &find_result->second;
}

template <class RESOURCE_TYPE, class LOADER>
inline ResourceRef<RESOURCE_TYPE> * RefCountCatalog<RESOURCE_TYPE, LOADER>::Find(RESOURCE_TYPE * resource)
{
	//RefToNameMap Look Up.
	typename RefToNameMap::iterator ref_find_result = ref_map.find(resource);
	if (ref_find_result == ref_map.end())
	{
		return false;
	}
	typename NameToRefMap::iterator name_find_result = ref_find_result->second;
	return &name_find_result->second;
}

template <class RESOURCE_TYPE, class LOADER>
inline typename RefCountCatalog<RESOURCE_TYPE, LOADER>::CatalogIterator RefCountCatalog<RESOURCE_TYPE, LOADER>::Begin()
{
	return name_map.begin();
}
template <class RESOURCE_TYPE, class LOADER>
inline typename RefCountCatalog<RESOURCE_TYPE, LOADER>::CatalogIterator RefCountCatalog<RESOURCE_TYPE, LOADER>::End()
{
	return name_map.end();
}

template <class RESOURCE_TYPE, class LOADER>
template <class MANAGER_TYPE>
inline ResourceRef<RESOURCE_TYPE> * RefCountCatalog<RESOURCE_TYPE, LOADER>::Retrieve(const char * name, MANAGER_TYPE * manager)
{
	//Attempt To Find Resource.
	ResourceRef<RESOURCE_TYPE> * find_result = Find(name);
	if (!find_result)
	{
		//Load Resource.
		
		RESOURCE_TYPE  * resource = new RESOURCE_TYPE();
		loader.Load(name, manager, resource);
		//Add Resource.
		std::pair<typename NameToRefMap::iterator, bool> insert_result = name_map.insert(std::make_pair(name, ResourceRef<RESOURCE_TYPE>(1, resource)));
		ref_map.insert(std::make_pair(resource, insert_result.first));
		return &insert_result.first->second;
	}
	else
	{
		find_result->IncrementRef();
		return find_result;
	}
}

template <class RESOURCE_TYPE, class LOADER>
template <class MANAGER_TYPE>
inline bool RefCountCatalog<RESOURCE_TYPE, LOADER>::Release(RESOURCE_TYPE * resource, MANAGER_TYPE * manager)
{
	//RefToNameMap Look Up.
	typename RefToNameMap::iterator ref_find_result = ref_map.find(resource);
	if (ref_find_result == ref_map.end())
	{
		return false;
	}
	typename NameToRefMap::iterator name_find_result = ref_find_result->second;
	ResourceRef<RESOURCE_TYPE> & ref = name_find_result->second;

	//Decrement Reference Count.
	ref.DecrementRef();
	if (name_find_result->second.GetRefCount() <= 0)
	{
		loader.Unload(resource, manager);
		delete resource;
		//Erase Entry in RefToNameMap and NameToRefMap.
		ref_map.erase(ref_find_result);
		name_map.erase(name_find_result);
		return true;
	}
	return false;
}

template <class RESOURCE_TYPE, class LOADER>
template <class MANAGER_TYPE>
inline RESOURCE_TYPE * RefCountCatalog<RESOURCE_TYPE, LOADER>::Release(const char * name, MANAGER_TYPE * manager)
{
	//NameToRefMap Look Up.
	typename NameToRefMap::iterator name_find_result = name_map.find(name);
	if (name_find_result == name_map.end())
	{
		return 0;
	}
	ResourceRef<RESOURCE_TYPE> & ref = name_find_result->second;

	//Decrement Reference Count.
	ref.DecrementRef();
	if (ref.GetRefCount() <= 0)
	{
		loader.Unload(ref.GetResource(), manager);
		//Erase Entry in RefToNameMap and NameToRefMap.
		RESOURCE_TYPE * rtn = ref.GetResource();
		ref_map.erase(rtn);
		name_map.erase(name_find_result);
		return rtn;
	}
	return 0;
}

template <class RESOURCE_TYPE, class LOADER>
template <class MANAGER_TYPE>
inline void RefCountCatalog<RESOURCE_TYPE, LOADER>::Clear(MANAGER_TYPE * manager)
{
	for (typename NameToRefMap::iterator it = name_map.begin(); it != name_map.end(); ++it)
	{
		loader.Unload(it->second.GetResource(), manager);
		delete it->second.GetResource();
	}
	name_map.clear();
	ref_map.clear();
}


#endif