CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Image_ProceduralTexture)

FILE(GLOB ProceduralTexture_SRCS *.cpp)
FILE(GLOB ProceduralTexture_H *.h)

IF(WIN32)
    SET(ProceduralTexture_SRCS ${ProceduralTexture_SRCS} ${ProceduralTexture_H})
ENDIF(WIN32)

ADD_LIBRARY(Image_ProceduralTexture STATIC ${ProceduralTexture_SRCS})

