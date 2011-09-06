CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Algorithms_NTree)

FILE(GLOB NTree_SRCS *.cpp)
FILE(GLOB NTree_H *.h)

IF(WIN32)
    SET(NTree_SRCS ${NTree_SRCS} ${NTree_H})
ENDIF(WIN32)

ADD_LIBRARY( Algorithms_NTree STATIC ${NTree_SRCS})

