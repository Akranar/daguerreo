#include "FileSystem.h"

#include <stdio.h>  /* defines FILENAME_MAX */
#ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
#endif

bool FileSystem::GetWorkingPath(std::string & rtn)
{
	char * current_path = new char [FILENAME_MAX];
	if (!GetCurrentDir(current_path, FILENAME_MAX))
	{
		delete current_path;
		return false;
	}
	rtn.assign(current_path);
	delete [] current_path;
	return true;
}

bool FileSystem::GetWorkingPathForwardSlashes(std::string & rtn)
{
	if (!FileSystem::GetWorkingPath(rtn)) return false;
	for (unsigned int i = 0; i < rtn.length(); ++i)
	{
		if (rtn[i] == '\\')
		{
			rtn[i] = '/';
		}
	}
	return true;
}