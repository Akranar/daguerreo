CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Geometry_GeometricTests)

FILE(GLOB GeometricTests_SRCS *.cpp)
FILE(GLOB GeometricTests_H *.h)

IF(WIN32)
    SET(GeometricTests_SRCS ${GeometricTests_SRCS} ${GeometricTests_H})
ENDIF(WIN32)

ADD_LIBRARY( Geometry_GeometricTests STATIC ${GeometricTests_SRCS})

