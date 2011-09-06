CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Geometry_GeometricPrimitives)

FILE(GLOB GeometricPrimitives_SRCS *.cpp)
FILE(GLOB GeometricPrimitives_H *.h)

IF(WIN32)
    SET(GeometricPrimitives_SRCS ${GeometricPrimitives_SRCS} ${GeometricPrimitives_H})
ENDIF(WIN32)

ADD_LIBRARY( Geometry_GeometricPrimitives STATIC ${GeometricPrimitives_SRCS})

