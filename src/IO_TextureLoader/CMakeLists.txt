CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(IO_TextureLoader)

FILE(GLOB TextureLoader_SRCS *.cpp)
FILE(GLOB TextureLoader_H *.h)

IF(WIN32)
    SET(TextureLoader_SRCS ${TextureLoader_SRCS} ${TextureLoader_H})
ENDIF(WIN32)

ADD_LIBRARY(IO_TextureLoader STATIC ${TextureLoader_SRCS})

