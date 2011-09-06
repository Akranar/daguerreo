CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(IO_ZipArchiver)

FILE(GLOB ZipArchiver_SRCS *.cpp)
FILE(GLOB ZipArchiver_H *.h)

IF(WIN32)
    SET(ZipArchiver_SRCS ${ZipArchiver_SRCS} ${ZipArchiver_H})
ENDIF(WIN32)

ADD_LIBRARY(IO_ZipArchiver STATIC ${ZipArchiver_SRCS})

