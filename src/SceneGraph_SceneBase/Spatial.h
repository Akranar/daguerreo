#ifndef _SPATIAL_H_
#define _SPATIAL_H_

#include <vector>
#include "BoundAndTransform.h"
#include "ExLib_NTree.h"

class Culler;

class Spatial: public NTreeWithParent<BoundAndTransform>
{
public:
	enum CullMode
	{
		CULL_ALWAYS,
		CULL_NEVER,
		CULL_DYNAMIC,
	};
protected:
	CullMode cull_mode;
public:
	Spatial();
	virtual ~Spatial();
	inline Spatial::CullMode GetCullMode() const;
	inline void GetCullMode(Spatial::CullMode mode);

	inline void ConcatParentTransform();
	inline void SetLocalSQT(const Eigen::Vector3f & scale, const Eigen::Quaternionf & rot, const Eigen::Vector3f & trans);
	inline void SetLocalRotationAndScale(const Eigen::Quaternionf & rot, const Eigen::Vector3f & scale);
	inline void SetLocalTranslation(const Eigen::Vector3f & trans);
	inline void SetLocalRotation(const Eigen::Quaternionf & rot);
	inline void SetLocalScale(const Eigen::Vector3f & scale);
	inline void LocalMakeAffine();
	inline void SetLocalIdentity();

	//SceneGraph Main Update Functions
	virtual void GeometricStateUpdate();
	virtual void GetVisibleSet(Culler * culler);

	//SceneGraph Helper Functions
	virtual void UpdateWorldTransform();
	virtual void UpdateWorldBound();

	virtual void UpdateVisibleSet(Culler * culler, bool culling_on);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

inline Spatial::CullMode Spatial::GetCullMode() const
{
	return cull_mode;
}
inline void Spatial::GetCullMode(CullMode mode)
{
	cull_mode = mode;
}

inline void Spatial::ConcatParentTransform()
{
	if (parent)
	{
		GetWorldTransform().matrix().noalias() = parent->GetWorldTransform().matrix() * GetLocalTransform().matrix();
		return;
	}
	SetWorldTransform(GetLocalTransform());
}

inline void Spatial::SetLocalSQT(const Eigen::Vector3f & scale, const Eigen::Quaternionf & rot, const Eigen::Vector3f & trans)
{
	GetLocalTransform().linear().noalias() =  rot * scale.asDiagonal();
	GetLocalTransform().translation() = trans;
}

inline void Spatial::SetLocalRotationAndScale(const Eigen::Quaternionf & rot, const Eigen::Vector3f & scale)
{
	GetLocalTransform().linear().noalias() =  rot * scale.asDiagonal();
}
inline void Spatial::SetLocalTranslation(const Eigen::Vector3f & trans)
{
	GetLocalTransform().translation() = trans;
}
inline void Spatial::SetLocalRotation(const Eigen::Quaternionf & rot)
{
	GetLocalTransform().linear().noalias() = rot.matrix();
}
inline void Spatial::SetLocalScale(const Eigen::Vector3f & scale)
{
	GetLocalTransform().linear().noalias() = Eigen::Matrix3f::Identity() * scale.asDiagonal();
}
inline void Spatial::LocalMakeAffine()
{
	GetLocalTransform().makeAffine();
}

inline void Spatial::SetLocalIdentity()
{
	GetLocalTransform().setIdentity();
}
#endif