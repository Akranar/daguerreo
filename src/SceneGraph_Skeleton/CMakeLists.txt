CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(SceneGraph_Skeleton)

FILE(GLOB Skeleton_SRCS *.cpp)
FILE(GLOB Skeleton_H *.h)

IF(WIN32)
    SET(Skeleton_SRCS ${Skeleton_SRCS} ${Skeleton_H})
ENDIF(WIN32)

ADD_LIBRARY(SceneGraph_Skeleton STATIC ${Skeleton_SRCS})

