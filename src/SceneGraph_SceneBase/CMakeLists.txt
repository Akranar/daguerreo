CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(SceneGraph_SceneBase)

FILE(GLOB SceneBase_SRCS *.cpp)
FILE(GLOB SceneBase_H *.h)

IF(WIN32)
    SET(SceneBase_SRCS ${SceneBase_SRCS} ${SceneBase_H})
ENDIF(WIN32)

ADD_LIBRARY(SceneGraph_SceneBase STATIC ${SceneBase_SRCS})

