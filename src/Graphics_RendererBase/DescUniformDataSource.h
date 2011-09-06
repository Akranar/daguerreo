#ifndef _DESC_UNIFORM_DATA_SOURCE_H_
#define _DESC_UNIFORM_DATA_SOURCE_H_

#include "Enumerants.h"

class DescUniformDataSource
{
public:
	/*
	enum DataType
	{
		FLOAT,
		DOUBLE,
		INT
	};
	*/
private:
	union
	{
		float * float_data;
		double * double_data;
		int * int_data;
		void * void_data;
	} data;
	unsigned short stride;
	unsigned short array_length;
	unsigned char type; //3 possible values
	bool one_time_update;
	bool column_major;
	unsigned char channels; //1 to 16
public:
	inline bool IsColumnMajor() const;
	inline bool IsOneTimeUpdate() const;
	inline unsigned char GetChannelsCount() const;
	inline unsigned short GetStride() const;
	inline unsigned short GetArrayLength() const;
	inline unsigned char GetDataType() const;
	inline float * GetFloatData() const;
	inline double * GetDoubleData() const;
	inline int * GetIntData() const;

	inline void SetColumnMajor(bool in);
	inline void SetOneTimeUpdate(bool one_time);
	inline void SetChannelsCount(unsigned char in);
	inline void SetStride(unsigned short in);
	inline void SetArrayLength(unsigned short in_array_length);
	inline void SetFloatData(float * in);
	inline void SetDoubleData(double * in);
	inline void SetIntData(int * in);

	inline void Set(const DescUniformDataSource & to_copy);
	inline void Set(bool is_column_major, bool one_time, unsigned char channels_count, unsigned short stride, unsigned short array_length, float * float_data);
	inline void Set(bool is_column_major, bool one_time, unsigned char channels_count, unsigned short stride, unsigned short array_length, double * double_data);
	inline void Set(bool is_column_major, bool one_time, unsigned char channels_count, unsigned short stride, unsigned short array_length, int * int_data);

	inline bool ShouldUpdate() const;
};

inline bool DescUniformDataSource::IsColumnMajor() const
{
	return column_major;
}
inline bool DescUniformDataSource::IsOneTimeUpdate() const
{
	return one_time_update;
}
inline unsigned char DescUniformDataSource::GetChannelsCount() const
{
	return channels;
}
inline unsigned short DescUniformDataSource::GetStride() const
{
	return stride;
}
inline unsigned short DescUniformDataSource::GetArrayLength() const
{
	return array_length;
}
inline unsigned char DescUniformDataSource::GetDataType() const
{
	return type;
}
inline float * DescUniformDataSource::GetFloatData() const
{
	return data.float_data;
}
inline double * DescUniformDataSource::GetDoubleData() const
{
	return data.double_data;
}
inline int * DescUniformDataSource::GetIntData() const
{
	return data.int_data;
}
inline void DescUniformDataSource::SetColumnMajor(bool in)
{
	column_major = in;
}
inline void DescUniformDataSource::SetOneTimeUpdate(bool one_time)
{
	one_time_update = one_time;
}
inline void DescUniformDataSource::SetChannelsCount(unsigned char in)
{
	channels = in;
}
inline void DescUniformDataSource::SetStride(unsigned short in)
{
	stride = in;
}
inline void DescUniformDataSource::SetArrayLength(unsigned short in_array_length)
{
	array_length = in_array_length;
}
inline void DescUniformDataSource::SetFloatData(float * in)
{
	data.float_data = in;
	type = DT_FLOAT;
}
inline void DescUniformDataSource::SetDoubleData(double * in)
{
	data.double_data = in;
	type = DT_DOUBLE;
}
inline void DescUniformDataSource::SetIntData(int * in)
{
	data.int_data = in;
	type = DT_INT;
}
inline void DescUniformDataSource::Set(const DescUniformDataSource & to_copy)
{
	*this = to_copy;
}
inline void DescUniformDataSource::Set(bool is_column_major, bool one_time, unsigned char channels_count, unsigned short stride, unsigned short array_length, float * float_data)
{
	SetColumnMajor(is_column_major);
	SetOneTimeUpdate(one_time);
	SetChannelsCount(channels_count);
	SetStride(stride);
	SetFloatData(float_data);
}
inline void DescUniformDataSource::Set(bool is_column_major, bool one_time, unsigned char channels_count, unsigned short stride, unsigned short array_length, double * double_data)
{
	SetColumnMajor(is_column_major);
	SetOneTimeUpdate(one_time);
	SetChannelsCount(channels_count);
	SetStride(stride);
	SetDoubleData(double_data);
}
inline void DescUniformDataSource::Set(bool is_column_major, bool one_time, unsigned char channels_count, unsigned short stride, unsigned short array_length, int * int_data)
{
	SetColumnMajor(is_column_major);
	SetOneTimeUpdate(one_time);
	SetChannelsCount(channels_count);
	SetStride(stride);
	SetIntData(int_data);
}
inline bool DescUniformDataSource::ShouldUpdate() const
{
	return data.void_data != 0;
}



#endif