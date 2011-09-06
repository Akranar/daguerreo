CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Geometry_ProceduralMesh)

FILE(GLOB ProceduralMesh_SRCS *.cpp)
FILE(GLOB ProceduralMesh_H *.h)

IF(WIN32)
    SET(ProceduralMesh_SRCS ${ProceduralMesh_SRCS} ${ProceduralMesh_H})
ENDIF(WIN32)

ADD_LIBRARY(Geometry_ProceduralMesh STATIC ${ProceduralMesh_SRCS})

