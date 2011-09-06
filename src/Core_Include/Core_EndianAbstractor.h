#ifndef _DATA_IO_H_
#define _DATA_IO_H_

#include <string>
#include <fstream>


#define DATA_IO_LITTLE_ENDIAN 1
#define DATA_IO_BIG_ENDIAN 0

//#define DATA_IO_OUTPUT DATA_IO_BIG_ENDIAN
//#define DATA_IO_OUTPUT DATA_IO_LITTLE_ENDIAN

class EndianAbstractor
{
	bool io_little_endian;
	bool machine_little_endian;
public:
	EndianAbstractor(bool io_little_endian)
		:
	io_little_endian(io_little_endian),
	machine_little_endian(LittleEndian())
	{
	}

	template <typename INT_TYPE>
	bool WriteIntType(std::fstream & stream, INT_TYPE value);

	template <typename INT_TYPE>
	bool WriteIntType(std::fstream & stream, INT_TYPE value, unsigned int byte_count);

	template <typename INT_TYPE>
	INT_TYPE ReadIntType(std::fstream & stream);

	template <typename INT_TYPE>
	INT_TYPE ReadIntType(std::fstream & stream, unsigned int byte_count);

	inline bool LittleEndian();
};

template <typename INT_TYPE>
bool EndianAbstractor::WriteIntType(std::fstream & stream, INT_TYPE value)
{
	char * value_char = (char *) &value;
	if (machine_little_endian == io_little_endian)
	{
		for (int size = 0; size < sizeof(INT_TYPE); ++size)
		{
			stream.put(value_char[size]);
		}
	}
	else
	{
		for (int size = sizeof(INT_TYPE)-1; size >= 0; --size)
		{
			stream.put(value_char[size]);
		}
	}
	return stream.good();
};

template <typename INT_TYPE>
INT_TYPE EndianAbstractor::ReadIntType(std::fstream & stream)
{
	INT_TYPE rtn = 0;
	char * rtn_char = (char *) &rtn;
	if (machine_little_endian == io_little_endian)
	{
		for (unsigned int size = 0; size < sizeof(INT_TYPE); ++size)
		{
			rtn_char[size] = stream.get();
		}
	}
	else
	{
		for (int size = sizeof(INT_TYPE)-1; size >= 0; --size)
		{
			rtn_char[size] = stream.get();
		}
	}
	return rtn;
};

template <typename INT_TYPE>
bool EndianAbstractor::WriteIntType(std::fstream & stream, INT_TYPE value, unsigned int byte_count)
{
	char * value_char = (char *) &value;
	if (machine_little_endian == io_little_endian)
	{
		for (unsigned int size = 0; size < byte_count; ++size)
		{
			stream.put(value_char[size]);
		}
	}
	else
	{
		for (int size = byte_count-1; size >= 0; --size)
		{
			stream.put(value_char[size]);
		}
	}
	return stream.good();
};

template <typename INT_TYPE>
INT_TYPE EndianAbstractor::ReadIntType(std::fstream & stream, unsigned int byte_count)
{
	INT_TYPE rtn = 0;
	char * rtn_char = (char *) &rtn;
	if (machine_little_endian == io_little_endian)
	{
		for (unsigned int size = 0; size < byte_count; ++size)
		{
			rtn_char[size] = stream.get();
		}
	}
	else
	{
		for (unsigned int size = byte_count; size > 0; --size)
		{
			rtn_char[size-1] = stream.get();
		}
	}
	return rtn;
};

//WARNING: Requires sizeof(char) != sizeof(int)
/*
inline bool EndianAbstractor::LittleEndian()
{
	int num = 1;
	return (*(char *)&num == 1);
}
*/
inline bool EndianAbstractor::LittleEndian()
{
	union {
        unsigned int i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] != 1; 
}



#endif