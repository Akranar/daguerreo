#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include "BoundingVolume.h"
#include "ExLib_Eigen.h"

class BoundingSphere: public BoundingVolume
{
private:
	Eigen::Vector3f center;
	float radius;
public:
	BoundingSphere() {};
	inline float GetRadius() const;
	inline void SetRadius(const float value);
	inline const Eigen::Vector3f & GetCenter() const;
	inline void SetCenter(const Eigen::Vector3f & center);
	
	BoundingVolume::BVType GetType() const;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
	
};

inline float BoundingSphere::GetRadius() const
{
	return this->radius;
}
inline void BoundingSphere::SetRadius(const float radius)
{
	this->radius = radius;
}
inline const Eigen::Vector3f & BoundingSphere::GetCenter() const
{
	return center;
}
inline void BoundingSphere::SetCenter(const Eigen::Vector3f & center)
{
	this->center = center;
}


#endif