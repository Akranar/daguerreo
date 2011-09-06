#ifndef OBB_H
#define OBB_H

#include "BoundingVolume.h"
#include "ExLib_Eigen.h"

class OBB: public BoundingVolume
{
private:
	Eigen::Vector3f center;
	Eigen::Vector3f axes[3];
	Eigen::Vector3f extents;
public:
	OBB() {};
	inline float GetExtent(int index) const;
	inline const Eigen::Vector3f & GetExtents() const;
	inline const Eigen::Vector3f & GetAxis(const int index) const;

	inline void SetAxis(const int index, const Eigen::Vector3f & axis);

	inline void SetExtent(const int index, const float value);
	inline void SetExtent(const Eigen::Vector3f & extents);
	inline const Eigen::Vector3f & GetCenter() const;
	inline void SetCenter(const Eigen::Vector3f & center);

	void GetRotationMatrix(Eigen::Matrix3f & rtn_matrix) const;

	BoundingVolume::BVType GetType() const;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
	
};

inline float OBB::GetExtent(int index) const
{
	return extents[index];
}

inline const Eigen::Vector3f & OBB::GetExtents() const
{
	return extents;
}

inline void OBB::SetExtent(const int index, const float value)
{
	extents[index] = value;
}
inline void OBB::SetExtent(const Eigen::Vector3f & extents)
{
	this->extents = extents;
}
inline const Eigen::Vector3f & OBB::GetCenter() const
{
	return center;
}
inline void OBB::SetCenter(const Eigen::Vector3f & center)
{
	this->center = center;
}

inline const Eigen::Vector3f & OBB::GetAxis(const int index) const
{
	return axes[index];
}

inline void OBB::SetAxis(const int index, const Eigen::Vector3f & axis)
{
	axes[index] = axis.normalized();
}


#endif