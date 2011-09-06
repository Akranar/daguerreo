#ifndef _XMLPARSER_H_
#define _XMLPARSER_H_

#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <memory>

#include "XMLElementCreator.h"

class XMLElement;

class XMLParser
{
	std::fstream fin;

	//Tokenizing
	char current_line[30000];
	int current_line_length;
	char current_token[4000];
	int current_token_length;
	char current_whitespaces[20000];
	int current_whitespaces_length;
	int current_start_position;
	int current_end_position;
	bool current_positions_valid;
	bool cached_skipped_space;

	
	char * CopyCurrentToken() const;
	char * Substring(const char * source, const int start, const int end) const;
	void CreateTokenFromSubstr(int start, int end);
	void SaveWhiteSpaceFromSubstr(int start, int end);
	int LocateInCurrentLine(const char * to_find, const int start_position) const;

	std::map<const std::string, const XMLElementCreatorBase *> type_registry;

	XMLAttributesList * current_element_attributes_list;
	bool current_element_is_closed;
	XMLElement * current_element_parent;
	char * current_element_tag;

	std::auto_ptr<XMLElement> root_element;
	std::auto_ptr<XMLElementCreatorBase> default_creator;

	bool ReadNextLine();
public:
	XMLParser();
	virtual ~XMLParser();
	void RegisterElement(const std::string & element_name, const XMLElementCreatorBase * creator);

	/*
		Used by XMLElements. {
	*/
	bool NextElementIs(const std::string & element_name);
	bool NextElementIsOneOf(const std::vector<std::string>& valid_elements);
	bool NextTokenIsElement();
	bool NextTokenIsEndTag(const std::string& element_name);
	bool NextTokenIsEndTag();

	bool GetNextToken();
	bool PeekNextToken();
	const char * GetCurrentToken() const;
	int GetCurrentTokenLength() const;
	const char * GetCurrentWhiteSpaces() const;
	int GetCurrentWhiteSpacesLength() const;

	XMLElement * ProcessNextElement(XMLElement * parent);
	XMLElement * ProcessSpecificElement(const std::string element_id, XMLElement * parent);

	void ProcessStringLiteral(std::string & return_string_literal);
	void ProcessUntil_Append(const std::string & end, std::string & return_string);
	void ProcessUntil_Skip(const std::string & end);

	static int StringToInt(const std::string & str);
	static float StringToFloat(const std::string & str);

	XMLAttributesList * GetCurrentElementAttrList() const;
	bool GetCurrentElementClosed() const;
	XMLElement * GetCurrentElementParent() const;
	char * GetCurrentElementTag() const;
	/*
		Used by XMLElements. }
	*/

	bool Parse(const std::string & filename);
	XMLElement * GetRoot() const;
	template <class T>
	T * GetRoot() const
	{
		return static_cast<T *>(this->root_element.get());
	}

	void SetDefaultCreator(XMLElementCreatorBase *);

};
#endif
