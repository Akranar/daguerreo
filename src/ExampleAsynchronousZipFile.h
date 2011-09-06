#ifndef _EXAMPLE_ASYNCHRONOUS_ZIP_FILE_H_
#define _EXAMPLE_ASYNCHRONOUS_ZIP_FILE_H_

#include <Core_AsynchronousFile.h>
#include <Core_SynchronousFileIO.h>
#include "ExLib_ZipArchiver.h"
#include "ZipFileInflate.h"

void ExampleAsynchronousZipFile()
{
	char data [200000];
	char uncompressed[200000];
	ZipArchiver zip;
	zip.BuildDirectory("Resources.zip");
	const ZipFileInfo & file_info = zip.GetFileInfo("GLSL/Texture2D.glslv");

#define USE_ASYNC
#ifdef USE_ASYNC

	AsynchronousFile async;
	unsigned int offset = file_info.GetFileOffset();
	unsigned int data_size = file_info.GetSizeCompressed();
	char * adjusted_data = data;
	async.AdjustOffsetAndSizes(offset, adjusted_data, data_size);

	bool ready = async.InitFile("Resources.zip", offset);
	bool reading = async.StartFileRead(data, data_size);
	while (reading)
	{
		if (async.CheckProgress(false))
		{
			break;
		}
	}
	async.Destroy();

	if (file_info.GetCompressionMethod() == 8)
	{
		if (ZipFileInflate::Inflate(uncompressed, file_info.GetSizeUncompressed(), adjusted_data, file_info.GetSizeCompressed(), true))
		{
			std::cout << uncompressed << std::endl;
		}
	}
	else
	{
		std::cout << data << std::endl;
	}
	
#else

	SynchronousFileIO::Read("Resources.zip", file_info.location_in_zip, data, file_info.size_compressed);
	if (file_info.compression_method == 8)
	{
		if (ZipFileInflate::Inflate(uncompressed, file_info.size_uncompressed, data, file_info.size_compressed, true))
		{
			std::cout << uncompressed << std::endl;
		}
	}
	else
	{
		std::cout << data << std::endl;
	}
	

#endif
}

#endif