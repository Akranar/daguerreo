#ifndef _SCENE_NODE_H_
#define _SCENE_NODE_H_

#include "SceneHierarchy.h"
#include <vector>

class SceneNode: public SceneHierarchy
{
protected:

public:
	virtual ~SceneNode() {};

	virtual void UpdateWorldTransform();
	virtual void UpdateWorldBound();
	virtual void UpdateVisibleSet(Culler * culler, bool culling_on);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

#endif
