#include "FileSystem.h"
//#include <stdlib.h>
#include <direct.h>

bool FileSystem::SetWorkingDirectoryResources()
{
	std::string path;
	FileSystem::GetWorkingPath(path);
	path.append("/Resources");
	return !_chdir(path.c_str());
}
