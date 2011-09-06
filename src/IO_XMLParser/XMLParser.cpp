#include "XMLParser.h"
#include "XMLElement.h"
#include "XMLAttributesList.h"
#include "XMLDefaultElement.h"

#include <sstream>
#include <cstring>

#include <Core_Assert.h>

XMLParser::XMLParser()
{
	current_positions_valid = false;
	current_start_position = 0;
	current_end_position = 0;

	current_element_attributes_list = 0;
	current_element_is_closed = false;
	current_element_parent = 0;

	current_line_length = 0;
	current_token_length = 0;

	default_creator.reset(new XMLElementCreator<XMLDefaultElement>(this));
}
XMLParser::~XMLParser()
{
	std::map<const std::string, const XMLElementCreatorBase *>::iterator it;
	for (it = type_registry.begin(); it != type_registry.end(); ++it)
	{
		delete it->second;
	}
}

int line = 1;
bool XMLParser::ReadNextLine()
{
	if (line == 903)
	{
		int x = 0;
	}
	//int my_line = line;

	//int length;
	//std::streampos saved_position;
	//saved_position = fin.tellg();
	fin.getline(this->current_line, 30000); //Get until '\n' found. Error if length is over 30000
	if (fin.fail())
	{
		//std::streampos old = fin.tellg();
		int gcount = fin.gcount();
		fin.clear();
		//old = fin.tellg();
		//gcount = fin.gcount();
		//std::streampos destination = old - 3000;
		//fin.seekg(saved_position);
		fin.seekg(-gcount, std::ios::cur);

		//old = fin.tellg();
		//gcount = fin.gcount();
		fin.get(this->current_line, 25000); //Try again, getting the first 24999 characters.
		if (fin.fail()) 
		{
			throw 1;
		}

		
		if (fin.peek() != ' ')
		{
			//saved_position = fin.tellg();
			//fin.seekg(saved_position);
			fin.get(this->current_line+24999, 5000, ' '); //Get the next 4999 characters until we find the first whitespace character.

			if (fin.fail())
			{
				//Case: There were 500 consecutive non-whitespace characters. Also know there are not newlines in this region. Fail.
				throw 1;
			}
		}
		if (fin.fail()) 
		{
			throw 1;
		}
		
	}
	current_line_length = strlen(current_line);
	current_start_position = 0;
	current_end_position = 0;

	line += 1;
	return !fin.eof();
}

void XMLParser::RegisterElement(const std::string & element_name, const XMLElementCreatorBase * creator)
{
	type_registry[element_name] = creator;
}

bool XMLParser::NextElementIs(const std::string & element_name)
{
	PeekNextToken();
	if (GetCurrentTokenLength() < 2) return false;
	if (GetCurrentToken()[0] != '<') return false;
	return strncmp(GetCurrentToken()+1, element_name.c_str(), element_name.length()) == 0;
}
bool XMLParser::NextElementIsOneOf(const std::vector<std::string>& valid_elements)
{
	for (unsigned int i = 0; i < valid_elements.size(); ++i)
	{
		if (NextElementIs(valid_elements[i])) return true;
	}
	return false;
}
bool XMLParser::NextTokenIsElement()
{
	PeekNextToken();
	if (GetCurrentTokenLength() < 2) return false;
	return (GetCurrentToken()[0] == '<' && GetCurrentToken()[1] != '/');
}
bool XMLParser::NextTokenIsEndTag(const std::string& element_name)
{
	PeekNextToken();
	if (GetCurrentTokenLength() < 3) return false;
	if (GetCurrentToken()[0] != '<' || GetCurrentToken()[1] != '/') return false;
	return strncmp(GetCurrentToken()+2, element_name.c_str(), element_name.length()) == 0;
}
bool XMLParser::NextTokenIsEndTag()
{
	PeekNextToken();
	if (GetCurrentTokenLength() < 3) return false;
	return (GetCurrentToken()[0] == '<' && GetCurrentToken()[1] == '/');
}



bool XMLParser::GetNextToken()
{
	if (current_positions_valid)
	{
		CreateTokenFromSubstr(current_start_position, current_end_position);
		current_positions_valid = false;
		return cached_skipped_space;
	}
	else
	{
		current_start_position = current_end_position;
		while (current_start_position >= current_line_length)
		{
			//Start new line.
			ReadNextLine();
		}
		
		if (isspace(current_line[current_end_position]))
		{
			//Case: white spaces.
			while (current_end_position < current_line_length && isspace(current_line[current_end_position]))
			{
				current_end_position += 1;
			}
			SaveWhiteSpaceFromSubstr(current_start_position, current_end_position);
			GetNextToken();
			return true;
		}
		else
		{
			if (current_line[current_end_position] == '/' 
				&& current_end_position < current_line_length-1 
				&& current_line[current_end_position+1] == '>')
			{
				//Case: "/>"
				current_end_position += 2;
				CreateTokenFromSubstr(current_start_position, current_end_position);
			}
			else if (current_line[current_end_position] == '/'
				|| current_line[current_end_position] == '='
				|| current_line[current_end_position] == '>'
				|| current_line[current_end_position] == '?')
			{
				//Case: "/"
				//Case: "="
				//Case: ">"
				//Case: "?"
				current_end_position += 1;
				CreateTokenFromSubstr(current_start_position, current_end_position);
			}
			else if (current_line[current_end_position] == '<' 
				&& current_end_position < current_line_length-3 
				&& current_line[current_end_position+1] == '!'
				&& current_line[current_end_position+2] == '-'
				&& current_line[current_end_position+3] == '-')
			{
				//Case Comment: "<!--"
				ProcessUntil_Skip("-->");
				current_end_position += 3;
				return GetNextToken();
			}
			else if (current_line[current_end_position] == '<' 
				&& current_end_position < current_line_length-8 
				&& current_line[current_end_position+1] == '!'
				&& current_line[current_end_position+2] == '['
				&& current_line[current_end_position+3] == 'C'
				&& current_line[current_end_position+3] == 'D'
				&& current_line[current_end_position+3] == 'A'
				&& current_line[current_end_position+3] == 'T'
				&& current_line[current_end_position+3] == 'A'
				&& current_line[current_end_position+3] == '['
				)
			{
				//Case Comment: "<![CDATA["
				ProcessUntil_Skip("]]>");
				current_end_position += 3;
				return GetNextToken();
			}
			else if (current_line[current_end_position] == '<')
			{
				
				if (current_end_position < current_line_length+2
					&& current_line[current_end_position+1] == '/')
				{
					//Case End Tags:
					//	-> Case: "</" + [alpha, symbol, number] * ... until: space, >, /, =, ?
					current_end_position += 2;
					while (current_end_position < current_line_length && !isspace(current_line[current_end_position])
						&& current_line[current_end_position] != '<'
						&& current_line[current_end_position] != '>'
						&& current_line[current_end_position] != '/'
						&& current_line[current_end_position] != '='
						&& current_line[current_end_position] != '?')
					{
						current_end_position += 1;
						if (current_line[current_end_position-1] == ';') break;
					}
				}
				else
				{
					//Opening Tags.
					//	-> Case: "<" + [alpha, symbol, number] * ... until: space, >, /, =, ?
					current_end_position += 2;
					while (current_end_position < current_line_length && !isspace(current_line[current_end_position])
						&& current_line[current_end_position] != '<'
						&& current_line[current_end_position] != '>'
						&& current_line[current_end_position] != '/'
						&& current_line[current_end_position] != '='
						&& current_line[current_end_position] != '?')
					{
						current_end_position += 1;
						if (current_line[current_end_position-1] == ';') break;
					}
				}
				CreateTokenFromSubstr(current_start_position, current_end_position);
			}
			else
			{
				//Case Non-whitespace until: white-space, <, >, /, =, ?
				current_end_position += 1;
				while (current_end_position < current_line_length && !isspace(current_line[current_end_position])
					&& current_line[current_end_position] != '<'
					&& current_line[current_end_position] != '>'
					&& current_line[current_end_position] != '/'
					&& current_line[current_end_position] != '='
					&& current_line[current_end_position] != '?')
				{
					current_end_position += 1;
					if (current_line[current_end_position-1] == ';') break;
				}
				CreateTokenFromSubstr(current_start_position, current_end_position);
			}
			return false;
		}
	}
	
}
bool XMLParser::PeekNextToken()
{
	if (current_positions_valid)
	{
		return cached_skipped_space;
	}
	else
	{
		cached_skipped_space = GetNextToken();
		current_positions_valid = true;
		return cached_skipped_space;
	}
}

XMLElement * XMLParser::ProcessNextElement(XMLElement * parent)
{
	PeekNextToken();
	std::auto_ptr<char> peek_token(CopyCurrentToken());
	if (GetCurrentTokenLength() < 2) return false;
	if (GetCurrentToken()[0] != '<') return false;
	
	std::string attribute_name;
	std::string equal_sign;
	std::string attribute_value;
	GetNextToken(); //Skip element name
	GetNextToken();
	XMLAttributesList * attributes_list = new XMLAttributesList();
	this->current_element_is_closed = false;
	while (strcmp(GetCurrentToken(),">") != 0 && strcmp(GetCurrentToken(),"/>") != 0)
	{
		attribute_name = std::string(GetCurrentToken());
		GetNextToken();
		equal_sign = std::string(GetCurrentToken());
		if (equal_sign.compare("=") != 0)
		{
			std::cout << "Error: Expected '='." << std::endl;
			return NULL;
		}
		ProcessStringLiteral(attribute_value);
		attributes_list->SetAttribute(attribute_name, attribute_value);
		GetNextToken();
	}
	this->current_element_parent = parent;
	if (strcmp(GetCurrentToken(),"/>") == 0) this->current_element_is_closed = true;
	this->current_element_attributes_list = attributes_list;
	this->current_element_tag = Substring(peek_token.get(), 1, strlen(peek_token.get()));

	XMLElement * rtn;
	std::map<const std::string, const XMLElementCreatorBase *>::iterator it = type_registry.find(this->current_element_tag);
	if (it != type_registry.end())
	{
		rtn = it->second->Create();
	}
	else
	{
		rtn = default_creator->Create();
	}
	if (!current_element_is_closed) rtn->ParseContent(this);
	return rtn;
}

XMLElement * XMLParser::ProcessSpecificElement(const std::string element_id, XMLElement * parent)
{
	PeekNextToken();
	std::auto_ptr<char> peek_token(CopyCurrentToken());
	if (GetCurrentTokenLength() < 2) return false;
	if (GetCurrentToken()[0] != '<') return false;
	
	std::string attribute_name;
	std::string equal_sign;
	std::string attribute_value;
	GetNextToken(); //Skip element name
	GetNextToken();
	XMLAttributesList * attributes_list = new XMLAttributesList();
	this->current_element_is_closed = false;
	while (strcmp(GetCurrentToken(),">") != 0 && strcmp(GetCurrentToken(),"/>") != 0)
	{
		attribute_name = std::string(GetCurrentToken());
		GetNextToken();
		equal_sign = std::string(GetCurrentToken());
		if (equal_sign.compare("=") != 0)
		{
			std::cout << "Error: Expected '='." << std::endl;
			return NULL;
		}
		ProcessStringLiteral(attribute_value);
		attributes_list->SetAttribute(attribute_name, attribute_value);
		GetNextToken();
	}
	this->current_element_parent = parent;
	if (strcmp(GetCurrentToken(),"/>") == 0) this->current_element_is_closed = true;
	this->current_element_attributes_list = attributes_list;
	this->current_element_tag = Substring(peek_token.get(), 1, strlen(peek_token.get()));
	std::map<const std::string, const XMLElementCreatorBase *>::iterator it = type_registry.find(this->current_element_tag);
	if (it != type_registry.end())
	{
		return it->second->Create();
	}
	else
	{
		//return new XMLDefaultElement(this);
		return default_creator->Create();
	}
}

void XMLParser::ProcessStringLiteral(std::string & return_string_literal)
{
	PeekNextToken(); //Make sure to consume whitespaces before token.
	return_string_literal = "";
	//If last token action was not a peek, start from beginning of next token.
	if (!current_positions_valid)
		current_start_position = current_end_position;

	if (current_line[current_start_position] == '"')
	{
		current_start_position += 1;
		current_end_position = current_start_position;
		ProcessUntil_Append("\"", return_string_literal);
	}
	else if (current_line[current_start_position] == '\'')
	{
		current_start_position += 1;
		current_end_position = current_start_position;
		ProcessUntil_Append("'", return_string_literal);
	}
	else
		throw 1;

	current_positions_valid = false;
	current_start_position += 1;
	current_end_position += 1;
}

void XMLParser::ProcessUntil_Skip(const std::string & end)
{
	//If last token action was not a peek, start from beginning of next token.
	if (!current_positions_valid)
		current_start_position = current_end_position;

	if (strlen(current_line) == 0) 
	{
		if (!ReadNextLine()) throw 1;
	}
	int position = LocateInCurrentLine(end.c_str(), current_start_position);
	while (position == -1)
	{
		if (!ReadNextLine()) throw 1;
		position = LocateInCurrentLine(end.c_str(), current_start_position);
	}

	current_positions_valid = false;
	current_start_position = position;
	current_end_position = position;
	
}



void XMLParser::ProcessUntil_Append(const std::string & end, std::string & return_string)
{
	//If last token action was not a peek, start from beginning of next token.
	if (!current_positions_valid)
		current_start_position = current_end_position;

	if (strlen(current_line) == 0) 
	{
		if (!ReadNextLine()) throw 1;
	}
	int position = LocateInCurrentLine(end.c_str(), current_start_position);
	while (position == -1)
	{
		std::auto_ptr<char> substr(Substring(current_line, current_start_position, current_line_length));
		return_string.append(substr.get());
		return_string.push_back('\n');
		if (!ReadNextLine()) throw 1;
		position = LocateInCurrentLine(end.c_str(), current_start_position);
	}
	std::auto_ptr<char> substr(Substring(current_line, current_start_position, position));
	return_string.append(substr.get());

	current_positions_valid = false;
	current_start_position = position;
	current_end_position = position;
	
}

int XMLParser::StringToInt(const std::string & str)
{
	int i;
	std::istringstream my_stream(str);

	if (my_stream >> i)
	{
		return i;
	}
	else
	{
		throw 2;
	}
}
float XMLParser::StringToFloat(const std::string & str)
{
	float f;
	std::istringstream my_stream(str);

	if (my_stream >> f)
	{
		return f;
	}
	else
	{
		throw 2;
	}
}


XMLElement * XMLParser::GetRoot() const
{
	return root_element.get();
}


XMLAttributesList * XMLParser::GetCurrentElementAttrList() const
{
	return this->current_element_attributes_list;
}
bool XMLParser::GetCurrentElementClosed() const
{
	return this->current_element_is_closed;
}
XMLElement * XMLParser::GetCurrentElementParent() const
{
	return this->current_element_parent;
}
char * XMLParser::GetCurrentElementTag() const
{
	return this->current_element_tag;
}

const char * XMLParser::GetCurrentToken() const
{
	return current_token;
}

const char * XMLParser::GetCurrentWhiteSpaces() const
{
	return current_whitespaces;
}

int XMLParser::GetCurrentTokenLength() const
{
	return current_token_length;
}

int XMLParser::GetCurrentWhiteSpacesLength() const
{
	return current_whitespaces_length;
}

char * XMLParser::CopyCurrentToken() const
{
	char * str = new char[GetCurrentTokenLength()+1];
	strncpy(str, current_token, GetCurrentTokenLength());
	str[GetCurrentTokenLength()] = 0;
	return str;
}

void XMLParser::CreateTokenFromSubstr(int start, int end)
{
	int token_index = 0;
	strncpy(current_token, current_line+start, end-start);
	current_token[end-start] = 0;
	current_token_length = end - start;
}

void XMLParser::SaveWhiteSpaceFromSubstr(int start, int end)
{
	int token_index = 0;
	strncpy(current_whitespaces, current_line+start, end-start);
	current_whitespaces[end-start] = 0;
	current_whitespaces_length = end - start;
}

char * XMLParser::Substring(const char * source, const int start, const int end) const
{
	int dest_index = 0;
	int source_length = strlen(source);
	unsigned int source_index = start;
	char * dest = new char[source_length + 1];
	strncpy(dest, source+start, end-start);
	dest[end-start] = 0;
	return dest;
}

int XMLParser::LocateInCurrentLine(const char * to_find, const int start_position) const
{
	int find_length = strlen(to_find);
	for (int i = start_position; i <= current_line_length - find_length; ++i)
	{
		if (!strncmp(to_find, current_line+i, find_length))
			return i;
	}
	return -1;
}

bool XMLParser::Parse(const std::string & filename)
{
	fin.open(filename.c_str(), std::ios_base::in);
	if (!fin.good()) return false;

	try 
	{

		std::string dummy;
		GetNextToken();
		if (strcmp(GetCurrentToken(),"<?xml") != 0) throw 1;
		ProcessUntil_Append("?>", dummy);
		GetNextToken();
		GetNextToken();

		root_element = std::auto_ptr<XMLElement>(ProcessNextElement(0));

	}
	catch (...)
	{
		Assert(false, "Error parsing file.");
		return false;
	}

	fin.close();
	std::cout << "Success parsing file." << std::endl;
	return true;
}

void XMLParser::SetDefaultCreator(XMLElementCreatorBase * creator)
{
	default_creator.reset(creator);
}