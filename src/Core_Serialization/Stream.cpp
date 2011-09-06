#include "Stream.h"
#include <Core_Assert.h>



Stream & Stream::operator << (const Serializable * val)
{
	Save(val);
	return *this;
}



bool Stream::ShouldSerialize(const Serializable * val_address)
{
	if (!val_address) return false;

	std::pair<std::set<const Serializable *>::iterator, bool> result = serialized_pointed_objects.insert(val_address);
	return result.second;
}

bool Stream::ShouldUnserialize(const Serializable * old_address, Serializable *& rtn_new_address)
{
	if (!old_address)
	{
		rtn_new_address = 0;
		return false;
	}
	std::map<const Serializable *, Serializable *>::iterator result = old_map_new.find(old_address);
	if (result != old_map_new.end())
	{
		rtn_new_address = result->second;
		return false;
	}
	else
	{
		return true;
	}
}

void Stream::SetOldToNew(const Serializable * old_address, Serializable * new_address)
{
	old_map_new[old_address] = new_address;
}
