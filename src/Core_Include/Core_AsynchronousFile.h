#ifndef _ASYNCHRONOUS_FILE_H_
#define _ASYNCHRONOUS_FILE_H_

#include <cstring>
#include <iostream>

#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)

#include <windows.h>

class AsynchronousFile
{
	HANDLE file_handle;
	OVERLAPPED overlapped;
	unsigned int bytes_read;
public:
	bool InitFile(const char * filename, unsigned int file_start_offset)
	{
		file_handle = 0;
		bytes_read = 0;
		memset(&overlapped, 0, sizeof(overlapped));
		overlapped.Offset = file_start_offset;
		overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

		file_handle = CreateFile(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING | FILE_ATTRIBUTE_NORMAL, 0);
		return file_handle != 0;
	};

	long GetFileSize(const char *fileName)
	{
		//http://blog.kowalczyk.info/article/Get-file-size-under-windows.html
		BOOL                        fOk;
		WIN32_FILE_ATTRIBUTE_DATA   fileInfo;

		if (NULL == fileName)
			return -1;

		fOk = GetFileAttributesEx(fileName, GetFileExInfoStandard, (void*)&fileInfo);
		if (!fOk)
			return -1;
		assert(0 == fileInfo.nFileSizeHigh);
		return (long)fileInfo.nFileSizeLow;
	}

	unsigned int GetSectorSize()
	{
		DWORD sectors_per_cluster;
		DWORD bytes_per_sector;
		DWORD number_of_free_clusters;
		DWORD total_number_of_clusters;
		BOOL result = GetDiskFreeSpace(NULL, &sectors_per_cluster, &bytes_per_sector, &number_of_free_clusters, &total_number_of_clusters);
		return bytes_per_sector;
	}

	unsigned int CeilToSectorSize(unsigned int size)
	{
		unsigned int sector_size = GetSectorSize();
		unsigned int rtn_size = size / sector_size;
		if (size % sector_size != 0)
		{
			rtn_size += 1;
		}
		rtn_size *= sector_size;
		return rtn_size;
	}

	unsigned int FloorToSectorSize(unsigned int size)
	{
		unsigned int sector_size = GetSectorSize();
		unsigned int rtn_size = size / sector_size;
		rtn_size *= sector_size;
		return rtn_size;
	}

	bool StartFileRead(void * dest, unsigned int max_size)
	{
		BOOL result = ReadFile(file_handle, dest, (DWORD) max_size, 0, &overlapped);
		DWORD error = GetLastError();
		bool async_started = !result && error == ERROR_IO_PENDING;
		if (!async_started)
		{
			unsigned char buffer [1000];
			DWORD hresult = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, (LPSTR) buffer, 1000, 0);
			std::cout << buffer << std::endl;
		}
		return (!result && error == ERROR_IO_PENDING);
	}

	bool CheckProgress(bool wait)
	{
		return HasOverlappedIoCompleted(&overlapped);
	}

	unsigned int GetBytesRead()
	{
		GetOverlappedResult( file_handle,
                               &overlapped,
                               (LPDWORD) &bytes_read,
                               FALSE);
		return bytes_read;
	}

	void Destroy()
	{
		CloseHandle(overlapped.hEvent);
		CloseHandle(file_handle);
	}

	bool Cancel()
	{
		BOOL result = CancelIoEx(file_handle, 0);
		if (!result)
		{
			DWORD error = GetLastError();
			unsigned char buffer [1000];
			DWORD hresult = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, error, 0, (LPSTR) buffer, 1000, 0);
			std::cout << buffer << std::endl;
		}
		return result != 0;
	}
	void AdjustOffsetAndSizes(unsigned int & file_offset, char *& buffer_start, unsigned int & buffer_size)
	{
		unsigned int adjusted_offset = FloorToSectorSize(file_offset);
		unsigned int offset_diff = file_offset - adjusted_offset;
		unsigned int adjusted_size = CeilToSectorSize(buffer_size + offset_diff);
		char * adjusted_buffer_start = buffer_start + offset_diff;

		file_offset = adjusted_offset;
		buffer_start = adjusted_buffer_start;
		buffer_size = adjusted_size;
	}
};

#else

#include <sys/types.h>
#include <aio.h>
#include <fcntl.h>
#include <errno.h>


class AsynchronousFile
{
	int file;
	aiocb control_block;
	unsigned int offset;
public:
	bool InitFile(const char * filename, unsigned int file_start_offset)
	{
		file = open(filename, O_RDONLY, 0);
		if (file == -1)				
        {				
			cout << "Unable to open file!" << endl;				
			return false;			
        }
		offset = file_start_offset;
		return true;
	};

	bool StartFileRead(void * dest, unsigned int max_size)
	{
		memset(&cb, 0, sizeof(aiocb));			
        control_block.aio_nbytes = SIZE_TO_READ;			
        control_block.aio_fildes = file;			
        control_block.aio_offset = offset;			
        control_block.aio_buf = dest;

		if (aio_read(&cb) == -1)
		{
			cout << "Unable to create request!" << endl;
			return false;
		}
		return true;
	}

	bool CheckProgress(bool wait)
	{
		return aio_error(&control_block) != EINPROGRESS;
	}

	void Destroy()
	{
		close(file);
	}

	unsigned int GetBytesRead()
	{
		return aio_return(&control_block);
	}

	bool Cancel()
	{
		return false;
	}
};


#endif

#endif