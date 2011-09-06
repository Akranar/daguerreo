CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Geometry_QuickHull)

FILE(GLOB QuickHull_SRCS *.cpp)
FILE(GLOB QuickHull_H *.h)

IF(WIN32)
    SET(QuickHull_SRCS ${QuickHull_SRCS} ${QuickHull_H})
ENDIF(WIN32)

ADD_LIBRARY( Geometry_QuickHull STATIC ${QuickHull_SRCS})

