#include "XMLDefaultElement.h"
#include "XMLParser.h"
#include "XMLAttributesList.h"

#include <Core_Assert.h>


bool debug = false;

ELEMENT_TYPE_IMPL(XMLDefaultElement);

XMLDefaultElement::XMLDefaultElement (XMLParser * xml_parser) : XMLElement(xml_parser)
{
	
	
}

void XMLDefaultElement::ParseContent(XMLParser * xml_parser)
{
	/*
		Process Elements or Tokens Until We Find The End Tag. {
	*/

	bool processing_element = false;
	bool processing_data = false;
	
	std::string current_data;
	while (!xml_parser->NextTokenIsEndTag(element_name.get()))
	{
		if (xml_parser->NextTokenIsElement())
		{
			processing_element = true;
			if (processing_data)
			{
				processing_data = false;
				data.push_back(current_data);
				current_data = "";
			}
			XMLElement * element = xml_parser->ProcessNextElement(this);
			if (element) elements.push_back(element);
		}
		else
		{
			processing_data = true;
			if (processing_element)
			{
				processing_element = false;
			}
			if (xml_parser->GetNextToken())
			{
				current_data.append(xml_parser->GetCurrentWhiteSpaces());
			}
			current_data.append(xml_parser->GetCurrentToken());	
		}
	}
	if (processing_data)
	{
		data.push_back(current_data);
	}
	
	/*
		Process Elements or Tokens Until We Find The End Tag. }
		Skip End Tag (Two Tokens) {
	*/

	if (xml_parser->NextTokenIsEndTag(this->element_name.get()))
	{
		xml_parser->GetNextToken();
		xml_parser->GetNextToken();
	}
	else
	{
		//throw 1;
		Assert(false, "XML Parsing Expected End Tag.");
	}
	/*
		Skip End Tag (Two Tokens) }
	*/

	if (debug)
	{
		// Print element name
		std::cout << "Element: " << this->element_name.get() << std::endl;

		attributes->StartIterator();
		std::string name, value;
		/*
			Print attributes {
		*/	
		if (attributes->AttributesQuantity() > 0)
		{
			std::cout << "	Attributes: ";
			while (attributes->HasMore())
			{
				attributes->GetNextAttribute(name, value);
				std::cout << name << "=\"" << value << "\"; ";
			}
			std::cout << std::endl;
		}
		/*
			Print attributes }
		*/	

		for (unsigned int i = 0; i < data.size(); ++i)
		{
			std::cout << "	Data: " << data[i] << std::endl;
		}
	}
	
}