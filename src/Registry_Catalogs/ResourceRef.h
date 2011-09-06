#ifndef _RESOURCE_REF_H_
#define _RESOURCE_REF_H_

template<class RESOURCE_TYPE>
class ResourceRef
{
	int ref_count;
	RESOURCE_TYPE * resource;
public:
	inline ResourceRef(RESOURCE_TYPE * resource) : ref_count(0), resource(resource) {};
	inline ResourceRef(unsigned int ref_count, RESOURCE_TYPE * resource) : ref_count(ref_count), resource(resource) {};
	inline int GetRefCount() const;
	inline void IncrementRef();
	inline void DecrementRef();

	inline RESOURCE_TYPE * GetResource() const;
	inline void SetResource(RESOURCE_TYPE * in);
};

template<class RESOURCE_TYPE>
inline int ResourceRef<RESOURCE_TYPE>::GetRefCount() const
{
	return ref_count;
}

template<class RESOURCE_TYPE>
inline void ResourceRef<RESOURCE_TYPE>::IncrementRef()
{
	ref_count += 1;
}
template<class RESOURCE_TYPE>
inline void ResourceRef<RESOURCE_TYPE>::DecrementRef()
{
	ref_count -= 1;
}

template<class RESOURCE_TYPE>
inline RESOURCE_TYPE * ResourceRef<RESOURCE_TYPE>::GetResource() const
{
	return resource;
}
template<class RESOURCE_TYPE>
inline void ResourceRef<RESOURCE_TYPE>::SetResource(RESOURCE_TYPE * in)
{
	resource = in;
}


#endif