#include "ZipFileInflate.h"
#include "ExLib_ZLIB.h"

bool ZipFileInflate::Inflate(char * dest, unsigned int dest_length, char * src, unsigned int src_length, bool null_terminate)
{
	z_stream z_stream;
	z_stream.zalloc = Z_NULL;
    z_stream.zfree = Z_NULL;
    z_stream.opaque = Z_NULL;

	z_stream.next_in = (Bytef*) src;
	z_stream.avail_in = src_length;
	z_stream.next_out = (Bytef*) dest;
	z_stream.avail_out = dest_length;

	//http://www.zlib.net/manual.html
	//	windowBits can also be –8..–15 for raw inflate. 
	//	In this case, -windowBits determines the window size. 
	//	inflate() will then process raw deflate data, not looking for a zlib or gzip header, 
	//	not generating a check value, and not looking for any check values for comparison at the end of the stream. 
	//	This is for use with other formats that use the deflate compressed data format such as zip.
    int ret = inflateInit2(&z_stream, -15);
	if (ret == Z_OK) 
	{
		int result = inflate(&z_stream, Z_SYNC_FLUSH);
		unsigned int bytes_written = z_stream.total_out;
		if (null_terminate)
		{
			dest[bytes_written] = 0;
		}
		inflateEnd(&z_stream);
		return true;
	}
	return false;
}