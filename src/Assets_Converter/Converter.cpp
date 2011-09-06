#include "Converter.h"
#include "ExLib_Assimp.h"
#include "ExLib_Renderer.h"
#include "ExLib_RenderResources.h"

#include "ExLib_Serialization.h"
#include "ExLib_FileSystem.h"
#include "ExLib_Asserts.h"
//#include "ExLib_ShaderEffects.h"
#include <cstdio>
#include <iostream>

#include "ExLib_Renderer.h"
//#include "ExLib_ResourceManager.h"
#include "ExLib_BinaryFileStream.h"

//#include "ResourceMaker.h"

#include "AssimpScenePrinter.h"

#include "ExLib_Skeleton.h"
#include "ExLib_SkeletalAnimation.h"

void GetNameAndExtension(const std::string & filename, std::string & name, std::string & extension)
{
	int index = filename.find(".");
	if (index == -1)
	{
		name = filename;
		extension = "";
	}
	else
	{
		name = filename.substr(0, index);
		extension = filename.substr(index+1, filename.length()-(index+1));
	}
}

void Test()
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile("boblampclean.md5mesh", aiProcess_Triangulate | aiProcess_FlipUVs);
	int x = 0;
}


int ConvertAssets(const char * filename, bool print_scene_data)
{
	//Test();
	std::string name;
	std::string extension;
	std::string std_filename(filename);
	GetNameAndExtension(std_filename, name, extension);

	Assimp::Importer importer;
	//const aiScene * scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeGraph | aiProcess_CalcTangentSpace);
	const aiScene * scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeGraph);

	

	if (!scene)
	{
		std::cout << "Conversion Failed." << std::endl;
		return 1;
	}

	if (print_scene_data)
	{
		AssimpScenePrinter printer;
		printer.SetPrintMatrices(true);
		printer.PrintAllToFile(scene, "assimp.txt");
	}

	/*
	Renderer renderer;
	ResourceManager rm;

	ResourceMaker resource_maker;
	resource_maker.MakeResourceManager(filename, scene, rm);

	BinaryFileStream bfs;
	unsigned int visual_count = 0;
	for (ResourceManager::VisualCatalog::iterator it = rm.IterateVisual(); it != rm.EndVisual(); ++it)
	{
		std::stringstream vf, vb, ib;
		Visual * visual = it->second;
		const VertexFormat * vertex_format = visual->GetVertexBuffer()->GetFormat();
		VertexBuffer * vertex_buffer = visual->GetVertexBuffer();
		IndexBuffer * index_buffer = visual->GetIndexBuffer();
		vertex_buffer->SetFormat(0);

		vf << name << "_" << visual_count << ".vft";
		vb << name << "_" << visual_count << ".vbr";
		ib << name << "_" << visual_count << ".ibr";

		bfs.StartWrite(vf.str().c_str());
		bfs.Save(vertex_format);
		bfs.Close();

		bfs.StartWrite(vb.str().c_str());
		bfs.Save(vertex_buffer);
		bfs.Close();

		bfs.StartWrite(ib.str().c_str());
		bfs.Save(index_buffer);
		bfs.Close();

		visual_count += 1;
	}

	for (ResourceManager::SkeletonCatalog::iterator it = rm.IterateSkeleton(); it != rm.EndSkeleton(); ++it)
	{
		std::stringstream skeleton_filename;
		Skeleton * skeleton = it->second;

		skeleton_filename << name << "_" << '0' << ".skel";

		bfs.StartWrite(skeleton_filename.str().c_str());
		bfs.Save(skeleton);
		bfs.Close();
	}

	for (ResourceManager::SkeletalAnimationCatalog::iterator it = rm.IterateSkeletalAnimation(); it != rm.EndSkeletalAnimation(); ++it)
	{
		std::stringstream skeletal_animation_filename;
		SkeletalAnimation * skeletal_animation = it->second;

		skeletal_animation_filename << name << "_" << '0' << ".anim";

		bfs.StartWrite(skeletal_animation_filename.str().c_str());
		bfs.Save(skeletal_animation);
		bfs.Close();
	}

	std::cout << "Complete!" << std::endl;
	//getchar();
	*/
    return 0;
	
}
