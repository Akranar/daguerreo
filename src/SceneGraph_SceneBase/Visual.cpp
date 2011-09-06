#include "Visual.h"
#include "Culler.h"
#include "VisibleSet.h"
#include "VisibleObject.h"
#include "ExLib_RendererBase.h"
#include "ExLib_RendererFX.h"
#include "ExLib_BoundingVolume.h"
#include "ExLib_BoundCreation.h"
#include "DescVisual.h"



Visual::Visual()
:
model_bound(0)
{
}

Visual::Visual(VertexBuffer * vertex_buffer, IndexBuffer * index_buffer)
: vertex_buffer(vertex_buffer), index_buffer(index_buffer), model_bound(0)
{
}


Visual::~Visual()
{
	if (model_bound) delete model_bound;
}

void Visual::CreateWorldFromModelBound()
{
	if (world_bound) delete world_bound;
	if (model_bound)
	{
		world_bound = BoundCreation::NewOfSameType(GetModelBound());
	}
}

void Visual::UpdateWorldTransform()
{
	ConcatParentTransform();
	UpdateWorldBound();
}
void Visual::UpdateWorldBound()
{
	BoundCreation::TransformBound(GetWorldTransform(), GetModelBound(), GetWorldBound());
}

void Visual::UpdateVisibleSet(Culler * culler, bool culling_on)
{
	if (GetCullMode() == Spatial::CULL_ALWAYS) return;
	if (GetCullMode() == Spatial::CULL_NEVER) culling_on = true;
	if (culling_on && !culler->IsVisible(GetWorldBound())) return;

	culler->GetVisibleSet()->Insert(VisibleObject(this));
}


//Serialization:
void Visual::Serialize(Stream & stream) const
{


}
void Visual::SerializeTypeId(Stream & stream) const
{
	stream.Save("Visual");
}
void Visual::Unserialize(Stream & stream)
{

}

