CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Registry_GraphicsRegistryBase)

FILE(GLOB GraphicsRegistryBase_SRCS *.cpp)
FILE(GLOB GraphicsRegistryBase_H *.h)

IF(WIN32)
    SET(GraphicsRegistryBase_SRCS ${GraphicsRegistryBase_SRCS} ${GraphicsRegistryBase_H})
ENDIF(WIN32)

ADD_LIBRARY(Registry_GraphicsRegistryBase STATIC ${GraphicsRegistryBase_SRCS})

