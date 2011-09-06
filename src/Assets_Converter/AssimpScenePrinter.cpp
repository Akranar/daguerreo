#include "AssimpScenePrinter.h"
#include "ExLib_Assimp.h"
#include <iostream>

#include <fstream>
#include <stdio.h>
#include <stdlib.h>

void SetPrintToFile(const char * filename)
{
	FILE *stream;
    stream = freopen(filename, "w", stdout);
}

void SetPrintToConsole()
{
	FILE *stream;
	stream = freopen("CON", "w", stdout);
}

void AssimpScenePrinter::Indent(int index) const
{

	for (int i = 0; i < index; ++i)
	{
		std::cout << "    ";
	}
}

void AssimpScenePrinter::PrintAllToFile(const aiScene * scene, const char * filename)
{
	bone_names.clear();

	SetPrintToFile(filename);

	PrintSceneOverview(scene, 0);

	PrintAllAnimationData(scene);
	PrintAllCameraData(scene);
	PrintAllLightData(scene);
	PrintAllMaterialData(scene);
	PrintAllMeshData(scene);
	PrintAllTextureData(scene);

	std::cout << "Node Hierarchy: " << std::endl;
	PrintNodeHierarchy(scene->mRootNode, 0);

	SetPrintToConsole();


}

void AssimpScenePrinter::PrintSceneOverview(const aiScene * scene, int indent) const
{
	Indent(indent);
	std::cout << "Scene Overview: " << std::endl;
	Indent(indent+1);
	std::cout << "Maximum Values: " << std::endl;
	Indent(indent+2);
	std::cout << "AI_MAX_BONE_WEIGHTS: " << AI_MAX_BONE_WEIGHTS << std::endl;
	Indent(indent+2);
	std::cout << "AI_MAX_FACE_INDICES: " << AI_MAX_FACE_INDICES << std::endl;
	Indent(indent+2);
	std::cout << "AI_MAX_FACES: " << AI_MAX_FACES << std::endl;
	Indent(indent+2);
	std::cout << "AI_MAX_NUMBER_OF_COLOR_SETS: " << AI_MAX_NUMBER_OF_COLOR_SETS << std::endl;
	Indent(indent+2);
	std::cout << "AI_MAX_NUMBER_OF_TEXTURECOORDS: " << AI_MAX_NUMBER_OF_TEXTURECOORDS << std::endl;
	Indent(indent+2);
	std::cout << "AI_MAX_VERTICES: " << AI_MAX_VERTICES << std::endl;
	std::cout << std::endl;
	Indent(indent+1);
	std::cout << "Animation_Count: " << scene->mNumAnimations << std::endl;
	Indent(indent+1);
	std::cout << "Camera_Count: " << scene->mNumCameras << std::endl;
	Indent(indent+1);
	std::cout << "Light_Count: " << scene->mNumLights << std::endl;
	Indent(indent+1);
	std::cout << "Material_Count: " << scene->mNumMaterials << std::endl;
	Indent(indent+1);
	std::cout << "Mesh_Count: " << scene->mNumMeshes << std::endl;
	Indent(indent+1);
	std::cout << "Texture_Count: " << scene->mNumTextures << std::endl << std::endl;
}

void AssimpScenePrinter::PrintAnimationData(const aiScene * scene, int index, int indent) const
{
	const aiAnimation * animation = scene->mAnimations[index];
	Indent(indent);
	std::cout << "Animation[" << index << "]: " << std::endl;
	Indent(indent+1);
	std::cout << "Name: " << animation->mName.data << std::endl;
	Indent(indent+1);
	std::cout << "Duration: " << animation->mDuration << std::endl;
	Indent(indent+1);
	std::cout << "Ticks Per Second: " << animation->mTicksPerSecond << std::endl;
	Indent(indent+1);
	std::cout << "Node Animations: " << std::endl;
	Indent(indent+2);
	std::cout << "Count: " << animation->mNumChannels << std::endl;
	
	for (unsigned int i = 0; i < animation->mNumChannels; ++i)
	{
		const aiNodeAnim * node_animation = animation->mChannels[i];
		Indent(indent+2);
		std::cout << "NodeAnimation[" << i << "]:" << std::endl;
		Indent(indent+3);
		std::cout << "NodeName: " << node_animation->mNodeName.data << std::endl;
		Indent(indent+3);
		std::cout << "Scaling Keys Count: " << node_animation->mNumScalingKeys << std::endl;
		Indent(indent+3);
		std::cout << "Rotation Keys Count: " << node_animation->mNumRotationKeys << std::endl;
		Indent(indent+3);
		std::cout << "Position Keys Count: " << node_animation->mNumPositionKeys << std::endl;

		Indent(indent+3);
		std::cout << "Pre State: ";
		if (node_animation->mPreState == aiAnimBehaviour_DEFAULT)
			std::cout << "DEFAULT" << std::endl;
		else if (node_animation->mPreState == aiAnimBehaviour_CONSTANT)
			std::cout << "CONSTANT" << std::endl;
		else if (node_animation->mPreState == aiAnimBehaviour_LINEAR)
			std::cout << "LINEAR" << std::endl;
		else if (node_animation->mPreState == aiAnimBehaviour_REPEAT)
			std::cout << "REPEAT" << std::endl;
		else
			std::cout << "UNRECOGNIZED" << std::endl;


		Indent(indent+3);
		std::cout << "Post State: ";
		if (node_animation->mPostState == aiAnimBehaviour_DEFAULT)
			std::cout << "DEFAULT" << std::endl;
		else if (node_animation->mPostState == aiAnimBehaviour_CONSTANT)
			std::cout << "CONSTANT" << std::endl;
		else if (node_animation->mPostState == aiAnimBehaviour_LINEAR)
			std::cout << "LINEAR" << std::endl;
		else if (node_animation->mPostState == aiAnimBehaviour_REPEAT)
			std::cout << "REPEAT" << std::endl;
		else
			std::cout << "UNRECOGNIZED" << std::endl;

	}

	Indent(indent+1);
	std::cout << "Mesh Animations: " << std::endl;
	Indent(indent+2);
	std::cout << "Count: " << animation->mNumMeshChannels << std::endl;

	std::cout << std::endl;
}
void AssimpScenePrinter::PrintCameraData(const aiScene * scene, int index, int indent) const
{

}
void AssimpScenePrinter::PrintLightData(const aiScene * scene, int index, int indent) const
{

}
void AssimpScenePrinter::PrintMaterialData(const aiScene * scene, int index, int indent) const
{

}
void AssimpScenePrinter::PrintMeshData(const aiScene * scene, int index, int indent)
{
	const aiMesh * mesh = scene->mMeshes[index];
	Indent(indent);
	std::cout << "Mesh[" << index << "] :" << std::endl;
	Indent(indent+1);
	std::cout << "Name: ";
	for (unsigned int i = 0; i < mesh->mName.length; ++i)
	{
		std::cout << mesh->mName.data[i];
	}
	std::cout << std::endl;
	Indent(indent+1);
	std::cout << "Material Index: " << mesh->mMaterialIndex << std::endl;
	Indent(indent+1);
	std::cout << "PrimitiveTypes: ";
	if (mesh->mPrimitiveTypes & aiPrimitiveType_POINT) std::cout << "POINT,";
	if (mesh->mPrimitiveTypes & aiPrimitiveType_LINE) std::cout << "LINE,";
	if (mesh->mPrimitiveTypes & aiPrimitiveType_TRIANGLE) std::cout << "TRIANGLE,";
	if (mesh->mPrimitiveTypes & aiPrimitiveType_POLYGON) std::cout << "POLYGON,";
	std::cout << std::endl;
	Indent(indent+1);
	std::cout << "Bone Count: " << mesh->mNumBones << std::endl;
	Indent(indent+1);
	std::cout << "Face Count: " << mesh->mNumFaces << std::endl;
	Indent(indent+1);
	std::cout << "Vertices Count: " << mesh->mNumVertices << std::endl;
	Indent(indent+1);
	if (mesh->HasPositions())
		std::cout << "HasPositions: YES" << std::endl;
	else
		std::cout << "HasPositions: NO" << std::endl;
	Indent(indent+1);
	if (mesh->HasNormals())
		std::cout << "HasNormals: YES" << std::endl;
	else
		std::cout << "HasNormals: NO" << std::endl;
	Indent(indent+1);
	if (mesh->HasTangentsAndBitangents())
		std::cout << "HasTangentsAndBitangents: YES" << std::endl;
	else
		std::cout << "HasTangentsAndBitangents: NO" << std::endl;
	Indent(indent+1);
	std::cout << "TexCoord Sets: ";
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_TEXTURECOORDS; ++i)
	{
		if (mesh->HasTextureCoords(i)) std::cout << i << ",";
	}
	std::cout << std::endl;
	Indent(indent+1);
	std::cout << "VertexColor Sets: ";
	for (unsigned int i = 0; i < AI_MAX_NUMBER_OF_COLOR_SETS; ++i)
	{
		if (mesh->HasVertexColors(i)) std::cout << i << ",";
	}
	std::cout << std::endl;
	PrintAllBoneData(scene, index, indent+1);
	std::cout << std::endl;
}
void AssimpScenePrinter::PrintTextureData(const aiScene * scene, int index, int indent) const
{

}

void AssimpScenePrinter::PrintAllAnimationData(const aiScene * scene) const
{
	std::cout << "Animation Data: " << std::endl;
	std::cout << "	Count: " << scene->mNumAnimations << std::endl;
	for (unsigned int animation_index = 0; animation_index < scene->mNumAnimations; ++animation_index)
	{
		PrintAnimationData(scene, animation_index, 1);
	}
	std::cout << std::endl;
}
void AssimpScenePrinter::PrintAllCameraData(const aiScene * scene) const
{
	std::cout << "Camera Data: " << std::endl;
	std::cout << "	Count: " << scene->mNumCameras << std::endl;
	for (unsigned int camera_index = 0; camera_index < scene->mNumCameras; ++camera_index)
	{
		
	}
	std::cout << std::endl;
}
void AssimpScenePrinter::PrintAllLightData(const aiScene * scene) const
{
	std::cout << "Light Data: " << std::endl;
	std::cout << "	Count: " << scene->mNumLights << std::endl;
	for (unsigned int light_index = 0; light_index < scene->mNumLights; ++light_index)
	{
		
	}
	std::cout << std::endl;
}
void AssimpScenePrinter::PrintAllMaterialData(const aiScene * scene) const
{
	std::cout << "Material Data: " << std::endl;
	std::cout << "	Count: " << scene->mNumMaterials << std::endl;
	for (unsigned int material_index = 0; material_index < scene->mNumMaterials; ++material_index)
	{
		
	}
	std::cout << std::endl;
}
void AssimpScenePrinter::PrintAllMeshData(const aiScene * scene)
{
	std::cout << "Mesh Data: " << std::endl;
	std::cout << "	Count: " << scene->mNumMeshes << std::endl;
	for (unsigned int mesh_index = 0; mesh_index < scene->mNumMeshes; ++mesh_index)
	{
		PrintMeshData(scene, mesh_index, 1);
	}
	std::cout << std::endl;
}
void AssimpScenePrinter::PrintAllTextureData(const aiScene * scene) const
{
	std::cout << "Texture Data: " << std::endl;
	std::cout << "	Count: " << scene->mNumTextures << std::endl;
	for (unsigned int texture_index = 0; texture_index < scene->mNumTextures; ++texture_index)
	{
		
	}
	std::cout << std::endl;
}

void AssimpScenePrinter::PrintAllBoneData(const aiScene * scene, int mesh_index, int indent)
{
	Indent(indent);
	std::cout << "Bone Data: " << std::endl;
	Indent(indent+1);
	std::cout << "Count: " << scene->mMeshes[mesh_index]->mNumBones << std::endl;
	for (unsigned int bone_index = 0; bone_index < scene->mMeshes[mesh_index]->mNumBones; ++bone_index)
	{
		PrintBoneData(scene, mesh_index, bone_index, indent+1);
	}
}
void AssimpScenePrinter::PrintBoneData(const aiScene * scene, int mesh_index, int bone_index, int indent)
{
	const aiBone * bone = scene->mMeshes[mesh_index]->mBones[bone_index];
	Indent(indent);
	std::cout << "Bone[" << bone_index << "]: " << std::endl;
	Indent(indent+1);
	std::cout << "Name: ";
	for (unsigned int i = 0; i < bone->mName.length; ++i)
	{
		std::cout << bone->mName.data[i];
	}
	std::cout << std::endl;
	Indent(indent+1);
	std::cout << "Weights Count: " << bone->mNumWeights << std::endl;
	if (print_matrices) 
	{
		Indent(indent+1);
		std::cout << "Offset Matrix: " << std::endl;
		PrintMatrix4x4(&bone->mOffsetMatrix, indent+2);
	}

	bone_names.insert(bone->mName.data);
	//scene->mRootNode->FindNode("hi");
	
}

void AssimpScenePrinter::PrintNodeHierarchy(const aiNode * node, int indent) const
{
	Indent(indent);
	if (bone_names.find(node->mName.data) != bone_names.end())
	{
		std::cout << "Bone: ";
	}
	else
	{
		std::cout << "Node: ";
	}
	for (unsigned int i = 0; i < node->mName.length; ++i)
	{
		std::cout << node->mName.data[i];
	}
	std::cout << " [" << node->mNumChildren << "]" << " [" << node->mNumMeshes << "]" << std::endl;
	if (print_matrices) PrintMatrix4x4(&node->mTransformation, indent+1);
	for (unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		PrintNodeHierarchy(node->mChildren[i], indent+1);
	}
}

void AssimpScenePrinter::PrintMatrix4x4(const aiMatrix4x4 * matrix, int indent) const
{
	Indent(indent);
	std::cout << matrix->a1 << ", " << matrix->a2 << ", " << matrix->a3 << ", " << matrix->a4 << std::endl;
	Indent(indent);
	std::cout << matrix->b1 << ", " << matrix->b2 << ", " << matrix->b3 << ", " << matrix->b4 << std::endl;
	Indent(indent);
	std::cout << matrix->c1 << ", " << matrix->c2 << ", " << matrix->c3 << ", " << matrix->c4 << std::endl;
	Indent(indent);
	std::cout << matrix->d1 << ", " << matrix->d2 << ", " << matrix->d3 << ", " << matrix->d4 << std::endl;
}