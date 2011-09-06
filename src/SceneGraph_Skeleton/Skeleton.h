#ifndef _SKELETON_H_
#define _SKELETON_H_

#include "ExLib_Serialization.h"
#include "Joint.h"

class Skeleton: public Serializable
{
	std::vector<Joint, Eigen::aligned_allocator<Joint> > joints;
public:
	Skeleton();
	~Skeleton();
	inline const Joint & GetJoint(int index) const;
	inline Joint & GetJoint(int index);
	inline unsigned int GetJointCount() const;
	inline void AddJoint(const Joint & joint);

	void PrintInverseBind() const;
	void PrintDefaultPose() const;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline const Joint & Skeleton::GetJoint(int index) const
{
	return joints[index];
}
inline Joint & Skeleton::GetJoint(int index)
{
	return joints[index];
}
inline unsigned int Skeleton::GetJointCount() const
{
	return joints.size();
}
inline void Skeleton::AddJoint(const Joint & joint)
{
	joints.push_back(joint);
}

#endif