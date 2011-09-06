#include "AssimpResources.h"
#include <Core_Exception.h>
#include "ExLib_FloatDataAccessor.h"
#include "ExLib_SceneBase.h"
#include "ExLib_Skeleton.h"
#include "ExLib_SkeletalAnimation.h"

#include <sstream>

#include <set>
#include <string>
#include <stack>
#include <vector>
#include <iostream>


void AssimpResources::CopyToColumnOrderTransform(const aiMatrix4x4 & in_matrix, Eigen::Affine3f & out_trans)
{
	out_trans.matrix()(0, 0) = in_matrix.a1;
	out_trans.matrix()(0, 1) = in_matrix.a2;
	out_trans.matrix()(0, 2) = in_matrix.a3;
	out_trans.matrix()(0, 3) = in_matrix.a4;

	out_trans.matrix()(1, 0) = in_matrix.b1;
	out_trans.matrix()(1, 1) = in_matrix.b2;
	out_trans.matrix()(1, 2) = in_matrix.b3;
	out_trans.matrix()(1, 3) = in_matrix.b4;

	out_trans.matrix()(2, 0) = in_matrix.c1;
	out_trans.matrix()(2, 1) = in_matrix.c2;
	out_trans.matrix()(2, 2) = in_matrix.c3;
	out_trans.matrix()(2, 3) = in_matrix.c4;

	out_trans.matrix()(3, 0) = in_matrix.d1;
	out_trans.matrix()(3, 1) = in_matrix.d2;
	out_trans.matrix()(3, 2) = in_matrix.d3;
	out_trans.matrix()(3, 3) = in_matrix.d4;
}



const aiNode * AssimpResources::FindNodeWithMesh(const aiScene * scene, const aiMesh * mesh)
{
	std::stack<const aiNode *> ai_stack;
	ai_stack.push(scene->mRootNode);

	while (!ai_stack.empty())
	{
		const aiNode * current_node = ai_stack.top();
		ai_stack.pop();

		for (unsigned int i = 0; i < current_node->mNumMeshes; ++i)
		{
			if (scene->mMeshes[current_node->mMeshes[i]] == mesh)
			{
				return current_node;
			}
		}

		for (unsigned int i = 0; i < current_node->mNumChildren; ++i)
		{
			ai_stack.push(current_node->mChildren[i]);
		}
	}
	return 0;
}

bool AssimpResources::NodeInBoneRegistry(const char * name)
{
	return (bone_names.find(name) != bone_names.end());
}

void AssimpResources::AddBoneAndParentsToRegistry(const aiScene * scene, const aiNode * bone, const aiNode * root_node)
{
	const aiNode * current = bone;
	while (current != root_node && current != root_node->mParent)
	{
		bone_names.insert(current->mName.data);
		current = current->mParent;
	}
	bone_names.insert(current->mName.data);
	skeleton_roots.insert(current);
}

Skeleton * AssimpResources::MakeSkeleton(const aiScene * scene, const aiNode * root_node)
{
	Skeleton * skeleton = new Skeleton();
	skeleton->AddJoint(Joint()); //Add root node.

	name_to_index.insert(std::make_pair(root_node->mName.data, 0)); //Map root node's name to index 0.
	std::map<std::string, const aiBone *>::iterator it = name_to_bone.find(root_node->mName.data); //Find the root node in the hierarchy.
	if (it != name_to_bone.end())
	{
		const aiBone * ai_bone = it->second;
		CopyToColumnOrderTransform(ai_bone->mOffsetMatrix, skeleton->GetJoint(skeleton->GetJointCount()-1).GetInverseBindMatrix()); //Get Root node's InverseBindMatrix.
		CopyToColumnOrderTransform(root_node->mTransformation, skeleton->GetJoint(skeleton->GetJointCount()-1).GetDefaultPose()); //Get Root node's DefaultPose.
	}
	else
	{
		skeleton->GetJoint(skeleton->GetJointCount()-1).GetInverseBindMatrix().setIdentity(); //Set Root node's InverseBind to Identity.
		CopyToColumnOrderTransform(root_node->mTransformation, skeleton->GetJoint(skeleton->GetJointCount()-1).GetDefaultPose()); //Get Root node's DefaultPose.
	}

	std::stack<const aiNode *> ai_stack;
	std::stack<unsigned char> joint_stack;

	ai_stack.push(root_node);
	joint_stack.push(0);

	while (!ai_stack.empty())
	{
		const aiNode * current_node = ai_stack.top();
		ai_stack.pop();

		unsigned char parent_index = joint_stack.top();
		joint_stack.pop();

		for (unsigned int i = 0; i < current_node->mNumChildren; ++i)
		{
			const aiNode * ai_current_child = current_node->mChildren[i];

			if (bone_names.find(ai_current_child->mName.data) != bone_names.end())
			{
				skeleton->AddJoint(Joint(parent_index));
				name_to_index.insert(std::make_pair(ai_current_child->mName.data, skeleton->GetJointCount()-1));
				Joint & current_joint = skeleton->GetJoint(skeleton->GetJointCount()-1);

				std::map<std::string, const aiBone *>::iterator it = name_to_bone.find(ai_current_child->mName.data);
				if (it != name_to_bone.end())
				{
					const aiBone * ai_bone = it->second;
					CopyToColumnOrderTransform(ai_bone->mOffsetMatrix, current_joint.GetInverseBindMatrix());
					CopyToColumnOrderTransform(ai_current_child->mTransformation, current_joint.GetDefaultPose());
					
				}
				else
				{
					current_joint.GetInverseBindMatrix().setIdentity();
					CopyToColumnOrderTransform(ai_current_child->mTransformation, current_joint.GetDefaultPose());
				}
				
				ai_stack.push(ai_current_child);
				joint_stack.push(skeleton->GetJointCount()-1);
			}
		}
	}
	return skeleton;
}


bool AssimpResources::AllPositionKeysTimeZero(const aiNodeAnim * node_anim)
{
	for (unsigned int i = 0; i < node_anim->mNumPositionKeys; ++i)
	{
		double time = node_anim->mPositionKeys[i].mTime;
		if (time != 0)
		{
			//return false;
		}
	}
	return true;
}

bool AssimpResources::AllScalingKeysTimeZero(const aiNodeAnim * node_anim)
{
	for (unsigned int i = 0; i < node_anim->mNumScalingKeys; ++i)
	{
		double time = node_anim->mScalingKeys[i].mTime;
		if (time != 0)
		{
			//return false;
		}
	}
	return true;
}
bool AssimpResources::AllRotationKeysTimeZero(const aiNodeAnim * node_anim)
{
	for (unsigned int i = 0; i < node_anim->mNumRotationKeys; ++i)
	{
		double time = node_anim->mRotationKeys[i].mTime;
		if (time != 0)
		{
			//return false;
		}
	}
	return true;
}

SkeletalAnimation * AssimpResources::MakeSkeletalAnimation(const aiScene * scene, unsigned int animation_index)
{
	const aiAnimation * animation = scene->mAnimations[animation_index];
	
	double length_in_seconds = animation->mTicksPerSecond ? (animation->mDuration / animation->mTicksPerSecond) : animation->mDuration;
	unsigned int node_channels_count = animation->mNumChannels;

	Uncompressed_SkeletalAnimation * skeletal_animation = new Uncompressed_SkeletalAnimation(length_in_seconds, node_channels_count);

	for (unsigned int channel_index = 0; channel_index < animation->mNumChannels; ++channel_index)
	{
		const aiNodeAnim * node_anim = animation->mChannels[channel_index];
		bool position_keys_zero = AllPositionKeysTimeZero(node_anim);
		bool scaling_keys_zero = AllScalingKeysTimeZero(node_anim);
		bool rotation_keys_zero = AllRotationKeysTimeZero(node_anim);
		std::map<std::string, unsigned int>::iterator it = name_to_index.find(node_anim->mNodeName.data);
		if (it == name_to_index.end())
		{
			std::cout << "Could not find bone index." << std::endl;
			throw 1;
		}

		unsigned char index_in_skeleton = it->second;
		int pre_state = node_anim->mPreState;
		int post_state = node_anim->mPostState;
		//aiAnimBehaviour_DEFAULT
		//aiAnimBehaviour_CONSTANT
		//aiAnimBehaviour_LINEAR
		//aiAnimBehaviour_REPEAT

		Uncompressed_JointChannel & joint_channel = skeletal_animation->GetJointChannel(channel_index);
		joint_channel.InitJointChannel(index_in_skeleton, node_anim->mNumScalingKeys, node_anim->mNumRotationKeys, node_anim->mNumPositionKeys);
		joint_channel.SetBehavior(pre_state, post_state);

		for (unsigned int scale_index = 0; scale_index < node_anim->mNumScalingKeys; ++scale_index)
		{
			aiVectorKey scale_key = node_anim->mScalingKeys[scale_index];
			//double time = scale_key.mTime;
			double time = (scale_key.mTime / animation->mTicksPerSecond);
			Eigen::Vector3f scaling = Eigen::Vector3f(scale_key.mValue.x, scale_key.mValue.y, scale_key.mValue.z);
			joint_channel.SetScale(scale_index, ScaleKey(time, scaling));
		}
		for (unsigned int rotation_index = 0; rotation_index < node_anim->mNumRotationKeys; ++rotation_index)
		{
			aiQuatKey rotation_key = node_anim->mRotationKeys[rotation_index];
			aiQuaternion rotation_value = rotation_key.mValue.Normalize();
			//double time = rotation_key.mTime;
			double time = (rotation_key.mTime / animation->mTicksPerSecond);
			Eigen::Quaternionf rotation = Eigen::Quaternionf(rotation_value.w, rotation_value.x, rotation_value.y, rotation_value.z); //Eigen::Quaternion constructor takes w first.
			joint_channel.SetRotation(rotation_index, RotationKey(time, rotation));
		}
		for (unsigned int position_index = 0; position_index < node_anim->mNumPositionKeys; ++position_index)
		{
			aiVectorKey position_key = node_anim->mPositionKeys[position_index];
			//double time = position_key.mTime;
			double time =  (position_key.mTime / animation->mTicksPerSecond);
			Eigen::Vector3f translation = Eigen::Vector3f(position_key.mValue.x, position_key.mValue.y, position_key.mValue.z);
			joint_channel.SetTranslation(position_index, TranslationKey(time, translation));
		}
	}

	return skeletal_animation;
}


void AssimpResources::InitBoneAndSkeletonRootsRegistry(const aiScene * scene)
{
	bone_names.clear();
	skeleton_roots.clear();
	for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
	{
		const aiMesh * mesh = scene->mMeshes[i];
		const aiNode * node_of_mesh = FindNodeWithMesh(scene, mesh);
		for (unsigned int b = 0; b < mesh->mNumBones; ++b)
		{
			name_to_bone.insert(std::make_pair(mesh->mBones[b]->mName.data, mesh->mBones[b]));
			AddBoneAndParentsToRegistry(scene, scene->mRootNode->FindNode(mesh->mBones[b]->mName.data), node_of_mesh);
		}
	}
}

void AssimpResources::CreateRendererBaseMesh(const aiMesh * ai_mesh, RendererBaseMesh & base_mesh, unsigned int option_flags)
{
	bool get_blend_data = ai_mesh->HasBones();
	unsigned int max_bones_per_vertex = 4;

	unsigned int position_channels = ai_mesh->HasPositions() ? 3 : 0;
	unsigned int normal_channels = ai_mesh->HasNormals() ? 3 : 0;
	unsigned int tangent_channels = 0;
	unsigned int binormal_channels = 0;
	std::vector<unsigned int> texture_units;
	std::vector<unsigned int> texture_channels;
	unsigned int color_channels = 0;
	unsigned int secondary_color_channels = 0;
	unsigned int blend_index_channels = get_blend_data ? max_bones_per_vertex : 0;
	unsigned int blend_weight_channels = blend_index_channels;

	unsigned int attrib_index_of_blendindices;
	unsigned int attrib_index_of_blendweights;

	{//Set above data from ai_mesh. BEGIN
		if (ai_mesh->HasTangentsAndBitangents())
		{
			tangent_channels = 3;
			binormal_channels = 3;
		}
		
		const unsigned int tex_unit_count = ai_mesh->GetNumUVChannels();
		for (unsigned int tex_unit = 0; tex_unit < tex_unit_count; ++tex_unit)
		{
			if (ai_mesh->HasTextureCoords(tex_unit))
			{
				unsigned int tex_unit_channels = ai_mesh->mNumUVComponents[tex_unit];
				texture_channels.push_back(tex_unit_channels);
				texture_units.push_back(tex_unit);
			}
		}
		
		const unsigned int color_unit_count = ai_mesh->GetNumColorChannels();
		for (unsigned int color_unit = 0; color_unit < color_unit_count; ++color_unit)
		{
			if (color_unit == 0 && ai_mesh->HasVertexColors(color_unit))
			{
				color_channels = 4;
			}
			if (color_unit == 1 && ai_mesh->HasVertexColors(color_unit))
			{
				secondary_color_channels = 4;
			}
		}
	}//Set above data from ai_mesh. END

	unsigned int attrib_count = 
		(position_channels ? 1 : 0) + 
		(normal_channels ? 1 : 0) + 
		(tangent_channels ? 1 : 0) + 
		(binormal_channels ? 1 : 0) + 
		(color_channels ? 1 : 0) + 
		(secondary_color_channels ? 1 : 0) +
		texture_channels.size() + 
		(blend_index_channels ? 1 : 0) + 
		(blend_weight_channels ? 1 : 0);

	unsigned int total_channels = 
		position_channels +
		normal_channels +
		tangent_channels + 
		binormal_channels +
		color_channels +
		secondary_color_channels + 
		blend_index_channels + 
		blend_weight_channels;

	for (unsigned int i = 0; i < texture_channels.size(); ++i)
	{
		total_channels += texture_channels[i];
	}

	base_mesh.GetFormatDesc().Initialize(attrib_count);
	unsigned int attrib_index = 0;
	unsigned int offset = 0;
	unsigned int byte_stride = total_channels * sizeof(float);
	
	if (position_channels)
	{
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(position_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("POSITION");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += position_channels * sizeof(float);
	}

	if (normal_channels)
	{
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(normal_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("NORMAL");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += normal_channels * sizeof(float);
	}

	if (tangent_channels)
	{
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(normal_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("TANGENT");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += normal_channels * sizeof(float);
	}

	if (binormal_channels)
	{
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(binormal_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("BINORMAL");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += binormal_channels * sizeof(float);
	}

	if (color_channels)
	{
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(color_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("COLOR");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += color_channels * sizeof(float);
	}

	if (secondary_color_channels)
	{
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(secondary_color_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("COLOR");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(1);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += secondary_color_channels * sizeof(float);
	}

	for (unsigned int i = 0; i < texture_channels.size(); ++i)
	{
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(texture_channels[i]);
		base_mesh.GetFormatDesc()[attrib_index].SetName("TEXCOORD");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(i);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += texture_channels[i] * sizeof(float);
	}

	if (blend_index_channels)
	{
		attrib_index_of_blendindices = attrib_index;
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(blend_index_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("BLENDINDICES");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += blend_index_channels * sizeof(float);	
	}

	if (blend_weight_channels)
	{
		attrib_index_of_blendweights = attrib_index;
		base_mesh.GetFormatDesc()[attrib_index].SetChannels(blend_weight_channels);
		base_mesh.GetFormatDesc()[attrib_index].SetName("BLENDWEIGHT");
		base_mesh.GetFormatDesc()[attrib_index].SetSemanticIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetOffset(offset);
		base_mesh.GetFormatDesc()[attrib_index].SetStreamIndex(0);
		base_mesh.GetFormatDesc()[attrib_index].SetStride(byte_stride);
		base_mesh.GetFormatDesc()[attrib_index].SetType(DT_FLOAT); //FIX ME LATER.
		attrib_index += 1;
		offset += blend_weight_channels * sizeof(float);
	}

	//VERTEX DATA
	
	
	base_mesh.GetVertexData().Set(ai_mesh->mNumVertices * byte_stride);

	attrib_index = 0; //Reset index;
	if (position_channels)
	{
		float * dest_float_data = (float *) (base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index].GetOffset());
		for (unsigned int vertex = 0; vertex < ai_mesh->mNumVertices; ++vertex)
		{
			const aiVector3D & src_vector = ai_mesh->mVertices[vertex];
			dest_float_data[0] = src_vector[0];
			dest_float_data[1] = src_vector[1];
			dest_float_data[2] = src_vector[2];
			
			dest_float_data = (float *) (((unsigned char *) dest_float_data) + byte_stride);
		}
		attrib_index += 1;
	}

	if (normal_channels)
	{
		float * dest_float_data = (float *) (base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index].GetOffset());
		for (unsigned int vertex = 0; vertex < ai_mesh->mNumVertices; ++vertex)
		{
			const aiVector3D & src_vector = ai_mesh->mNormals[vertex];
			dest_float_data[0] = src_vector[0];
			dest_float_data[1] = src_vector[1];
			dest_float_data[2] = src_vector[2];
			
			dest_float_data = (float *) (((unsigned char *) dest_float_data) + byte_stride);
		}
		attrib_index += 1;
	}

	if (tangent_channels)
	{
		float * dest_float_data = (float *) (base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index].GetOffset());
		for (unsigned int vertex = 0; vertex < ai_mesh->mNumVertices; ++vertex)
		{
			const aiVector3D & src_vector = ai_mesh->mTangents[vertex];
			dest_float_data[0] = src_vector[0];
			dest_float_data[1] = src_vector[1];
			dest_float_data[2] = src_vector[2];
			
			dest_float_data = (float *) (((unsigned char *) dest_float_data) + byte_stride);
		}
		attrib_index += 1;
	}

	if (binormal_channels)
	{
		float * dest_float_data = (float *) (base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index].GetOffset());
		for (unsigned int vertex = 0; vertex < ai_mesh->mNumVertices; ++vertex)
		{
			const aiVector3D & src_vector = ai_mesh->mBitangents[vertex];
			dest_float_data[0] = src_vector[0];
			dest_float_data[1] = src_vector[1];
			dest_float_data[2] = src_vector[2];
			
			dest_float_data = (float *) (((unsigned char *) dest_float_data) + byte_stride);
		}
		attrib_index += 1;
	}

	if (color_channels)
	{
		float * dest_float_data = (float *) (base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index].GetOffset());
		for (unsigned int vertex = 0; vertex < ai_mesh->mNumVertices; ++vertex)
		{
			const aiColor4D & src_vector = ai_mesh->mColors[vertex][0];
			dest_float_data[0] = src_vector[0];
			dest_float_data[1] = src_vector[1];
			dest_float_data[2] = src_vector[2];
			dest_float_data[3] = src_vector[3];
			
			dest_float_data = (float *) (((unsigned char *) dest_float_data) + byte_stride);
		}
		attrib_index += 1;
	}

	if (secondary_color_channels)
	{
		float * dest_float_data = (float *) (base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index].GetOffset());
		for (unsigned int vertex = 0; vertex < ai_mesh->mNumVertices; ++vertex)
		{
			const aiColor4D & src_vector = ai_mesh->mColors[vertex][1];
			dest_float_data[0] = src_vector[0];
			dest_float_data[1] = src_vector[1];
			dest_float_data[2] = src_vector[2];
			dest_float_data[3] = src_vector[3];
			
			dest_float_data = (float *) (((unsigned char *) dest_float_data) + byte_stride);
		}
		attrib_index += 1;
	}

	for (unsigned int i = 0; i < texture_channels.size(); ++i)
	{
		for (unsigned int tex_unit = 0; tex_unit < texture_units.size(); ++tex_unit)
		{
			unsigned int offset = base_mesh.GetFormatDesc()[attrib_index].GetOffset();
			float * dest_float_data = (float *) (base_mesh.GetVertexData().GetData() + offset);
			for (unsigned int vertex = 0; vertex < ai_mesh->mNumVertices; ++vertex)
			{
				const aiVector3D & src_vector = ai_mesh->mTextureCoords[texture_units[tex_unit]][vertex];
				for (unsigned int channel = 0; channel < texture_channels[i]; ++channel)
				{
					dest_float_data[channel] = src_vector[channel];
				}
				
				dest_float_data = (float *) (((unsigned char *) dest_float_data) + byte_stride);
			}
			attrib_index += 1;
		}
	}

	if (get_blend_data)
	{
		std::vector<std::vector<std::pair<float, float> > > weights_by_vertices(ai_mesh->mNumVertices, std::vector<std::pair<float, float> >());
		unsigned char * blendindex_data_start = base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index_of_blendindices].GetOffset();
		unsigned char * blendweight_data_start = base_mesh.GetVertexData().GetData() + base_mesh.GetFormatDesc()[attrib_index_of_blendweights].GetOffset();
		for (unsigned int bone = 0; bone < ai_mesh->mNumBones; ++bone)
		{
			aiString & bone_name = ai_mesh->mBones[bone]->mName;
			std::map<std::string, unsigned int>::iterator find_result = name_to_index.find(bone_name.data);
			CONDITIONAL_EXCEPTION(find_result == name_to_index.end(), "Could not get bone index from bone name.");
			unsigned int bone_index = find_result->second;
			for (unsigned int vertex_weight_index = 0; vertex_weight_index < ai_mesh->mBones[bone]->mNumWeights; ++vertex_weight_index)
			{
				aiVertexWeight & vertex_weight = ai_mesh->mBones[bone]->mWeights[vertex_weight_index];
				weights_by_vertices[vertex_weight.mVertexId].push_back(std::make_pair(float(bone_index), vertex_weight.mWeight));
			}
			
		}

		for (unsigned int i = 0; i < weights_by_vertices.size(); ++i)
		{
			while (weights_by_vertices[i].size() < max_bones_per_vertex)
			{
				weights_by_vertices[i].push_back(std::make_pair(0, 0));
			}
		}
		for (unsigned int i = 0; i < weights_by_vertices.size(); ++i)
		{
			for (unsigned int j = 0; j < max_bones_per_vertex; ++j)
			{
				((float *)(blendindex_data_start + (i * byte_stride)))[j] = weights_by_vertices[i][j].first;
				((float *)(blendweight_data_start + (i * byte_stride)))[j] = weights_by_vertices[i][j].second;
			}
		}
		std::set<float> indices;
		for (unsigned int i = 0; i < weights_by_vertices.size(); ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				indices.insert(weights_by_vertices[i][j].first);
			}
		}
		int wtf = 0;
	}
	
	base_mesh.GetVertexDesc().SetByteCount(ai_mesh->mNumVertices * byte_stride);
	base_mesh.GetVertexDesc().SetStride(byte_stride);
	base_mesh.GetVertexDesc().SetUsage(BUFFER_USAGE_STATIC);
	base_mesh.GetVertexDesc().SetInitialDataPointer(base_mesh.GetVertexData().GetData());

	//INDEX DATA
	base_mesh.GetIndexData().Set(ai_mesh->mNumFaces * 3 * sizeof(unsigned int));
	unsigned int * dest_index_data = (unsigned int *) base_mesh.GetIndexData().GetData();
	unsigned int data_index = 0;
	for (unsigned int face = 0; face < ai_mesh->mNumFaces; ++face)
	{
		const aiFace & current_face = ai_mesh->mFaces[face];
		CONDITIONAL_EXCEPTION(current_face.mNumIndices != 3, "Face Is Not Triangle");
		for (unsigned int index = 0; index < current_face.mNumIndices; ++index)
		{
			unsigned int current_index = current_face.mIndices[index];
			dest_index_data[data_index] = current_index;
			data_index += 1;
		}
	}

	base_mesh.GetIndexDesc().SetByteCount(ai_mesh->mNumFaces * 3 * sizeof(unsigned int));
	base_mesh.GetIndexDesc().SetUsage(BUFFER_USAGE_STATIC);
	base_mesh.GetIndexDesc().SetPrimitiveType(PRIMITIVE_TRIANGLE_LIST);
	base_mesh.GetIndexDesc().SetInitialDataPointer(base_mesh.GetIndexData().GetData());
	
}


void AssimpResources::CreateAssimpSceneData(const std::string & filename, const aiScene * ai_scene, AssimpSceneData & scene_data, unsigned int option_flags)
{
	if (ai_scene)
	{
		if (ai_scene->HasMeshes())
		{
			InitBoneAndSkeletonRootsRegistry(ai_scene);
			int skeleton_num = 0;
			for (std::set<const aiNode *>::iterator it = skeleton_roots.begin(); it != skeleton_roots.end(); ++it)
			{
				Skeleton * skeleton = MakeSkeleton(ai_scene, *it);
				scene_data.skeletons.push_back(skeleton);
				std::stringstream skeleton_name;
				skeleton_name << filename << '_' << skeleton_num;
				skeleton_num += 1;
			}

			int skeletal_animation_num = 0;
			for (unsigned int anim = 0; anim < ai_scene->mNumAnimations; ++anim)
			{
				SkeletalAnimation * skeletal_animation = MakeSkeletalAnimation(ai_scene, anim);
				scene_data.skeletal_animations.push_back(skeletal_animation);
				std::stringstream anim_name;
				anim_name << filename << '_' << skeletal_animation_num;
				skeletal_animation_num += 1;
			}


			std::vector<RendererBaseMesh *> mesh_with_bones;
			scene_data.mesh_descs.Set(ai_scene->mNumMeshes);
			for (unsigned int mesh = 0; mesh < ai_scene->mNumMeshes; ++mesh)
			{
				aiMesh * current_mesh = ai_scene->mMeshes[mesh];
				CreateRendererBaseMesh(current_mesh, scene_data.mesh_descs[mesh], option_flags);

				if (current_mesh->HasBones())
				{
					mesh_with_bones.push_back(&scene_data.mesh_descs[mesh]);
				}
			}

			
			
			
		}
	}
}

