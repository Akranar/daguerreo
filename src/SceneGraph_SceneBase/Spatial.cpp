#include "Spatial.h"
#include "Culler.h"
#include "VisibleSet.h"

//#include "ExLib_RendererStates.h"
#include "ExLib_BoundingVolume.h"


Spatial::Spatial()
:
cull_mode(Spatial::CULL_DYNAMIC)
{

}
Spatial::~Spatial()
{

}

void Spatial::UpdateWorldTransform()
{
	ConcatParentTransform();
}

void Spatial::UpdateWorldBound()
{

}


void Spatial::UpdateVisibleSet(Culler * culler, bool culling_on)
{
	
}

void Spatial::GeometricStateUpdate()
{
	UpdateWorldTransform();
	//Propogate World Bounds To Root {
	Spatial * parent = GetParentAs<Spatial>();
	while (parent != NULL)
	{
		parent->UpdateWorldBound();
		parent = parent->GetParentAs<Spatial>();
	}
	//Propogate World Bounds To Root }
}


void Spatial::GetVisibleSet(Culler * culler)
{
	culler->SetFrustum();
	culler->GetVisibleSet()->Clear();
	UpdateVisibleSet(culler, true);
}

//Serialization
void Spatial::Serialize(Stream & stream) const
{
	/*
	BoundAndTransform::Serialize(stream);
	stream.Save(cull_mode);
	stream.Save(world_bound);
	*/
}
void Spatial::Unserialize(Stream & stream)
{
	/*
	BoundAndTransform::Unserialize(stream);
	stream.Load((unsigned int &) cull_mode);
	stream.Load(world_bound);
	*/
}
void Spatial::SerializeTypeId(Stream & stream) const
{
	//stream.Save("Spatial");
}