CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Graphics_RendererBase)

FILE(GLOB RendererBase_SRCS *.cpp)
FILE(GLOB RendererBase_H *.h)

IF(WIN32)
    SET(RendererBase_SRCS ${RendererBase_SRCS} ${RendererBase_H})
ENDIF(WIN32)

ADD_LIBRARY(Graphics_RendererBase STATIC ${RendererBase_SRCS})

ADD_SUBDIRECTORY(OpenGL2.1)
ADD_SUBDIRECTORY(OpenGL3.2)
ADD_SUBDIRECTORY(D3D9)
ADD_SUBDIRECTORY(D3D10)

IF (USE_DIRECTX10)
    SET(RendererBase_Implementations Graphics_RendererBase_D3D10)
ELSEIF(USE_DIRECTX9)
    SET(RendererBase_Implementations Graphics_RendererBase_D3D9)
ELSEIF(USE_OPENGL32)
    SET(RendererBase_Implementations Graphics_RendererBase_OpenGL_32)
ELSEIF(USE_OPENGL21)
    SET(RendererBase_Implementations Graphics_RendererBase_OpenGL_21)
ENDIF (USE_DIRECTX10)

TARGET_LINK_LIBRARIES(Graphics_RendererBase ${RendererBase_Implementations})
