#ifndef _NODE_LOADERS_H_
#define _NODE_LOADERS_H_

#include <Core_ObjectFactory.h>
#include "ExLib_Serialization.h"
#include "ExLib_SceneBase.h"
#include "ExLib_BinaryFileStream.h"
//*********************************************
//	VisualLoader
//*********************************************
class VisualLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * resource_manager, Visual * resource)
	{
		DescVisual visual_desc;
		BinaryFileStream resource_stream;
		resource_stream.StartRead(filename, *factory);
		{
			resource_stream.Load(visual_desc);
		}
		resource_stream.Close();
		VertexBuffer * vertex_buffer = resource_manager->Retrieve(resource_manager->GetVertexBufferCatalog(), visual_desc.GetVertexBufferID().c_str());
		IndexBuffer * index_buffer = resource_manager->Retrieve(resource_manager->GetIndexBufferCatalog(), visual_desc.GetIndexBufferID().c_str());
		resource->SetVertexBuffer(vertex_buffer);
		resource->SetIndexBuffer(index_buffer);
		resource->SetModelBound(visual_desc.ReliquishBoundOwnership());
		resource->CreateWorldFromModelBound();
		resource->SetFX(0);//FIX LATER
	}
	template <class MANAGER_TYPE>
	void Unload(Visual * resource, MANAGER_TYPE * resource_manager)
	{
		if (resource->GetVertexBuffer())
		{
			resource_manager->Release(resource_manager->GetVertexBufferCatalog(), resource->GetVertexBuffer());
		}	
		if (resource->GetIndexBuffer())
		{
			resource_manager->Release(resource_manager->GetIndexBufferCatalog(), resource->GetIndexBuffer());
		}
		
		if (resource->GetFX())
		{
			//resource_manager->Release(resource_manager->GetVisualFXInstanceCatalog(), resource->GetFX()); FIX LATER
		}
		
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};

#endif