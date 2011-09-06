#include "SkinnedMesh.h"
#include "Skeleton.h"

#include <iostream>

SkinnedMesh::SkinnedMesh()
{

}

SkinnedMesh::SkinnedMesh(Skeleton * skeleton)
:
skeleton(skeleton)
{
	const unsigned int joint_count = skeleton->GetJointCount();
	local_pose.reserve(joint_count);
	global_pose.reserve(joint_count);

	for (unsigned int i = 0; i < joint_count; ++i)
	{
		local_pose.push_back(Eigen::Affine3f(Eigen::Affine3f::Identity()));
		global_pose.push_back(Eigen::Affine3f(Eigen::Affine3f::Identity()));
	}
}

SkinnedMesh::~SkinnedMesh()
{

}

unsigned int SkinnedMesh::GetJointCount() const
{
	return skeleton->GetJointCount();
}

void SkinnedMesh::SetSkeleton(Skeleton * in_skeleton)
{
	skeleton = in_skeleton;
	local_pose.clear();
	global_pose.clear();
	const unsigned int joint_count = skeleton->GetJointCount();
	local_pose.reserve(joint_count);
	global_pose.reserve(joint_count);

	for (unsigned int i = 0; i < joint_count; ++i)
	{
		local_pose.push_back(Eigen::Affine3f(Eigen::Affine3f::Identity()));
		global_pose.push_back(Eigen::Affine3f(Eigen::Affine3f::Identity()));
	}
}

void SkinnedMesh::SetDefaultPose()
{
	const unsigned int count = local_pose.size();
	global_pose[0].matrix().noalias() = GetWorldTransform().matrix() * GetLocalTransform().matrix();
	for (unsigned int i = 1; i < count; ++i)
	{
		GetLocalPose(i) = skeleton->GetJoint(i).GetDefaultPose();//temp
	}
}


void SkinnedMesh::UpdateWorldTransform()
{
	ConcatParentTransform();

	const unsigned int count = local_pose.size();
	//GetWorldTransform().matrix().noalias() = parent->GetWorldTransform().matrix() * GetLocalTransform().matrix();
	GetGlobalPose(0).matrix().noalias() = GetWorldTransform().matrix() * GetLocalPose(0).matrix();
	for (unsigned int i = 1; i < count; ++i)
	{
		GetGlobalPose(i).matrix().noalias() = GetGlobalPose(skeleton->GetJoint(i).GetParentIndex()) * GetLocalPose(i).matrix();
	}

	UpdateWorldBound();
}
void SkinnedMesh::UpdateWorldBound()
{
	
}

void SkinnedMesh::UpdateVisibleSet(Culler * culler, bool culling_on)
{
	if (GetCullMode() == Spatial::CULL_ALWAYS) return;
	if (GetCullMode() == Spatial::CULL_NEVER) culling_on = true;
	if (culling_on && !culler->IsVisible(GetWorldBound())) return;
	
	const unsigned int skin_count = skin.size();
	for (unsigned int i = 0; i < skin_count; ++i)
	{
		culler->GetVisibleSet()->Insert(VisibleObject(skin[i]));
	}
	
}

void SkinnedMesh::PrintLocalPose() const
{
	for (unsigned int i = 0; i < local_pose.size(); ++i)
	{
		std::cout << local_pose[i].matrix() << std::endl << std::endl;
	}
}

void SkinnedMesh::PrintGlobalPose() const
{
	for (unsigned int i = 0; i < global_pose.size(); ++i)
	{
		std::cout << global_pose[i].matrix() << std::endl << std::endl;
	}
}