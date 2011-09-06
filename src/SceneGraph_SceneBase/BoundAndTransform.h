#ifndef _BOUND_AND_TRANSFORM_H_
#define _BOUND_AND_TRANSFORM_H_


#include "ExLib_Eigen.h"
#include "ExLib_Serialization.h"

class BoundingVolume;

class BoundAndTransform: public Serializable
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
protected:
	Eigen::Affine3f local_transform;
	Eigen::Affine3f world_transform;
	BoundingVolume * world_bound;
public:
	BoundAndTransform();
	virtual ~BoundAndTransform();
	inline BoundingVolume * GetWorldBound() const;
	inline void SetWorldBound(BoundingVolume * bound);
	inline const Eigen::Affine3f & GetLocalTransform() const;
	inline Eigen::Affine3f & GetLocalTransform();
	inline const Eigen::Affine3f & GetWorldTransform() const;
	inline Eigen::Affine3f & GetWorldTransform();

	inline void SetLocalTransform(const Eigen::Affine3f & trans);
	inline void SetWorldTransform(const Eigen::Affine3f & trans);
	inline void ResetLocalTransform();
	inline void ResetWorldTransform();

	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline BoundingVolume * BoundAndTransform::GetWorldBound() const
{
	return world_bound;
}
inline void BoundAndTransform::SetWorldBound(BoundingVolume * bound)
{
	world_bound = bound;
}

inline const Eigen::Affine3f & BoundAndTransform::GetLocalTransform() const
{
	return local_transform;
}
inline Eigen::Affine3f & BoundAndTransform::GetLocalTransform()
{
	return local_transform;
}
inline const Eigen::Affine3f & BoundAndTransform::GetWorldTransform() const
{
	return world_transform;
}
inline Eigen::Affine3f & BoundAndTransform::GetWorldTransform()
{
	return world_transform;
}

inline void BoundAndTransform::SetLocalTransform(const Eigen::Affine3f & trans)
{
	GetLocalTransform() = trans;
}
inline void BoundAndTransform::SetWorldTransform(const Eigen::Affine3f & trans)
{
	GetWorldTransform() = trans;
}

inline void BoundAndTransform::ResetLocalTransform()
{
	GetLocalTransform() = Eigen::Affine3f::Identity();
}
inline void BoundAndTransform::ResetWorldTransform()
{
	GetWorldTransform() = Eigen::Affine3f::Identity();
}

#endif