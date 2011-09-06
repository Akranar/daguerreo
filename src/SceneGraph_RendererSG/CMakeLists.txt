CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(SceneGraph_RendererSG)

FILE(GLOB RendererSG_SRCS *.cpp)
FILE(GLOB RendererSG_H *.h)

IF(WIN32)
    SET(RendererSG_SRCS ${RendererSG_SRCS} ${RendererSG_H})
ENDIF(WIN32)

ADD_LIBRARY(SceneGraph_RendererSG STATIC ${RendererSG_SRCS})

