#include "SceneNode.h"
#include "Culler.h"
#include "ExLib_BoundingVolume.h"
#include "ExLib_BoundCreation.h"

void SceneNode::UpdateWorldTransform()
{
	ConcatParentTransform();
	for (unsigned int i = 0; i < GetOrder(); ++i)
	{
		GetChildAt(i)->UpdateWorldTransform();	
	}
	UpdateWorldBound();
}
void SceneNode::UpdateWorldBound()
{
	if (this->GetOrder() > 0)
	{
		unsigned int i = 0;
		for (; i < this->GetOrder(); ++i)
		{
			if (this->GetChildAt(i)->GetWorldBound())
			{
				BoundCreation::CopyBound(this->GetChildAt(i)->GetWorldBound(), this->GetWorldBound());
				break;
			}
		}
		++i;
		
		for (; i < this->GetOrder(); ++i)
		{
			if (this->GetChildAt(i)->GetWorldBound())
			{
				BoundCreation::MergeBound(this->GetWorldBound(), this->GetChildAt(i)->GetWorldBound(), this->GetWorldBound());
			}
		}
	}
}

void SceneNode::UpdateVisibleSet(Culler * culler, bool culling_on)
{
	if (GetCullMode() == Spatial::CULL_ALWAYS) return;
	if (GetCullMode() == Spatial::CULL_NEVER) culling_on = true;
	if (culling_on && !culler->IsVisible(GetWorldBound())) return;


	//Add effect sentinel here (open).
	for (unsigned int i = 0; i < GetOrder(); ++i)
	{
		GetChildAt(i)->UpdateVisibleSet(culler, culling_on);
	}
	//Add effect sentinel here (close).
}

//Serialization
void SceneNode::Serialize(Stream & stream) const
{
	/*
	SceneHierarchy::Serialize(stream);
	STLSerializer<std::vector<RenderState*> > renderstate_serializer(stream);
	renderstate_serializer.Save(render_states);
	*/
}
void SceneNode::Unserialize(Stream & stream)
{
	/*
	SceneHierarchy::Unserialize(stream);
	STLSerializer<std::vector<RenderState*> > renderstate_serializer(stream);
	renderstate_serializer.Load(render_states);
	*/
}
void SceneNode::SerializeTypeId(Stream & stream) const
{
	//stream.Save("SceneNode");
}