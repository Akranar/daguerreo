CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Graphics_RendererFX)

FILE(GLOB RendererFX_SRCS *.cpp)
FILE(GLOB RendererFX_H *.h)

IF(WIN32)
    SET(RendererFX_SRCS ${RendererFX_SRCS} ${RendererFX_H})
ENDIF(WIN32)

ADD_LIBRARY(Graphics_RendererFX STATIC ${RendererFX_SRCS})

