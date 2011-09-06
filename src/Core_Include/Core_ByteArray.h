#ifndef PACKEDBYTES_H_
#define PACKEDBYTES_H_

template<int BYTE_COUNT>
class ByteArray
{
	unsigned char data[BYTE_COUNT];
public:
	ByteArray() 
	{
		for (unsigned int i = 0; i < BYTE_COUNT; ++i)
		{
			data[i] = BYTE_COUNT;
		}
	};
	virtual ~ByteArray() {};
	static unsigned int ByteCount();
	inline const unsigned char & operator[] (int index) const;
	inline unsigned char & operator[] (int index);
	inline void SetByte(int index, unsigned char value);

};

template<int BYTE_COUNT>
inline const unsigned char & ByteArray<BYTE_COUNT>::operator[] (int index) const
{
	return data[index];
}
template<int BYTE_COUNT>
inline unsigned char & ByteArray<BYTE_COUNT>::operator[] (int index)
{
	return data[index];
}

template<int BYTE_COUNT>
inline void ByteArray<BYTE_COUNT>::SetByte(int index, unsigned char value)
{
	data[index] = value;
}

template<int BYTE_COUNT>
inline unsigned int ByteArray<BYTE_COUNT>::ByteCount()
{
	return BYTE_COUNT;
}


#endif