CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Registry_SceneRegistryBase)

FILE(GLOB SceneRegistryBase_SRCS *.cpp)
FILE(GLOB SceneRegistryBase_H *.h)

IF(WIN32)
    SET(SceneRegistryBase_SRCS ${SceneRegistryBase_SRCS} ${SceneRegistryBase_H})
ENDIF(WIN32)

ADD_LIBRARY(Registry_SceneRegistryBase STATIC ${SceneRegistryBase_SRCS})

