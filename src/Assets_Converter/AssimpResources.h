#ifndef _RESOURCE_MAKER_H_
#define _RESOURCE_MAKER_H_

#include "ExLib_Assimp.h"
#include "ExLib_Eigen.h"

#include <set>
#include <string>
#include <map>

class Visual;
class DescVertexFormat;
class DescVertexBuffer;
class DescIndexBuffer;

class Skeleton;
class Joint;
class SkeletalAnimation;

#include "RendererBaseMesh.h"
#include "AssimpSceneData.h"

class AssimpResources
{
	std::set<std::string> bone_names;
	std::set<const aiNode *> skeleton_roots;
	std::map<std::string, const aiBone *> name_to_bone;
	std::map<std::string, unsigned int> name_to_index;

	void InitBoneAndSkeletonRootsRegistry(const aiScene * scene);
	const aiNode * FindNodeWithMesh(const aiScene * scene, const aiMesh * mesh);
	void AddBoneAndParentsToRegistry(const aiScene * scene, const aiNode * bone, const aiNode * root_node);
	bool NodeInBoneRegistry(const char * name);

	bool AllPositionKeysTimeZero(const aiNodeAnim * node_anim);
	bool AllScalingKeysTimeZero(const aiNodeAnim * node_anim);
	bool AllRotationKeysTimeZero(const aiNodeAnim * node_anim);
public:
	//void MakeResourceManager(const std::string & filename, const aiScene * scene, ResourceManager & out_resource_manager);
	//DescVertexFormat * MakeVertexFormat(const aiMesh * current_mesh);
	//DescVertexBuffer * MakeVertexBuffer(const aiMesh * current_mesh, DescVertexFormat * vertex_format);
	//DescIndexBuffer * MakeIndexBuffer(const aiMesh * current_mesh);
	//Visual * MakeVisual(const aiMesh * current_mesh);

	void CreateRendererBaseMesh(const aiMesh * ai_mesh, RendererBaseMesh & base_mesh, unsigned int option_flags);

	Skeleton * MakeSkeleton(const aiScene * scene, const aiNode * skeleton_root);
	SkeletalAnimation * MakeSkeletalAnimation(const aiScene * scene, unsigned int animation_index); 

	void CopyToColumnOrderTransform(const aiMatrix4x4 & in_matrix, Eigen::Affine3f & out_trans);

	void CreateAssimpSceneData(const std::string & filename, const aiScene * scene, AssimpSceneData & scene_data, unsigned int option_flags);
};


#endif