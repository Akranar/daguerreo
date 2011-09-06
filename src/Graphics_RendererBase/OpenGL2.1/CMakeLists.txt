CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Graphics_RendererBase_OpenGL_21)

FILE(GLOB RendererBase_OpenGL_21_SRCS *.cpp)
FILE(GLOB RendererBase_OpenGL_21_H *.h)

SET(RendererBase_OpenGL_21_SRCS ${RendererBase_OpenGL_21_SRCS} ${RendererBase_OpenGL_21_H})

IF (NOT USE_OPENGL21)
    SET_SOURCE_FILES_PROPERTIES(${RendererBase_OpenGL_21_SRCS} PROPERTIES HEADER_FILE_ONLY TRUE)
ENDIF (NOT USE_OPENGL21)    

ADD_LIBRARY(Graphics_RendererBase_OpenGL_21 STATIC ${RendererBase_OpenGL_21_SRCS})

