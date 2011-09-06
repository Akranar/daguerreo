CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Example_RendererBaseUsage)

FILE(GLOB RendererBaseUsage_SRCS *.cpp)
FILE(GLOB RendererBaseUsage_H *.h)

SET(RendererBaseUsage_SRCS ${RendererBaseUsage_SRCS} ${RendererBaseUsage_H})
#SET_SOURCE_FILES_PROPERTIES(${RendererBaseUsage_SRCS} PROPERTIES HEADER_FILE_ONLY TRUE)

ADD_LIBRARY(Example_RendererBaseUsage STATIC ${RendererBaseUsage_SRCS})
SET_TARGET_PROPERTIES(Example_RendererBaseUsage PROPERTIES LINKER_LANGUAGE CXX)
