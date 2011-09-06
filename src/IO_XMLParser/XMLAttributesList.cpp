#include "XMLAttributesList.h"

bool XMLAttributesList::GetAttribute(const std::string & name, std::string & return_string) const
{
	std::map<const std::string, const std::string>::const_iterator  iter = this->attributes.find(name);
	if (iter != attributes.end())
	{
		return_string = (*iter).second;
		return true;
	}
	return false;
}

const std::string & XMLAttributesList::GetAttribute(const std::string & name) const
{
	std::map<const std::string, const std::string>::const_iterator  iter = this->attributes.find(name);
	if (iter != attributes.end())
	{
		return (*iter).second;
	}
	else
	{
		throw 1;
	}
}

void XMLAttributesList::SetAttribute(const std::string & name, const std::string & value)
{
	attributes.insert(std::pair<const std::string, const std::string>(name, value));
}

void XMLAttributesList::GetNextAttribute(std::string & name, std::string & value)
{
	name = (*iter).first;
	value = (*iter).second;
	iter++;
}

void XMLAttributesList::StartIterator()
{
	iter = this->attributes.begin();
}

bool XMLAttributesList::HasMore() const
{
	return iter != this->attributes.end();
}

int XMLAttributesList::AttributesQuantity() const
{
	return attributes.size();
}