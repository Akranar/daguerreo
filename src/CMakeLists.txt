cmake_minimum_required(VERSION 2.8)


PROJECT( Main )
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)

#********************************************************
#   Contents
#       -> Options Configurations
#       -> Include Directories
#       -> Libraries
#********************************************************

#********************************************************
#   Options Configurations
#********************************************************
OPTION(USE_DIRECTX10 "Use DirectX10" OFF)
OPTION(USE_DIRECTX9 "Use DirectX9" OFF)
OPTION(USE_OPENGL32 "Use OpenGL 3.2" OFF)
OPTION(USE_OPENGL21 "Use OpenGL 2.1" OFF)
OPTION(USE_CG "Use Cg" OFF)

OPTION(PATH_VERBOSE OFF)

#Set Location of Modules for Finding Packages.
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

#Find DirectX10
#   -> See ${CMAKE_SOURCE_DIR}/cmake/FindDirectX10.cmake for details.
IF (USE_DIRECTX10)
    FIND_PACKAGE(DirectX10)
    IF (DX10_FOUND)
        IF (PATH_VERBOSE)
            MESSAGE(STATUS "DirectX10 Found: ")
            MESSAGE(STATUS "    DX10_INCLUDE_PATH: " ${DX10_INCLUDE_PATH})
            MESSAGE(STATUS "    D3D10_LIBRARY: " ${D3D10_LIBRARY})
            MESSAGE(STATUS "    D3DX10_LIBRARY: " ${D3DX10_LIBRARY})
        ENDIF (PATH_VERBOSE)
        INCLUDE_DIRECTORIES(${DX10_INCLUDE_PATH})
        SET(EXTRA_LIBS ${EXTRA_LIBS} ${DX10_LIBRARIES})
    ELSE (DX10_FOUND)
        SET(USE_DIRECTX10 OFF)
    ENDIF (DX10_FOUND)
ENDIF (USE_DIRECTX10)

#Find DirectX9
#   -> See ${CMAKE_SOURCE_DIR}/cmake/FindDirectX9.cmake for details.
IF (USE_DIRECTX9)
    FIND_PACKAGE(DirectX9)
    IF (DX9_FOUND)
        IF (PATH_VERBOSE)
            MESSAGE(STATUS "DirectX9 Found: ")
            MESSAGE(STATUS "    DX9_INCLUDE_PATH: " ${DX9_INCLUDE_PATH})
            MESSAGE(STATUS "    D3D9_LIBRARY: " ${D3D9_LIBRARY})
            MESSAGE(STATUS "    D3DX9_LIBRARY: " ${D3DX9_LIBRARY})
        ENDIF (PATH_VERBOSE)
        INCLUDE_DIRECTORIES(${DX9_INCLUDE_PATH})
        SET(EXTRA_LIBS ${EXTRA_LIBS} ${DX9_LIBRARIES})
    ELSE (DX9_FOUND)
        SET(USE_DIRECTX9 OFF)
    ENDIF (DX9_FOUND)
ENDIF (USE_DIRECTX9)

#Find Cg
#   -> See ${CMAKE_SOURCE_DIR}/cmake/FindCg.cmake for details.
IF (USE_CG)
    FIND_PACKAGE(Cg)
    IF (CG_FOUND)
        IF (PATH_VERBOSE)
            MESSAGE(STATUS "Cg Found: ")
            MESSAGE(STATUS "    CG_INCLUDE_PATH: " ${CG_INCLUDE_PATH})
            MESSAGE(STATUS "    CG_LIBRARY: " ${CG_LIBRARY})
        ENDIF (PATH_VERBOSE)
        INCLUDE_DIRECTORIES(${CG_INCLUDE_PATH})
        IF (USE_DIRECTX10)
			SET(CG_D3D10_LIBRARIES ${CG_LIBRARY} ${CGD3D10_LIBRARY})
			SET(EXTRA_LIBS ${EXTRA_LIBS} ${CG_D3D10_LIBRARIES})
		ELSEIF (USE_DIRECTX9)
            SET(CG_D3D9 LIBRARIES ${CG_LIBRARY} ${CGD3D9_LIBRARY})
			SET(EXTRA_LIBS ${EXTRA_LIBS} ${CG_D3D9_LIBRARIES})
        ELSEIF (USE_OPENGL)
			SET(CG_OPENGL_LIBRARIES ${CG_LIBRARY} ${CGGL_LIBRARY})
			SET(EXTRA_LIBS ${EXTRA_LIBS} ${CG_OPENGL_LIBRARIES})
		ENDIF (USE_DIRECTX10)
    ELSE (CG_FOUND)
        SET(USE_CG OFF)
    ENDIF (CG_FOUND)
ENDIF(USE_CG)


IF(USE_DIRECTX10)
    SET (USE_DIRECTX9 OFF)
    SET (USE_OPENGL32 OFF)
    SET (USE_OPENGL21 OFF)
    SET (USE_OPENGL OFF)
ELSEIF (USE_DIRECTX9)
    SET (USE_OPENGL32 OFF)
    SET (USE_OPENGL21 OFF)
    SET (USE_OPENGL OFF)
ELSEIF (USE_OPENGL32)
    SET (USE_OPENGL21 OFF)
    SET (USE_OPENGL ON)
ELSEIF (USE_OPENGL21)
    SET (USE_OPENGL ON)
ENDIF(USE_DIRECTX10)


IF (USE_DIRECTX10)
	ADD_DEFINITIONS(-DUSE_DIRECTX10)
	MESSAGE(STATUS "DEFINE: USE_DIRECTX10")
ELSEIF (USE_DIRECTX9)
    ADD_DEFINITIONS(-USE_DIRECTX9)
    MESSAGE(STATUS "DEFINE: USE_DIRECTX9")
ELSEIF (USE_OPENGL32)
	ADD_DEFINITIONS(-DUSE_OPENGL)
    ADD_DEFINITIONS(-DUSE_OPENGL32)
	MESSAGE(STATUS "DEFINE: USE_OPENGL")
    MESSAGE(STATUS "DEFINE: USE_OPENGL32")
ELSEIF (USE_OPENGL21)
	ADD_DEFINITIONS(-DUSE_OPENGL)
    ADD_DEFINITIONS(-DUSE_OPENGL21)
	MESSAGE(STATUS "DEFINE: USE_OPENGL")
    MESSAGE(STATUS "DEFINE: USE_OPENGL21")
ENDIF(USE_DIRECTX10)

IF (USE_CG)
    ADD_DEFINITIONS(-DUSE_CG)
    MESSAGE(STATUS "DEFINE: USE_CG")
ENDIF (USE_CG)

#********************************************************
#   Include Paths
#********************************************************

INCLUDE_DIRECTORIES(Core_Include)
#This command needed for Eigen
#   -> Due to the include on line 29 of StdVector.h
INCLUDE_DIRECTORIES(Geometry_Eigen)


#SSE2
IF(MSVC)
    ADD_DEFINITIONS(/arch:SSE2)
ELSE (MSVC)
    SET(CMAKE_C_FLAGS -m32)
    SET(CMAKE_CXX_FLAGS -m32)
ENDIF(MSVC)

#**********************************
#   Example
#**********************************

ADD_SUBDIRECTORY(Example_RendererBaseUsage)
ADD_SUBDIRECTORY(Example_RendererSGUsage)

SET(Example_Library
    Example_RendererBaseUsage)

#**********************************
#   Animation
#**********************************

ADD_SUBDIRECTORY(Animation_SkeletalAnimation)
SET(Animation_Library Animation_SkeletalAnimation)

#**********************************
#   Algorithms
#**********************************
ADD_SUBDIRECTORY(Algorithms_NTree)
SET(Algorithms_Library Algorithms_NTree)

#**********************************
#   Assets
#**********************************
ADD_SUBDIRECTORY(Assets_Assimp)
ADD_SUBDIRECTORY(Assets_Converter)

SET(Assets_Library Assets_Assimp Assets_Converter)

#**********************************
#   Image
#**********************************

ADD_SUBDIRECTORY(Image_ProceduralTexture)
ADD_SUBDIRECTORY(Image_LibPNG154)

SET(Image_Library ${Image_Library}
    Image_ProceduralTexture
    Image_LibPNG154)


#**********************************
#   Core
#**********************************
ADD_SUBDIRECTORY(Core_Include)
ADD_SUBDIRECTORY(Core_WindowEvents)
ADD_SUBDIRECTORY(Core_FileSystem)
ADD_SUBDIRECTORY(Core_Serialization)
ADD_SUBDIRECTORY(Core_MemoryAllocators)
ADD_SUBDIRECTORY(Core_FloatDataAccessor)

SET(Core_Library
    Core_Include
    Core_WindowEvents
    Core_FileSystem
    Core_Serialization
    Core_MemoryAllocators
    Core_FloatDataAccessor)

#**********************************
#   Geometry
#**********************************

ADD_SUBDIRECTORY(Geometry_GeometricTests)
ADD_SUBDIRECTORY(Geometry_GeometricPrimitives)
ADD_SUBDIRECTORY(Geometry_BoundCreation)
ADD_SUBDIRECTORY(Geometry_BoundingVolume)
ADD_SUBDIRECTORY(Geometry_ProceduralMesh)
ADD_SUBDIRECTORY(Geometry_QuickHull)
ADD_SUBDIRECTORY(Geometry_Eigen)
ADD_SUBDIRECTORY(Geometry_Topology)
    
SET(Geometry_Library ${Geometry_Library}
    Geometry_BoundCreation
    Geometry_BoundingVolume
    Geometry_ProceduralMesh
    Geometry_QuickHull
    Geometry_Topology
    )


#**********************************
#   Graphics
#**********************************

IF (USE_DIRECTX10)

#ADD_SUBDIRECTORY(Graphics_API_DirectX10)
#SET(Graphics_Library ${Graphics_Library}
#    Graphics_API_DirectX10)

ENDIF (USE_DIRECTX10)
    
ADD_SUBDIRECTORY(Graphics_API_OpenGL)
SET(Graphics_Library ${Graphics_Library}
    Graphics_API_OpenGL)
    

ADD_SUBDIRECTORY(Graphics_Projector)
ADD_SUBDIRECTORY(Graphics_RendererBase)
ADD_SUBDIRECTORY(Graphics_RendererFX)
SET(Graphics_Library ${Graphics_Library}
    Graphics_Projector
    Graphics_RendererBase
    Graphics_RendererFX)


#**********************************
#   IO
#**********************************
ADD_SUBDIRECTORY(IO_SOIL)
ADD_SUBDIRECTORY(IO_ZLIB)
ADD_SUBDIRECTORY(IO_ZipArchiver)
ADD_SUBDIRECTORY(IO_BinaryFileStream)
ADD_SUBDIRECTORY(IO_TextureLoader)
ADD_SUBDIRECTORY(IO_XMLParser)


SET(IO_Library
    IO_SOIL
    IO_ZLIB
    IO_ZipArchiver
    IO_BinaryFileStream
    IO_TextureLoader
    IO_XMLParser)

#**********************************
#   Registry
#**********************************
ADD_SUBDIRECTORY(Registry_Catalogs)
ADD_SUBDIRECTORY(Registry_GraphicsRegistryBase)
ADD_SUBDIRECTORY(Registry_GraphicsRegistryFX)
ADD_SUBDIRECTORY(Registry_SceneRegistryBase)

SET(Registry_Library
    Registry_Catalogs
    Registry_GraphicsRegistryBase
    Registry_GraphicsRegistryFX
    Registry_SceneRegistryBase)

#**********************************
#   SceneGraph
#**********************************
ADD_SUBDIRECTORY(SceneGraph_RendererSG)
ADD_SUBDIRECTORY(SceneGraph_SceneBase)
ADD_SUBDIRECTORY(SceneGraph_Skeleton)

SET(SceneGraph_Library
    SceneGraph_RendererSG
    SceneGraph_SceneBase
    SceneGraph_Skeleton)

#**********************************
#   DYNE_LIBRARIES
#       Core, IO  
#       Graphics, Geometry
#       SceneGraph
#       Animation
#       Registry
#       Application
#
#**********************************

SET(DYNE_LIBRARIES
    ${Example_Library}
    ${Assets_Library}
    ${Animation_Library} 
    ${Image_Library}
    ${SceneGraph_Library}
    ${Registry_Library}
    ${Geometry_Library}
    ${Graphics_Library}
    ${IO_Library}
    ${Algorithms_Library}
    ${Core_Library}
    )

IF(APPLE)
    FILE(GLOB_RECURSE RESOURCE_FILES Resources/*.*)
    SET(GUI_TYPE MACOSX_BUNDLE ${RESOURCE_FILES} )
ENDIF(APPLE)


FILE(GLOB Main_SRCS *.cpp)
FILE(GLOB Main_H *.h)
SET(Main_SRCS ${Main_SRCS} ${Main_H})
ADD_EXECUTABLE( Application ${GUI_TYPE} ${Main_SRCS} )

#IF(APPLE)
#    SET_SOURCE_FILES_PROPERTIES(${RESOURCE_FILES} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
#ENDIF(APPLE)

#MESSAGE(STATUS "${EXTRA_LIBS}")


TARGET_LINK_LIBRARIES(Application ${DYNE_LIBRARIES} ${EXTRA_LIBS})

IF(NOT APPLE)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Resources)
ELSE(NOT APPLE)
    execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/Resources ${CMAKE_BINARY_DIR}/Application.app/Contents/Resources)
ENDIF(NOT APPLE)


