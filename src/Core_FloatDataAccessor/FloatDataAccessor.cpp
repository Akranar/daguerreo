#include "FloatDataAccessor.h"

bool FloatDataAccessor::Append(const FloatDataAccessor & append)
{
	if (data_size != append.data_size) return false;

	for (unsigned int i = 0; i < append.segments.size(); ++i)
	{
		segments.push_back(append.segments[i]);
		
		if (segments.size() > 1)
		{
			FloatDataSegment & previous = segments[segments.size()-2];
			unsigned int current_size = segments.back().max - segments.back().min;
			segments.back().min = previous.max + 1;
			segments.back().max = segments.back().min + current_size;
		}
	}
	data_count += append.GetCount();
	return true;
}

float * FloatDataAccessor::FindData(unsigned int index) const
{
	unsigned int previous_index = -1;
	unsigned int current_index = segments.size()/2;
	while (current_index != previous_index)
	{
		if (segments[current_index].max < index)
		{
			previous_index = current_index;
			current_index = (segments.size() + current_index) / 2;
		} 
		else if (index <  segments[current_index].min)
		{
			previous_index = current_index;
			current_index = current_index / 2;
		}
		else
		{
			cached_segment_index = current_index;
			return segments[current_index].base + segments[current_index].stride * (index - segments[current_index].min);
		}
	}
	throw 1;
}