CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Registry_GraphicsRegistryFX)

FILE(GLOB GraphicsRegistryFX_SRCS *.cpp)
FILE(GLOB GraphicsRegistryFX_H *.h)

IF(WIN32)
    SET(GraphicsRegistryFX_SRCS ${GraphicsRegistryFX_SRCS} ${GraphicsRegistryFX_H})
ENDIF(WIN32)

ADD_LIBRARY(Registry_GraphicsRegistryFX STATIC ${GraphicsRegistryFX_SRCS})

