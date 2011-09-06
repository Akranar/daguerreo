#ifndef _ASSIMP_SCENE_DATA_H_
#define _ASSIMP_SCENE_DATA_H_

#include <Core_FixedArray.h>
#include <vector>
#include <string>
#include "RendererBaseMesh.h"
#include "ExLib_Skeleton.h"
#include "ExLib_SkeletalAnimation.h"

class AssimpSceneData
{
public:
	AssimpSceneData();
	~AssimpSceneData();
	FixedArray<RendererBaseMesh> mesh_descs;
	std::vector<Skeleton *> skeletons;
	std::vector<SkeletalAnimation *> skeletal_animations;

	void OutputMeshResources(const std::string & filename);
	void OutputSceneGraphResources(const std::string & filename);
	void OuptutAnimationResources(const std::string & filename);

	inline unsigned int GetMeshCount() const;
	inline unsigned int GetSkeletonCount() const;
	inline unsigned int GetAnimationCount() const;

};

inline unsigned int AssimpSceneData::GetMeshCount() const
{
	return mesh_descs.GetSize();
}
inline unsigned int AssimpSceneData::GetSkeletonCount() const
{
	return skeletons.size();
}
inline unsigned int AssimpSceneData::GetAnimationCount() const
{
	return skeletal_animations.size();
}

#endif