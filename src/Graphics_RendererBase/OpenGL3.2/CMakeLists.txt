CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Graphics_RendererBase_OpenGL_32)

FILE(GLOB RendererBase_OpenGL_32_SRCS *.cpp)
FILE(GLOB RendererBase_OpenGL_32_H *.h)

SET(RendererBase_OpenGL_32_SRCS ${RendererBase_OpenGL_32_SRCS} ${RendererBase_OpenGL_32_H})

IF (NOT USE_OPENGL32)
    SET_SOURCE_FILES_PROPERTIES(${RendererBase_OpenGL_32_SRCS} PROPERTIES HEADER_FILE_ONLY TRUE)
ENDIF (NOT USE_OPENGL32)    

ADD_LIBRARY(Graphics_RendererBase_OpenGL_32 STATIC ${RendererBase_OpenGL_32_SRCS})

