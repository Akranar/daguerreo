#include "AssimpSceneData.h"
#include "ExLib_BinaryFileStream.h"
#include <sstream>

#include "ExLib_SceneBase.h"
#include "ExLib_BoundingVolume.h"

AssimpSceneData::AssimpSceneData()
{

}
AssimpSceneData::~AssimpSceneData()
{
	
	for (unsigned int i = 0; i < skeletons.size(); ++i)
	{
		delete skeletons[i];
	}
	skeletons.clear();
	for (unsigned int i = 0; i < skeletal_animations.size(); ++i)
	{
		delete skeletal_animations[i];
	}
	skeletal_animations.clear();
	
}

void AssimpSceneData::OutputMeshResources(const std::string & filename)
{
	BinaryFileStream stream;
	for (unsigned int i = 0; i < mesh_descs.GetSize(); ++i)
	{
		RendererBaseMesh & current_mesh = mesh_descs[i];
		std::stringstream string_stream;
		string_stream << filename << '_' << i << '.';

		{
			std::stringstream out_file;
			out_file << string_stream.str() << "vbr";

			STLSerializer<FixedArray<unsigned char> > data_serializer(stream);
			stream.StartWrite(out_file.str().c_str());
			{
				stream.Save(current_mesh.GetFormatDesc());
				stream.Save(current_mesh.GetVertexDesc());
				data_serializer.Save(current_mesh.GetVertexData());
			}
			stream.Close();

		}

		{
			std::stringstream out_file;
			out_file << string_stream.str() << "ibr";

			STLSerializer<FixedArray<unsigned char> > data_serializer(stream);
			stream.StartWrite(out_file.str().c_str());
			{
				stream.Save(current_mesh.GetIndexDesc());
				data_serializer.Save(current_mesh.GetIndexData());
			}
			stream.Close();

		}
	}
}
void AssimpSceneData::OutputSceneGraphResources(const std::string & filename)
{
	BinaryFileStream stream;

	for (unsigned int i = 0; i < mesh_descs.GetSize(); ++i)
	{
		RendererBaseMesh & current_mesh = mesh_descs[i];
		std::stringstream string_stream;
		string_stream << filename << '_' << i << '.';

		std::stringstream vb_file;
		vb_file << string_stream.str() << "vbr";
		std::stringstream ib_file;
		ib_file << string_stream.str() << "ibr";
		std::stringstream out_file;
		out_file << string_stream.str() << "vsr";

		DescVisual visual_desc;
		visual_desc.SetVertexBufferID(vb_file.str());
		visual_desc.SetIndexBufferID(ib_file.str());

		
		DescVertexFormatEntry & attrib = current_mesh.GetFormatDesc()[0];
		if (attrib.GetName().compare("POSITION") == 0)
		{//Only AABB supported currently.
			visual_desc.CreateBoundingVolume(BoundingVolume::BV_AABB, 
				current_mesh.GetVertexData().GetData() + attrib.GetOffset(), 
				attrib.GetStride() / 4,  //NEED TO FIX THIS LATER
				current_mesh.GetVertexData().GetSize() / attrib.GetStride());
		}

		stream.StartWrite(out_file.str().c_str());
		stream.Save(visual_desc);
		stream.Close();
	}
}
void AssimpSceneData::OuptutAnimationResources(const std::string & filename)
{
	BinaryFileStream stream;

	for (unsigned int i = 0; i < skeletons.size(); ++i)
	{
		std::stringstream string_stream;
		string_stream << filename << '_' << i << '.';

		std::stringstream skeleton_file;
		skeleton_file << string_stream.str() << "skel";

		stream.StartWrite(skeleton_file.str().c_str());
		{
			stream.Save(skeletons[i]);
		}
		stream.Close();
	}

	for (unsigned int i = 0; i < skeletal_animations.size(); ++i)
	{
		std::stringstream string_stream;
		string_stream << filename << '_' << i << '.';

		std::stringstream anim_file;
		anim_file << string_stream.str() << "anim";

		stream.StartWrite(anim_file.str().c_str());
		{
			stream.Save(skeletal_animations[i]);
		}
		stream.Close();
	}
}