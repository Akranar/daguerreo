CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(IO_BinaryFileStream)

FILE(GLOB BinaryFileStream_SRCS *.cpp)
FILE(GLOB BinaryFileStream_H *.h)

IF(WIN32)
    SET(BinaryFileStream_SRCS ${BinaryFileStream_SRCS} ${BinaryFileStream_H})
ENDIF(WIN32)

ADD_LIBRARY( IO_BinaryFileStream STATIC ${BinaryFileStream_SRCS})

