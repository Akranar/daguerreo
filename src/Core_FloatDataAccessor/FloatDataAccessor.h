#ifndef FLOAT_DATA_ACCESSOR_H_
#define FLOAT_DATA_ACCESSOR_H_

#include <vector>

class FloatDataAccessor
{
	struct FloatDataSegment
	{
		float * base;
		unsigned int stride;
		unsigned int min;
		unsigned int max;
	};

	unsigned int data_count;
	unsigned int data_size;
	mutable unsigned int cached_segment_index;
	std::vector<FloatDataSegment> segments;
public:
	FloatDataAccessor()
		: data_count(0), data_size(1), cached_segment_index(0)
	{

	}

	FloatDataAccessor(unsigned int data_size)
		: data_count(0), data_size(data_size), cached_segment_index(0)
	{

	}

	FloatDataAccessor(float * base, unsigned int data_size, unsigned int data_count, unsigned int stride)
		: data_count(data_count), data_size(data_size), cached_segment_index(0)
	{
		segments.push_back(FloatDataSegment());
		segments.back().base = base;
		segments.back().stride = stride;
		segments.back().min = 0;
		segments.back().max = data_count-1;
	};

	virtual ~FloatDataAccessor(){};
	inline float * operator[] (unsigned int index) const;
	inline unsigned int GetCount() const;
	inline unsigned int GetDataSize() const;
	bool Append(const FloatDataAccessor & append);
private:
	float * FindData(unsigned int index) const;
};

inline float * FloatDataAccessor::operator[] (unsigned int index) const
{
	if (segments.size() == 1)
	{
		return segments[0].base + segments[0].stride * index;
	}
	else
	{
		if (index <= segments[cached_segment_index].max && index >= segments[cached_segment_index].min)
		{
			return segments[cached_segment_index].base + segments[cached_segment_index].stride * (index - segments[cached_segment_index].min);
		}
		else
		{
			return FindData(index);
		}
	}
}

inline unsigned int FloatDataAccessor::GetCount() const
{
	return data_count;
}

inline unsigned int FloatDataAccessor::GetDataSize() const
{
	return data_size;
}

#endif