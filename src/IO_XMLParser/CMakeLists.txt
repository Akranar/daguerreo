CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(IO_XMLParser)

FILE(GLOB XMLParser_SRCS *.cpp)
FILE(GLOB XMLParser_H *.h)

IF(WIN32)
    SET(XMLParser_SRCS ${XMLParser_SRCS} ${XMLParser_H})
ENDIF(WIN32)

ADD_LIBRARY( IO_XMLParser STATIC ${XMLParser_SRCS})

