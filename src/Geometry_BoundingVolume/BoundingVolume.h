#ifndef BOUNDINGVOLUME_H
#define BOUNDINGVOLUME_H

#include "ExLib_Serialization.h"

class BoundingVolume: public Serializable
{
public:
	enum BVType
	{
		BV_SPHERE,
		BV_AABB,
		BV_OBB,
		BV_COUNT
	};
	virtual BVType GetType() const = 0;
	virtual ~BoundingVolume() {};

	//Serialization
	virtual void Serialize(Stream & stream) const = 0;
	virtual void Unserialize(Stream & stream) = 0;
	virtual void SerializeTypeId(Stream & stream) const = 0;
};

#endif