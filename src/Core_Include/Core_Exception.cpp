#include "Core_Exception.h"
#include <iostream>

const char * Exception::GetReason() const
{
	return reason;
}
void Exception::PrintReason() const
{
	std::cout << reason << std::endl;
}