#ifndef _SKELETAL_ANIMATION_H_
#define _SKELETAL_ANIMATION_H_

#include "ExLib_Serialization.h"
class SkinnedMesh;

class SkeletalAnimation: public Serializable
{
public:
	virtual ~SkeletalAnimation() {};
	virtual void UpdateSkinnedMesh(SkinnedMesh * skinned_mesh, double time, float blend_factor, bool overwrite) = 0;
	virtual double GetDuration() const = 0;
};

#endif