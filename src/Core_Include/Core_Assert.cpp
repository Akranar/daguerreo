#include "Core_Assert.h"

bool MyAssert(bool value, const char * text, const char * file, int line)
{
	std::cout << "Assert Failed: " << std::endl;
	std::cout << "	Text: " << text << std::endl;
	std::cout << "	File: " << file << std::endl;
	std::cout << "	Line: " << line << std::endl;
	return true;
}