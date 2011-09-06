#ifndef _XMLELEMENTCREATOR_H_
#define _XMLELEMENTCREATOR_H_

//#include "XMLParser.h"

class XMLParser;
class XMLAttributesList;
class XMLElement;

class XMLElementCreatorBase
{
public:
	virtual ~XMLElementCreatorBase() {}
    virtual XMLElement* Create() const = 0;
};

template<class Product>
class XMLElementCreator : public XMLElementCreatorBase
{
	XMLParser * xml_parser;
public:
	XMLElementCreator(XMLParser * xml_parser)
	{
		this->xml_parser = xml_parser;
	}
	virtual XMLElement * Create() const 
	{
		return new Product(xml_parser);
	}
};


#endif
