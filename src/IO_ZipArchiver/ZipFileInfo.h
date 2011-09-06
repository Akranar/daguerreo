#ifndef _ZIP_FILE_INFO_H_
#define _ZIP_FILE_INFO_H_

class ZipArchiver;

class ZipFileInfo
{
friend class ZipArchiver;
	unsigned int compression_method;
	unsigned int size_compressed;
	unsigned int size_uncompressed;
	unsigned int location_in_zip;
public:
	inline unsigned int GetCompressionMethod() const;
	inline unsigned int GetSizeCompressed() const;
	inline unsigned int GetSizeUncompressed() const;
	inline unsigned int GetFileOffset() const;
};

inline unsigned int ZipFileInfo::GetCompressionMethod() const
{
	return compression_method;
}
inline unsigned int ZipFileInfo::GetSizeCompressed() const
{
	return size_compressed;
}
inline unsigned int ZipFileInfo::GetSizeUncompressed() const
{
	return size_uncompressed;
}
inline unsigned int ZipFileInfo::GetFileOffset() const
{
	return location_in_zip;
}


#endif