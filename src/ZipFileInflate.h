#ifndef _ZIP_FILE_INFLATE_H_
#define _ZIP_FILE_INFLATE_H_

class ZipFileInflate
{
public:
	static bool Inflate(char * dest, unsigned int dest_length, char * src, unsigned int src_length, bool null_terminate);
};

#endif