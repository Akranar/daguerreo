#ifndef _ZIP_STRUCTURES_H_
#define _ZIP_STRUCTURES_H_

#ifdef _MSC_VER 
#if _MSC_VER < 1600
#include <Core_pstdint.h>
#endif //_MSC_VER < 1600
#endif // _MSC_VER 

#include <fstream>

class EndianAbstractor;

class ZipLocalFileHeader //A
{
public:
	uint32_t local_file_header_signature;
	uint16_t version_needed_to_extract;
	uint16_t general_purpose_bit_flag;
	uint16_t compression_method;
	uint16_t last_mod_file_time;
	uint16_t last_mod_file_date;
	uint32_t crc_32;
	uint32_t compressed_size;
	uint32_t uncompressed_size;
	uint16_t file_name_length;
	uint16_t extra_field_length;
};

class ZipDataDescriptor
{
public:
	uint32_t crc_32;
	uint32_t compressed_size;
	uint32_t uncompressed_size;
};

class ZipArchiveExtraDataRecord
{
public:
	uint32_t archive_extra_data_signature;
	uint32_t extra_field_length;
	uint32_t extra_field_data;
};

class ZipCentralDirectoryStructure
{
public:
	uint32_t central_file_header_signature;
	uint16_t version_made_by;
	uint16_t version_needed_to_extract;
	uint16_t general_purpose_bit_flag;
	uint16_t compression_method;
	uint16_t last_mod_file_time;
	uint16_t last_mode_file_date;
	uint32_t crc_32;
	uint32_t compressed_size;
	uint32_t uncompressed_size;
	uint16_t file_name_length;
	uint16_t extra_field_length;
	uint16_t file_comment_length;
	uint16_t disk_number_start;
	uint16_t internal_file_attributes;
	uint32_t external_file_attributes;
	uint32_t relative_offset_of_local_header;
	uint32_t header_signature;
	uint16_t size_of_data;

	uint32_t total_size;

	void ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor, 
		char * filename, unsigned int filename_buffer_size, 
		void * extra_field, unsigned int extra_buffer_size, 
		char * file_comment, unsigned int comment_buffer_size);

	//Read Filename Only
	unsigned int ReadFilename(std::fstream & stream, EndianAbstractor & endian_abstractor, 
		char * filename, unsigned int filename_buffer_size);

	//Read only enough data to get to the next file.
	void SeekToNext(std::fstream & stream, EndianAbstractor & endian_abstractor);

	//Read only enough data get the offset of the file data.
	unsigned int ReadForFileData(std::fstream & stream, EndianAbstractor & endian_abstractor, 
		unsigned int & out_compressed_size, unsigned int & out_uncompressed_size, unsigned int & compression);
};

class ZipCentralDirectoryDigitalSignature
{
public:
	uint32_t header_signature;
	uint16_t size_of_data;
};


class ZipEndOfCentralDirectoryRecord
{
public:
	uint32_t end_of_central_dir_signature;
	uint16_t num_of_disk;
	uint16_t num_of_disk_with_start_of_CD;
	uint16_t total_entries_in_CD_this_disk;
	uint16_t total_entries_in_CD;
	uint32_t central_directory_size;
	uint32_t offset_start_CD_wrt_start_disk_number;
	uint16_t zip_file_comment_length;


	void ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor);
};

#ifdef INT64_MAX

class Zip64EndOfCentralDirectoryRecord
{
public:
	uint32_t signature;
	uint64_t size_of_end_of_central_directory_record;
	uint16_t version_made_by;
	uint16_t version_needed_to_extract;
	uint32_t disk_number;
	uint32_t disk_number_with_start_of_central_directory;
	uint64_t total_entries_in_central_directory_on_this_disk;
	uint64_t total_entries_in_central_directory;
	uint64_t size_of_central_directory;
	uint64_t offset_to_start_central_directory_start_disk;

	void ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor);
};

class Zip64EndOfCentralDirectoryLocator
{
public:
	uint32_t zip64_end_of_CDL_signature;
	uint32_t num_of_disk_with_zip64_end_CDL_start;
	uint64_t relative_offset_zip64_EOCDL;
	uint32_t total_number_of_disks;

	void ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor);
};


#endif


#endif