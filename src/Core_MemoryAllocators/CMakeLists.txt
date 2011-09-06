CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Core_MemoryAllocators)

FILE(GLOB MemoryAllocators_SRCS *.cpp)
FILE(GLOB MemoryAllocators_H *.h)

IF(WIN32)
    SET(MemoryAllocators_SRCS ${MemoryAllocators_SRCS} ${MemoryAllocators_H})
ENDIF(WIN32)

ADD_LIBRARY( Core_MemoryAllocators STATIC ${MemoryAllocators_SRCS})

