CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Core_Serialization)

FILE(GLOB Serialization_SRCS *.cpp)
FILE(GLOB Serialization_H *.h)

IF(WIN32)
    SET(Serialization_SRCS ${Serialization_SRCS} ${Serialization_H})
ENDIF(WIN32)

ADD_LIBRARY(Core_Serialization STATIC ${Serialization_SRCS})

