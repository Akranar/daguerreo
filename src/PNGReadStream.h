#ifndef _PNG_READ_STREAM_H_
#define _PNG_READ_STREAM_H_

class PNGReadStream
{
public:
	virtual void Read(char * dest, unsigned int bytes_to_read) = 0;
};

class PNGStream_InMemory: public PNGReadStream
{
	const char * read_ptr;
public:
	void SetReadPointer(const char * ptr)
	{
		read_ptr = ptr;
	}
	void Read(char * dest, unsigned int bytes_to_read)
	{
		memcpy(dest, read_ptr, bytes_to_read);
		read_ptr += bytes_to_read;
	}
};

#endif