#ifndef _EXAMPLE_ZIPPED_PNGS_H_
#define _EXAMPLE_ZIPPED_PNGS_H_

#include <Core_SynchronousFileIO.h>
#include "ExLib_ZipArchiver.h"
#include "PNGReader.h"
#include "PNGReadStream.h"
#include "PNGWriter.h"

void ExampleZippedPNGs()
{
	char image_data [800000];
	char uncompressed_data [800000];

	ZipArchiver zip;
	zip.BuildDirectory("Resources.zip");
	const ZipFileInfo & file_info = zip.GetFileInfo("boy_10.png");

	SynchronousFileIO::Read("Resources.zip", file_info.GetFileOffset(), image_data, file_info.GetSizeCompressed());
	if (file_info.GetCompressionMethod() == 8)
	{
		if (ZipFileInflate::Inflate(uncompressed_data, file_info.GetSizeUncompressed(), image_data, file_info.GetSizeCompressed(), false))
		{
			SynchronousFileIO::Write("file.png", 0, uncompressed_data, file_info.GetSizeUncompressed());
		}
	}
	else
	{
		//SynchronousFileIO::Write("file.png", 0, image_data, file_info.GetSizeCompressed());
		if (PNGReader::IsPNG(image_data, 8))
		{
			PNGStream_InMemory png_stream;
			png_stream.SetReadPointer(image_data);

			PNGReader reader;
			reader.SetReadStream(&png_stream);
			reader.ReadPNGHeader();
			char * row_ptrs [1024];
			reader.ReadImage(uncompressed_data, row_ptrs, true);

			PNGWriter writer;
			writer.WriteRGB("file.png", 512, 512, uncompressed_data, "My Test Image", true);

		}
	}
}

#endif