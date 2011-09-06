#ifndef AABB_H
#define AABB_H

#include "BoundingVolume.h"
#include "ExLib_Eigen.h"

class FloatDataAccessor;

class AABB: public BoundingVolume
{
private:
	Eigen::Vector3f center;
	Eigen::Vector3f extents;
public:
	AABB() {};
	inline const Eigen::Vector3f & GetCenter() const;
	inline float GetExtent(int index) const;
	inline const Eigen::Vector3f & GetExtents() const;

	inline void SetExtent(const int index, const float value);
	inline void SetExtents(const Eigen::Vector3f & extents);
	inline void SetCenter(const Eigen::Vector3f & center);

	//Convenience
	inline void GetMins(Eigen::Vector3f & rtn_min);
	inline void GetMaxes(Eigen::Vector3f & rtn_max);

	void GetVertices(FloatDataAccessor & accessor) const;
	void GetIBufferLines(unsigned char * ibuffer) const;
	void GetIBufferTriangles(unsigned char * rtn_ibuffer) const;
	unsigned int GetIBufferLinesCount() const;
	unsigned int GetIBufferTrianglesCount() const;

	BoundingVolume::BVType GetType() const;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
	
};

inline void AABB::GetMins(Eigen::Vector3f & rtn_min)
{
	rtn_min = GetCenter() - GetExtents();
}

inline void AABB::GetMaxes(Eigen::Vector3f & rtn_max)
{
	rtn_max = GetCenter() + GetExtents();
}



inline float AABB::GetExtent(int index) const
{
	return extents[index];
}

inline const Eigen::Vector3f & AABB::GetExtents() const
{
	return extents;
}

inline const Eigen::Vector3f & AABB::GetCenter() const
{
	return center;
}
inline void AABB::SetExtent(const int index, const float value)
{
	extents[index] = value;
}
inline void AABB::SetExtents(const Eigen::Vector3f & extents)
{
	this->extents = extents;
}
inline void AABB::SetCenter(const Eigen::Vector3f & center)
{
	this->center = center;
}


#endif