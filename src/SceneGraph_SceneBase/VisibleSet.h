#ifndef _VISIBLE_SET_H_
#define _VISIBLE_SET_H_

#include "VisibleObject.h"
#include <vector>

class VisibleSet
{
	std::vector<VisibleObject> visible_set;
public:
	inline void Clear();
	inline VisibleObject operator [] (int index) const;
	inline unsigned int GetCount() const;
	inline void Reserve(unsigned int max_count);
	inline void Insert(const VisibleObject & visible_object);
};

inline void VisibleSet::Clear()
{
	visible_set.clear();
}
inline VisibleObject VisibleSet::operator [] (int index) const
{
	return visible_set[index];
}
inline unsigned int VisibleSet::GetCount() const
{
	return visible_set.size();
}
inline void VisibleSet::Reserve(unsigned int max_count)
{
	visible_set.reserve(max_count);
}
inline void VisibleSet::Insert(const VisibleObject & visible_object)
{
	visible_set.push_back(visible_object);
}

#endif