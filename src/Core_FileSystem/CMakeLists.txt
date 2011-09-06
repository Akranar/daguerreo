CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Core_FileSystem)

FILE(GLOB FileSystem_SRCS FileSystem.cpp)
FILE(GLOB FileSystem_H *.h)


IF(WIN32)
    SET(FileSystem_SRCS ${FileSystem_SRCS} FileSystem_WINDOWS.cpp)
ELSE(WIN32)
    IF(APPLE)
        SET(FileSystem_SRCS ${FileSystem_SRCS} FileSystem_MACOSX.cpp FileSystem_MACOSX_Cocoa.m)
    ELSE(APPLE)
        SET(FileSystem_SRCS ${FileSystem_SRCS} FileSystem_LINUX.cpp)
    ENDIF(APPLE)
ENDIF(WIN32)

IF(WIN32)
    SET(FileSystem_SRCS ${FileSystem_SRCS} ${FileSystem_H})
ENDIF(WIN32)

ADD_LIBRARY( Core_FileSystem STATIC ${FileSystem_SRCS})

