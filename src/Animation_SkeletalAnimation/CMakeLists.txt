CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Animation_SkeletalAnimation)

FILE(GLOB SkeletalAnimation_SRCS *.cpp)
FILE(GLOB SkeletalAnimation_H *.h)

IF(WIN32)
    SET(SkeletalAnimation_SRCS ${SkeletalAnimation_SRCS} ${SkeletalAnimation_H})
ENDIF(WIN32)

ADD_LIBRARY(Animation_SkeletalAnimation STATIC ${SkeletalAnimation_SRCS})

