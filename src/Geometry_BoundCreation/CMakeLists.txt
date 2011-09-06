CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Geometry_BoundCreation)

FILE(GLOB BoundCreation_SRCS *.cpp)
FILE(GLOB BoundCreation_H *.h)

IF(WIN32)
    SET(BoundCreation_SRCS ${BoundCreation_SRCS} ${BoundCreation_H})
ENDIF(WIN32)

ADD_LIBRARY( Geometry_BoundCreation STATIC ${BoundCreation_SRCS})

