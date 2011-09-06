#ifndef _UNCOMPRESSED_SKELETAL_ANIMATION_SAMPLER_H_
#define _UNCOMPRESSED_SKELETAL_ANIMATION_SAMPLER_H_

#include "SkeletalAnimationSampler.h"

class Uncompressed_SkeletalAnimationSampler: SkeletalAnimationSampler
{
	float global_start_time;
	float playback_rate;
public:
	Uncompressed_SkeletalAnimationSampler(int joint_channels);
};

#endif