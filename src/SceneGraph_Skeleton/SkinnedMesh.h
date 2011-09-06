#ifndef _SKINNED_MESH_INSTANCE_H_
#define _SKINNED_MESH_INSTANCE_H_

#include "ExLib_SceneBase.h"
#include "ExLib_Eigen.h"
#include <vector>

class Skeleton;

class SkinnedMesh: public Spatial
{
	Skeleton * skeleton;
	std::vector<Eigen::Affine3f, Eigen::aligned_allocator<Eigen::Affine3f> > local_pose;
	std::vector<Eigen::Affine3f, Eigen::aligned_allocator<Eigen::Affine3f> > global_pose;
	std::vector<Visual *> skin;
public:
	SkinnedMesh();
	SkinnedMesh(Skeleton * skeleton);
	~SkinnedMesh();
	unsigned int GetJointCount() const;
	inline Skeleton * GetSkeleton() const;
	void SetSkeleton(Skeleton * in_skeleton);
	inline const Eigen::Affine3f & GetLocalPose(int index) const;
	inline Eigen::Affine3f & GetLocalPose(int index);
	inline const Eigen::Affine3f & GetGlobalPose(int index) const;
	inline Eigen::Affine3f & GetGlobalPose(int index);
	inline void ReserveSkinSpace(int mesh_count);
	inline void AddMeshToSkin(Visual * visual);

	virtual void UpdateWorldTransform();
	virtual void UpdateWorldBound();
	virtual void UpdateVisibleSet(Culler * culler, bool culling_on);

	void SetDefaultPose();

	void PrintLocalPose() const;
	void PrintGlobalPose() const;
};

inline Skeleton * SkinnedMesh::GetSkeleton() const
{
	return skeleton;
}

inline const Eigen::Affine3f & SkinnedMesh::GetLocalPose(int index) const
{
	return local_pose[index];
}
inline Eigen::Affine3f & SkinnedMesh::GetLocalPose(int index)
{
	return local_pose[index];
}
inline const Eigen::Affine3f & SkinnedMesh::GetGlobalPose(int index) const
{
	return global_pose[index];
}
inline Eigen::Affine3f & SkinnedMesh::GetGlobalPose(int index)
{
	return global_pose[index];
}
inline void SkinnedMesh::ReserveSkinSpace(int mesh_count)
{
	skin.reserve(mesh_count);
}
inline void SkinnedMesh::AddMeshToSkin(Visual * visual)
{
	skin.push_back(visual);
}

#endif