#ifndef _BITFIELD_H_
#define _BITFIELD_H_

#include <limits.h>

template <typename INT_TYPE>
class Bitfield
{
	INT_TYPE data;
public:
	Bitfield() { data = 0;};
	Bitfield(INT_TYPE data)
		: data(data)
	{
	};
	~Bitfield() {};
	static INT_TYPE BitCount();
	inline int operator[] (int index) const;
	inline void SetBit(int index, bool on);
	inline void ToggleBit(int index);
};

template <typename INT_TYPE>
inline int Bitfield<INT_TYPE>::operator[] (int index) const
{
	return (data >> index) & 0x1;
}

template <typename INT_TYPE>
inline void Bitfield<INT_TYPE>::SetBit(int index, bool on)
{
	if (on)
	{
		data |= (0x1 << index);
		return;
	}
	data &= ~(1 << index);
}

template <typename INT_TYPE>
inline void Bitfield<INT_TYPE>::ToggleBit(int index)
{
	data ^= (1 << index);
}

template <typename INT_TYPE>
INT_TYPE Bitfield<INT_TYPE>::BitCount()
{
	return sizeof(INT_TYPE) * CHAR_BIT;
}



#endif