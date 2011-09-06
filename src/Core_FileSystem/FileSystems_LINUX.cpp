#include "PathOperations.h"

#include <unistd.h>

bool PathOperations::SetWorkingDirectoryResources()
{
	std::string path;
	PathOperations::GetWorkingPath(path);
	path.append("/Resources");
	return !chdir(path.c_str());
}

