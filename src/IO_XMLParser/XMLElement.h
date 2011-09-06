#ifndef _XMLELEMENT_H_
#define _XMLELEMENT_H_

#include <memory>
#include <string>
#include <stack>
#include <queue>
#include "XMLType.h"

class XMLAttributesList;
class XMLParser;

class XMLElement
{
protected:
	std::auto_ptr<char> element_name;
	std::string name;
	bool is_closed_element;
	XMLElement * parent;
public:
	ELEMENT_TYPE_DECL;
	std::auto_ptr<XMLAttributesList> attributes;
	XMLElement * GetParent() const;
	XMLElement(XMLParser * xml_parser);
	const char * GetElementTag() const
	{
		return element_name.get();
	};
	virtual ~XMLElement();

	virtual void ParseContent(XMLParser * xml_parser) = 0;

	//virtual bool TraverseDepthFirst(std::stack<XMLElement *>& traversal_stack) {return false;};
	//virtual bool TraverseBreadthFirst(std::queue<XMLElement *>& traversal_queue) {return false;};
};

#endif