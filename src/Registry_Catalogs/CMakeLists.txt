CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Registry_Catalogs)

FILE(GLOB Catalogs_SRCS *.cpp)
FILE(GLOB Catalogs_H *.h)

IF(WIN32)
    SET(Catalogs_SRCS ${Catalogs_SRCS} ${Catalogs_H})
ENDIF(WIN32)

ADD_LIBRARY(Registry_Catalogs STATIC ${Catalogs_SRCS})

