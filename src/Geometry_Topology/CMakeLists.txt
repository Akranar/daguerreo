CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Geometry_Topology)

FILE(GLOB Topology_SRCS *.cpp)
FILE(GLOB Topology_H *.h)

IF(WIN32)
    SET(Topology_SRCS ${Topology_SRCS} ${Topology_H})
ENDIF(WIN32)

ADD_LIBRARY( Geometry_Topology STATIC ${Topology_SRCS})

