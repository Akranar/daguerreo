#ifndef _ZIP_ARCHIVER_H_
#define _ZIP_ARCHIVER_H_

#include <map>

#include "ZipFileInfo.h"
#include <string>

class ZipArchiver
{
public:
	
private:
	std::map <std::string, ZipFileInfo> zip_directory;
	std::string zip_filename;
public:
	ZipArchiver(){};
	void BuildDirectory(const char * filename);
	unsigned int ReadFileData(const char * filename, void * dest, unsigned int dest_length);
	const ZipFileInfo & GetFileInfo(const char * filename) const;
	const char * GetArchiveFilename() const;
};

#endif