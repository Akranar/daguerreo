CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Core_FloatDataAccessor)

FILE(GLOB FloatDataAccessor_SRCS *.cpp)
FILE(GLOB FloatDataAccessor_H *.h)

IF(WIN32)
    SET(FloatDataAccessor_SRCS ${FloatDataAccessor_SRCS} ${FloatDataAccessor_H})
ENDIF(WIN32)

ADD_LIBRARY( Core_FloatDataAccessor STATIC ${FloatDataAccessor_SRCS})

