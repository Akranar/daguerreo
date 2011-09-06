#include "XMLElement.h"
#include "XMLParser.h"
#include "XMLAttributesList.h"

ELEMENT_TYPE_IMPL(XMLElement);

XMLElement::XMLElement(XMLParser * xml_parser)
{
	this->attributes.reset(xml_parser->GetCurrentElementAttrList());
	this->parent = xml_parser->GetCurrentElementParent();
	element_name.reset(xml_parser->GetCurrentElementTag());
	name = element_name.get();
	this->is_closed_element = xml_parser->GetCurrentElementClosed();
}
XMLElement::~XMLElement()
{

}

XMLElement * XMLElement::GetParent() const
{
	return parent;
}