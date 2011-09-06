#ifndef _SCENE_HIERARCHY_H_
#define _SCENE_HIERARCHY_H_

#include "Spatial.h"
#include "ExLib_NTree.h"

class SceneHierarchy: public NTreeDynamicUnordered<Spatial, true>
{
public:
	virtual ~SceneHierarchy();
	void DeleteChildren();

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

#endif