CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

Project(Assets_Converter)


INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/../Assets_LibPNG)

FILE(GLOB Converter_SRCS *.cpp)
FILE(GLOB Converter_H *.h)

IF(WIN32)
    SET(Converter_SRCS ${Converter_SRCS} ${Converter_H})
ENDIF(WIN32)

#ADD_EXECUTABLE(Assets_Converter ${Converter_SRCS})

#SET(LEVEL_1 Graphics_GLEW IO_TextureLoader IO_SOIL Scripting_Lua Algorithms_Containers Dev_Asserts Core_FloatDataAccessor Core_FileSystem Core_Threading Core_Exceptions)
#SET(LEVEL_2 Graphics_SceneGraph Graphics_Renderer Graphics_RenderStates Graphics_Visual Graphics_Camera Geometry_Spatial Graphics_ShaderEffects Graphics_RenderResources Geometry_BoundCreation Geometry_BoundingVolume IO_BinaryFileStream IO_Serialization Geometry_QuickHull Geometry_Topology Core_ObjectCreation Algorithms_NTree)
#SET(LEVEL_3 IO_ColladaResource IO_XMLParser Core_URI IO_ColladaForm IO_ShaderParser)
#SET(LEVEL_4 Scripting_LuaBinds IO_ResourceManager IO_AssetsAggregation)
#SET(MEMORY Dev_Diagnostics Core_MemoryManagement)

#SET(EXTRA_LIBS ${MEMORY} ${LEVEL_4} ${LEVEL_3} ${LEVEL_2} ${LEVEL_1})


#TARGET_LINK_LIBRARIES(Assets_Converter Assets_Assimp IO_LibPNG IO_ZLib ${EXTRA_LIBS})


ADD_LIBRARY(Assets_Converter STATIC ${Converter_SRCS})


