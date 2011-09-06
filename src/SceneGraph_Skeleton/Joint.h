#ifndef _JOINT_H_
#define _JOINT_H_

#include "ExLib_Eigen.h"
#include "ExLib_Serialization.h"

class Joint: public Serializable
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
protected:
	Eigen::Affine3f inverse_bind_matrix;
	Eigen::Affine3f default_local_pose;
	unsigned char parent_index;
public:
	Joint();
	Joint(unsigned char parent_index);
	virtual ~Joint();
	inline const Eigen::Affine3f & GetInverseBindMatrix() const;
	inline Eigen::Affine3f & GetInverseBindMatrix();
	inline const Eigen::Affine3f & GetDefaultPose() const;
	inline Eigen::Affine3f & GetDefaultPose();
	inline unsigned char GetParentIndex() const;
	inline void SetInverseBindMatrix(const Eigen::Affine3f & data);
	inline void SetParentIndex(unsigned char index);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline const Eigen::Affine3f & Joint::GetInverseBindMatrix() const
{
	return inverse_bind_matrix;
}
inline Eigen::Affine3f & Joint::GetInverseBindMatrix()
{
	return inverse_bind_matrix;
}
inline const Eigen::Affine3f & Joint::GetDefaultPose() const
{
	return default_local_pose;
}
inline Eigen::Affine3f & Joint::GetDefaultPose()
{
	return default_local_pose;
}
inline unsigned char Joint::GetParentIndex() const
{
	return parent_index;
}
inline void Joint::SetInverseBindMatrix(const Eigen::Affine3f & data)
{
	inverse_bind_matrix = data;
}
inline void Joint::SetParentIndex(unsigned char index)
{
	parent_index = index;
}

#endif