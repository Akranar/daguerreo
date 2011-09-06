#ifndef _FILE_SYSTEM_H_
#define _FILE_SYSTEM_H_

#include <string>

class FileSystem
{
public:
	static bool GetWorkingPath(std::string & rtn);
    static bool SetWorkingDirectoryResources();
	static bool GetWorkingPathForwardSlashes(std::string & rtn);
};

#endif
