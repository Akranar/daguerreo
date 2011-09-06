#include "ZipArchiver.h"
#include "ZipExplorer.h"
#include <Core_Exception.h>
#include <fstream>



void ZipArchiver::BuildDirectory(const char * filename)
{
	ZipExplorer zip_explorer;
	zip_explorer.Open(filename);
	zip_filename = filename;
	zip_explorer.SeekToFirstFile();
	char filename_buffer[1000];
	
	do
	{
		ZipFileInfo current_file;
		current_file.location_in_zip = 
			zip_explorer.CurrentReadDataParamsOnly(
				current_file.size_compressed, 
				current_file.size_uncompressed, 
				current_file.compression_method);

		zip_explorer.CurrentReadFilenameOnly(filename_buffer, 1000);

		zip_directory.insert(std::make_pair(filename_buffer, current_file));

	} while (zip_explorer.NextFile());
	zip_explorer.Close();
}
unsigned int ZipArchiver::ReadFileData(const char * filename, void * dest, unsigned int dest_length)
{
	std::map<std::string, ZipFileInfo>::iterator search_result = zip_directory.find(filename);
	CONDITIONAL_EXCEPTION(search_result == zip_directory.end(), "File not found in Zip File Directory");

	unsigned int data_location = search_result->second.location_in_zip;
	unsigned int compressed_size = search_result->second.size_compressed;
	unsigned int data_to_read = std::min<int>(dest_length, compressed_size);

	std::fstream stream(zip_filename.c_str(), std::ios_base::binary | std::ios_base::in);
	CONDITIONAL_EXCEPTION(stream.bad(), "Zip archive file could not be opened");
	stream.seekg(data_location);
	stream.read((char *) dest, data_to_read);
	stream.close();
	return data_to_read;
}
const ZipFileInfo & ZipArchiver::GetFileInfo(const char * filename) const
{
	std::map<std::string, ZipFileInfo>::const_iterator search_result = zip_directory.find(filename);
	CONDITIONAL_EXCEPTION(search_result == zip_directory.end(), "File not found in Zip File Directory");
	return search_result->second;
}
const char * ZipArchiver::GetArchiveFilename() const
{
	return zip_filename.c_str();
}