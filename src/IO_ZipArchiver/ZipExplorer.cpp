#include "ZipExplorer.h"
#include <Core_Exception.h>
#include <algorithm>
#include <Core_cstr.h>

#include "ZipStructures.h"

unsigned int ZipExplorer::GetCurrentFileIndex()
{
	return current_file_index;
}

unsigned int ZipExplorer::GetFileCount()
{
	return (unsigned int) total_entries_in_CD;
}

bool ZipExplorer::HasReadCurrentHeader()
{
	return (nextfile_pos >= 0);
}

bool ZipExplorer::FindCentralDirectory64()
{
	char read_buffer[1028];

	std::ios::end;
	int begin = stream.tellg();
	stream.seekg (0, std::ios::end);
	int end = stream.tellg();

	unsigned int max_distance_from_end = 0x0FFFF;
	unsigned int step_size = 0x400 + 4;
	unsigned int max_steps = max_distance_from_end / step_size;

	int seek_position = end - step_size;
	for (unsigned int step = 0; step < max_steps; ++step)
	{
		stream.seekg(seek_position);
		stream.read(read_buffer, 1028);
		for (unsigned int i = 0; i < step_size; ++i)
		{
			if (read_buffer[i] == 0x50 && read_buffer[i+1] == 0x4b && read_buffer[i+2] == 0x06 && read_buffer[i+3] == 0x06)
			{
				central_directory_location = seek_position + i;
				return true;
			}
		}
		seek_position -= step_size;
	}
	central_directory_location = -1;
	return false;
}

bool ZipExplorer::FindCentralDirectory()
{
	char read_buffer[1028];

	std::ios::end;
	int begin = stream.tellg();
	stream.seekg (0, std::ios::end);
	int end = stream.tellg();

	unsigned int max_distance_from_end = 0x0FFFF;
	unsigned int step_size = 0x400 + 4;
	unsigned int max_steps = max_distance_from_end / step_size;

	int seek_position = end - step_size;
	for (unsigned int step = 0; step < max_steps; ++step)
	{
		stream.seekg(seek_position);
		stream.read(read_buffer, 1028);
		for (unsigned int i = 0; i < step_size; ++i)
		{
			if (read_buffer[i] == 0x50 && read_buffer[i+1] == 0x4b && read_buffer[i+2] == 0x05 && read_buffer[i+3] == 0x06)
			{
				central_directory_location = seek_position + i;
				return true;
			}
		}
		seek_position -= step_size;
	}
	central_directory_location = -1;
	return false;
}

void ZipExplorer::ReadCurrentFileHeader(char * filename, unsigned int filename_buffer_size, 
					void * extra_field, unsigned int extra_buffer_size, 
					char * file_comment, unsigned int comment_buffer_size)
{
	current_file.ReadFromFile(stream, endian_abstractor, 
		filename, filename_buffer_size,
		extra_field, extra_buffer_size, 
		file_comment, comment_buffer_size);

	nextfile_pos = (int) stream.tellg() + current_file.total_size;
}

bool ZipExplorer::SeekToFirstFile()
{
	stream.seekg((unsigned int) offset_start_CD_wrt_start_disk_number);
	return true;
}



bool ZipExplorer::NextFile()
{
	if (current_file_index >= total_entries_in_CD - 1)
	{
		return false; //We have reached the end of archive.
	}

	if (!HasReadCurrentHeader())
		//We have not read the current file header yet.
	{
		current_file.SeekToNext(stream, endian_abstractor); //Reads minimum data to move to next.
		current_file_index += 1;
		return true;
	}

	stream.seekg(nextfile_pos);
	CONDITIONAL_EXCEPTION(stream.bad(), "stream.bad() == true");
	current_file_index += 1;
	nextfile_pos = -1;
	return true;
}

unsigned int ZipExplorer::GetFileDataOffset()
{
	if (!HasReadCurrentHeader())
		//We have not read the current file header yet.
	{
		unsigned int compressed; 
		unsigned int uncompressed;
		unsigned int compression;
		return current_file.ReadForFileData(stream, endian_abstractor, compressed, uncompressed, compression);
	}
	return current_file.relative_offset_of_local_header + current_file.file_name_length + current_file.extra_field_length + 28;
}

unsigned int ZipExplorer::CurrentReadFilenameOnly(char * dest, unsigned int buffer_size)
{
	return current_file.ReadFilename(stream, endian_abstractor, dest, buffer_size);
}

//Read only enough data get the offset of the file data.
unsigned int ZipExplorer::CurrentReadDataParamsOnly(unsigned int & out_compressed_size, unsigned int & out_uncompressed_size, unsigned int & out_compression)
{
	return current_file.ReadForFileData(stream, endian_abstractor, out_compressed_size, out_uncompressed_size, out_compression);
}

void ZipExplorer::Open(const char * filename)
{
	stream.open(filename, std::ios_base::binary | std::ios_base::in);

	CONDITIONAL_EXCEPTION(!stream.good(), "Error opening file.");

#ifdef INT64_MAX
	if (FindCentralDirectory64())
	{
		stream.seekg(central_directory_location);
		Zip64EndOfCentralDirectoryRecord central_directory;
		central_directory.ReadFromFile(stream, endian_abstractor);
			total_entries_in_CD = central_directory.total_entries_in_central_directory;
			offset_start_CD_wrt_start_disk_number = central_directory.offset_to_start_central_directory_start_disk;
	}
	else
	{
		if (FindCentralDirectory())
		{
			stream.seekg(central_directory_location);
			ZipEndOfCentralDirectoryRecord central_directory;
			central_directory.ReadFromFile(stream, endian_abstractor);
				total_entries_in_CD = central_directory.total_entries_in_CD;
				offset_start_CD_wrt_start_disk_number = central_directory.offset_start_CD_wrt_start_disk_number;
			CONDITIONAL_EXCEPTION(central_directory.num_of_disk != 0, "Multiple Disks Not Supported.");
		}
		else
		{
			throw Exception("CentralDirectory Not Found.");
		}
	}
#else
	if (FindCentralDirectory())
	{
		stream.seekg(central_directory_location);
		ZipEndOfCentralDirectoryRecord central_directory;
		central_directory.ReadFromFile(stream, endian_abstractor);
			total_entries_in_CD = central_directory.total_entries_in_CD;
			offset_start_CD_wrt_start_disk_number = central_directory.offset_start_CD_wrt_start_disk_number;
		CONDITIONAL_EXCEPTION(central_directory.num_of_disk != 0, "Multiple Disks Not Supported.");
	}
	else
	{
		throw Exception("CentralDirectory Not Found.");
	}
#endif

	SeekToFirstFile();
	current_file_index = 0;
}

void ZipExplorer::Close()
{
	stream.close();
	stream.clear();
}



unsigned int ZipExplorer::CurrentReadData(void * dest, unsigned int buffer_size, unsigned int data_offset, unsigned int uncompressed_length)
{
	int start = stream.tellg();
	stream.seekg(data_offset);

	int data_read = std::min<int>(buffer_size, uncompressed_length);
	stream.read((char *) dest, data_read);

	stream.seekg(start);
	return data_read;
}



bool ZipExplorer::FindFile(const char * filename, bool case_sensitive)
{
	unsigned int start = stream.tellg();
	SeekToFirstFile();
	char filename_buffer[1000];
	do
	{
		this->CurrentReadFilenameOnly(filename_buffer, 1000);

		if (case_sensitive)
		{
			if (cstr::strcasecmp(filename, filename_buffer) == 0)
			{
				return true;
			}
		}
		else
		{
			if (strcmp(filename, filename_buffer) == 0)
			{
				return true;
			}
		}
	}
	while (NextFile());
	stream.seekg(start);
	return false;
}