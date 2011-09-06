CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Core_Include)

FILE(GLOB Include_SRCS *.cpp)
FILE(GLOB Include_H *.h)

IF(WIN32)
    SET(Include_SRCS ${Include_SRCS} ${Include_H})
ENDIF(WIN32)

ADD_LIBRARY(Core_Include STATIC ${Include_SRCS})

