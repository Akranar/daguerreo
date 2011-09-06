#ifndef _DESC_SHADER_PROGRAM_H_
#define _DESC_SHADER_PROGRAM_H_

#include <string>
#include <fstream>
#include <Core_Exception.h>

class DescShaderProgram
{
public:
	std::string vertex_program_string;
	std::string fragment_program_string;

	void ReadFiles(const char * vs_file, const char * fs_file)
	{
		std::fstream stream;
		std::string line;
		
		stream.open(vs_file, std::ios_base::in);
		CONDITIONAL_EXCEPTION(!stream.good(), "Failed to Open File.");
		while (!stream.eof())
		{
			std::getline(stream, line);
			vertex_program_string.append(line);
			vertex_program_string.push_back('\n');
		}
		stream.close();

		stream.clear();

		stream.open(fs_file, std::ios_base::in);
		while (!stream.eof())
		{
			std::getline(stream, line);
			fragment_program_string.append(line);
			fragment_program_string.push_back('\n');
		}
		stream.close();
	}

	const char * GetProgramString(int domain) const
	{
		if (domain == 0)
		{
			return vertex_program_string.c_str();
		}
		else if (domain == 1)
		{
			return fragment_program_string.c_str(); 
		}
		return 0;
	}

	unsigned int GetProgramStringLength(int domain) const
	{
		if (domain == 0)
		{
			return vertex_program_string.length();
		}
		else if (domain == 1)
		{
			return fragment_program_string.length(); 
		}
		return 0;
	}
};


#endif