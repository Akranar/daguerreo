#include "ZipStructures.h"
#include <Core_EndianAbstractor.h>
#include <Core_Exception.h>

void ZipEndOfCentralDirectoryRecord::ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor)
{
	end_of_central_dir_signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	num_of_disk = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	num_of_disk_with_start_of_CD = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	total_entries_in_CD_this_disk = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	total_entries_in_CD = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	central_directory_size = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	offset_start_CD_wrt_start_disk_number = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	zip_file_comment_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
};

#ifdef INT64_MAX


void Zip64EndOfCentralDirectoryRecord::ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor)
{
	signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	size_of_end_of_central_directory_record = endian_abstractor.ReadIntType<uint64_t>(stream, 8);
	version_made_by = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	version_needed_to_extract = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	disk_number = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	disk_number_with_start_of_central_directory = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	total_entries_in_central_directory_on_this_disk = endian_abstractor.ReadIntType<uint64_t>(stream, 8);
	total_entries_in_central_directory = endian_abstractor.ReadIntType<uint64_t>(stream, 8);
	size_of_central_directory = endian_abstractor.ReadIntType<uint64_t>(stream, 8);
	offset_to_start_central_directory_start_disk = endian_abstractor.ReadIntType<uint64_t>(stream, 8);
}

void Zip64EndOfCentralDirectoryLocator::ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor)
{
	zip64_end_of_CDL_signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	num_of_disk_with_zip64_end_CDL_start = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	relative_offset_zip64_EOCDL = endian_abstractor.ReadIntType<uint64_t>(stream, 8);
	total_number_of_disks = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
}


#endif

void ZipCentralDirectoryStructure::ReadFromFile(std::fstream & stream, EndianAbstractor & endian_abstractor, 
	char * filename, unsigned int filename_buffer_size, 
	void * extra_field, unsigned int extra_buffer_size, 
	char * file_comment, unsigned int comment_buffer_size)
{
	unsigned int start = stream.tellg();
	central_file_header_signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	CONDITIONAL_EXCEPTION(central_file_header_signature != 0x02014b50, "Central Directory Structure Signature Mismatch");
	version_made_by = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	version_needed_to_extract = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	general_purpose_bit_flag = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	compression_method = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	last_mod_file_time = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	last_mode_file_date = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	crc_32 = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	compressed_size = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	uncompressed_size = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	file_name_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	extra_field_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	file_comment_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	disk_number_start = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	internal_file_attributes = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	external_file_attributes = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	relative_offset_of_local_header = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	if (filename)
	{
		int characters_read = std::min<int>(filename_buffer_size-1, file_name_length);
		stream.read(filename, characters_read);
		filename[characters_read] = 0;
	}
	else
	{
		stream.seekg((int) stream.tellg() + file_name_length);
	}
	if (extra_field)
	{
		int characters_read = std::min<int>(filename_buffer_size-1, file_name_length);
		stream.read((char *)extra_field, extra_field_length);
		((char*)extra_field)[characters_read] = 0;
	}
	else
	{
		stream.seekg((int) stream.tellg() + extra_field_length);
	}
	if (file_comment)
	{
		int characters_read = std::min<int>(filename_buffer_size-1, file_name_length);
		stream.read(file_comment, file_comment_length);
		file_comment[characters_read] = 0;
	}
	else
	{
		stream.seekg((int) stream.tellg() + file_comment_length);
	}
	unsigned int end = stream.tellg();
	total_size = end - start;
	stream.seekg(start);
	
}

//Read Filename Only
unsigned int ZipCentralDirectoryStructure::ReadFilename(std::fstream & stream, EndianAbstractor & endian_abstractor, 
	char * filename, unsigned int filename_buffer_size)
{
	unsigned int start = stream.tellg();
	central_file_header_signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	CONDITIONAL_EXCEPTION(central_file_header_signature != 0x02014b50, "Central Directory Structure Signature Mismatch");
	stream.seekg(((int) stream.tellg()) + 24);
	file_name_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	stream.seekg(((int) stream.tellg()) + 16);
	if (filename)
	{
		int characters_read = std::min<int>(filename_buffer_size-1, file_name_length);
		stream.read(filename, characters_read);
		filename[characters_read] = 0;
	}
	stream.seekg(start);
	return file_name_length;
}


//Read only enough data to get to the next file.
void ZipCentralDirectoryStructure::SeekToNext(std::fstream & stream, EndianAbstractor & endian_abstractor)
{
	unsigned int start = stream.tellg();
	central_file_header_signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	CONDITIONAL_EXCEPTION(central_file_header_signature != 0x02014b50, "Central Directory Structure Signature Mismatch");
	stream.seekg(((int) stream.tellg()) + 24);

	file_name_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	extra_field_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	file_comment_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	int next = start + 46 + file_name_length + extra_field_length + file_comment_length;
	stream.seekg(next);
}

//Read only necessary info to retrieve compressed data (Reads local file header).
unsigned int ZipCentralDirectoryStructure::ReadForFileData(std::fstream & stream, EndianAbstractor & endian_abstractor, 
														   unsigned int & out_compressed_size, unsigned int & out_uncompressed_size, unsigned int & compression)
{
	int start = stream.tellg();

	central_file_header_signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	CONDITIONAL_EXCEPTION(central_file_header_signature != 0x02014b50, "Central Directory Structure Signature Mismatch");
	stream.seekg(((int) stream.tellg()) + 38);
	relative_offset_of_local_header = endian_abstractor.ReadIntType<uint32_t>(stream, 4);

	stream.seekg(relative_offset_of_local_header);
	uint32_t local_signature = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	CONDITIONAL_EXCEPTION(local_signature != 0x04034b50, "Local File Header Signature Mismatch");
	uint16_t version_needed = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	stream.seekg(((int) stream.tellg()) + 2);
	uint16_t compression_method = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	stream.seekg(((int) stream.tellg()) + 8);
	uint32_t compressed_size = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	uint32_t uncompressed_size = endian_abstractor.ReadIntType<uint32_t>(stream, 4);
	uint16_t file_name_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);
	uint16_t extra_field_length = endian_abstractor.ReadIntType<uint16_t>(stream, 2);

	int data_position = ((int)stream.tellg()) + file_name_length + extra_field_length;
	stream.seekg(start);

	out_compressed_size = compressed_size;
	out_uncompressed_size = uncompressed_size;
	compression = compression_method;
	return data_position;
}






