CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

#Relevant Defines:
#   -> USE_DIRECTX10

Project(Core_WindowEvents_Common)

FILE(GLOB WindowEvents_Common_SRCS *.cpp)
FILE(GLOB WindowEvents_Common_H *.h)

IF(WIN32)
    SET(WindowEvents_Common_SRCS ${WindowEvents_Common_SRCS} ${WindowEvents_Common_H})
ENDIF(WIN32)

ADD_LIBRARY(Core_WindowEvents STATIC ${WindowEvents_Common_SRCS})

ADD_SUBDIRECTORY(Win32OpenGL)
ADD_SUBDIRECTORY(MacOSXCocoa)
ADD_SUBDIRECTORY(XLIB)
ADD_SUBDIRECTORY(Win32D3D10)
ADD_SUBDIRECTORY(Win32D3D9)

IF(WIN32)
    IF (USE_DIRECTX10)
        SET(WindowEvents_Implementation Core_WindowEvents_Win32D3D10)
    ELSEIF(USE_DIRECTX9)
        SET(WindowEvents_Implementation Core_WindowEvents_Win32D3D9)
    ELSEIF(USE_OPENGL) 
        SET(WindowEvents_Implementation Core_WindowEvents_Win32OpenGL)
    ENDIF(USE_DIRECTX10)
ELSEIF(APPLE)
    SET(WindowEvents_Implementation Core_WindowEvents_MacOSXCocoa)
ELSE(WIN32)
    SET(WindowEvents_Implementation Core_WindowEvents_XLIB)
ENDIF(WIN32)
TARGET_LINK_LIBRARIES(Core_WindowEvents ${WindowEvents_Implementation})


