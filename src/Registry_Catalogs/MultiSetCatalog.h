#ifndef _MULTI_SET_CATALOG_H_
#define _MULTI_SET_CATALOG_H_

#include <set>

template<class RESOURCE_TYPE, class LOADER>
class MultiSetCatalog
{
public:
	typedef typename std::set<RESOURCE_TYPE* >::iterator CatalogIterator;
private:
	std::set<RESOURCE_TYPE *> set;
	LOADER loader;
public:
	inline unsigned int Size() const;
	inline LOADER & GetLoader();
	inline bool Add(RESOURCE_TYPE * resource);

	inline RESOURCE_TYPE * Find(RESOURCE_TYPE * resource);

	inline CatalogIterator Begin();
	inline CatalogIterator End();

	template <class MANAGER_TYPE>
	inline RESOURCE_TYPE * Retrieve(const char * name, MANAGER_TYPE * manager);

	template <class MANAGER_TYPE>
	inline RESOURCE_TYPE * Release(RESOURCE_TYPE * resource, MANAGER_TYPE * manager);

	template <class MANAGER_TYPE>
	inline void Clear(MANAGER_TYPE * manager);
	
};

template<class RESOURCE_TYPE, class LOADER>
inline unsigned int MultiSetCatalog<RESOURCE_TYPE, LOADER>::Size() const
{
	return set.size();
}

template<class RESOURCE_TYPE, class LOADER>
inline LOADER & MultiSetCatalog<RESOURCE_TYPE, LOADER>::GetLoader()
{
	return loader;
}

template<class RESOURCE_TYPE, class LOADER>
inline bool MultiSetCatalog<RESOURCE_TYPE, LOADER>::Add(RESOURCE_TYPE * resource)
{
	std::pair<CatalogIterator, bool> result = set.insert(resource);
	return result.second;
}

template <class RESOURCE_TYPE, class LOADER>
inline RESOURCE_TYPE * MultiSetCatalog<RESOURCE_TYPE, LOADER>::Find(RESOURCE_TYPE * resource)
{
	//RefToNameMap Look Up.
	typename std::set<RESOURCE_TYPE*>::iterator it = set.find(resource);
	if (it != set.end())
	{
		return *it;
	}
	return 0;
}

template<class RESOURCE_TYPE, class LOADER>
inline typename MultiSetCatalog<RESOURCE_TYPE, LOADER>::CatalogIterator MultiSetCatalog<RESOURCE_TYPE, LOADER>::Begin()
{
	return set.begin();
}

template<class RESOURCE_TYPE, class LOADER>
inline typename MultiSetCatalog<RESOURCE_TYPE, LOADER>::CatalogIterator MultiSetCatalog<RESOURCE_TYPE, LOADER>::End()
{
	return set.end();
}

template<class RESOURCE_TYPE, class LOADER>
template <class MANAGER_TYPE>
inline RESOURCE_TYPE * MultiSetCatalog<RESOURCE_TYPE, LOADER>::Retrieve(const char * name, MANAGER_TYPE * manager)
{
	//Load Resource.
	RESOURCE_TYPE * resource = new RESOURCE_TYPE();
	loader.Load(name, manager, resource);
	//Add Resource.
	Add(resource);
	return resource;
}

template<class RESOURCE_TYPE, class LOADER>
template <class MANAGER_TYPE>
inline RESOURCE_TYPE * MultiSetCatalog<RESOURCE_TYPE, LOADER>::Release(RESOURCE_TYPE * resource, MANAGER_TYPE * manager)
{
	loader.Unload(resource, manager);
	if (set.erase(resource))
	{
		//return resource;
	}
	delete resource;
	return 0;
}

template<class RESOURCE_TYPE, class LOADER>
template <class MANAGER_TYPE>
inline void MultiSetCatalog<RESOURCE_TYPE, LOADER>::Clear(MANAGER_TYPE * manager)
{
	for (typename std::set<RESOURCE_TYPE *>::iterator it = set.begin(); it != set.end(); ++it)
	{
		loader.Unload(*it, manager);
		delete *it;
	}
	set.clear();
}


#endif