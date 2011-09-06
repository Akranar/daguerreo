#ifndef _DESC_SKINNED_MESH_H_
#define _DESC_SKINNED_MESH_H_

#include "ExLib_Serialization.h"
#include <string>
#include <vector>

class DescSkinnedMesh
{
public:
	std::vector<std::string> visual_ids;
	std::string skeleton_id;

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
};

#endif