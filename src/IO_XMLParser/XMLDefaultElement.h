#ifndef _XMLDefaultElement_H_
#define _XMLDefaultElement_H_

#include "XMLElement.h"
#include "XMLType.h"
#include <vector>

class XMLDefaultElement : public XMLElement
{
public:
	ELEMENT_TYPE_DECL;
	std::vector<XMLElement*> elements;
	std::vector<std::string> data;

	XMLDefaultElement (XMLParser * xml_parser);
	virtual void ParseContent(XMLParser * xml_parser);

	virtual ~XMLDefaultElement() 
	{
		for (unsigned int i = 0; i < elements.size(); ++i)
		{
			delete elements[i];
		}
		elements.clear();
		data.clear();
	};

	template <class T>
	T * GetChildAt(int index) const
	{
		return static_cast<T *>(this->elements[index]);
	}

};

#endif