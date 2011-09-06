#ifndef _SYNCHRONOUS_FILE_IO_H_
#define _SYNCHRONOUS_FILE_IO_H_

#include <fstream>

class SynchronousFileIO
{
public:
	static bool Read(const char * filename, unsigned int file_offset, char * dest, unsigned int length)
	{
		std::fstream stream(filename, std::ios_base::binary | std::ios_base::in);
		if (stream.bad())
		{
			stream.close();
			return false;
		}
		stream.seekg(file_offset);
		stream.read(dest, length);
		stream.close();
		return true;
	}

	static bool Write(const char * filename, unsigned int file_offset, char * src, unsigned int length)
	{
		std::fstream stream(filename, std::ios_base::binary | std::ios_base::out);
		if (stream.bad())
		{
			stream.close();
			return false;
		}
		stream.seekg(file_offset);
		stream.write(src, length);
		stream.close();
		return true;
	}
};

#endif