#ifndef _ZIP_FILE_BROWSER_H_
#define _ZIP_FILE_BROWSER_H_

#include "ZipStructures.h"
#include <Core_EndianAbstractor.h>
#include <fstream>

#ifdef _MSC_VER 
#if _MSC_VER < 1600
#include <Core_pstdint.h>
#endif //_MSC_VER < 1600
#endif // _MSC_VER 

class ZipExplorer
{
	std::fstream stream;
	EndianAbstractor endian_abstractor;
	unsigned int central_directory_location;
	
	ZipCentralDirectoryStructure current_file;
	int nextfile_pos; //negative if uninitialized;
	unsigned int current_file_index;

	bool FindCentralDirectory();
	bool FindCentralDirectory64();


	//Data from EndOfCentralDirectory
#ifdef INT64_MAX
	uint64_t total_entries_in_CD;
	uint64_t offset_start_CD_wrt_start_disk_number;
#else
	uint32_t total_entries_in_CD;
	uint32_t offset_start_CD_wrt_start_disk_number;
#endif
	
public:
	ZipExplorer()
		:
	endian_abstractor(true)
	{
	}

	unsigned int GetCurrentFileIndex();
	unsigned int GetFileCount();

	bool SeekToFirstFile();
	bool NextFile();

	unsigned int CurrentReadFilenameOnly(char * dest, unsigned int buffer_size);
	unsigned int CurrentReadDataParamsOnly(unsigned int & out_compressed_size, unsigned int & out_uncompressed_size, unsigned int & out_compression);

	bool HasReadCurrentHeader();
	void ReadCurrentFileHeader(char * filename = 0, unsigned int filename_buffer_size = 0, 
						void * extra_field = 0, unsigned int extra_buffer_size = 0, 
						char * file_comment = 0, unsigned int comment_buffer_size = 0);

	unsigned int GetFileDataOffset();

	unsigned int CurrentReadData(void * dest, unsigned int buffer_size, unsigned int data_offset, unsigned int uncompressed_length);
	
	void Open(const char * filename);
	void Close();

	bool FindFile(const char * filename, bool case_sensitive);
};

#endif