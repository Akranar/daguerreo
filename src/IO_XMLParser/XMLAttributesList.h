#ifndef _XMLATTRIBUTESLIST_H_
#define _XMLATTRIBUTESLIST_H_

#include <map>
#include <string>

class XMLAttributesList
{
	std::map<const std::string, const std::string> attributes;
	std::map<const std::string, const std::string>::iterator iter;
public:
	bool GetAttribute(const std::string & name, std::string & return_string) const;
	const std::string & GetAttribute(const std::string & name) const;
	void SetAttribute(const std::string & name, const std::string & value);
	void GetNextAttribute(std::string & name, std::string & value);
	void StartIterator();
	bool HasMore() const;
	int AttributesQuantity() const;
};

#endif
