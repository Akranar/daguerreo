CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Geometry_BoundingVolume)

FILE(GLOB BoundingVolume_SRCS *.cpp)
FILE(GLOB BoundingVolume_H *.h)

IF(WIN32)
    SET(BoundingVolume_SRCS ${BoundingVolume_SRCS} ${BoundingVolume_H})
ENDIF(WIN32)

ADD_LIBRARY( Geometry_BoundingVolume STATIC ${BoundingVolume_SRCS})

