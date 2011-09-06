
IF (WIN32)
	FIND_PATH(CG_INCLUDE_PATH cg.h
		PATHS
			"$ENV{PROGRAMFILES}/NVIDIA Corporation/Cg/include"
		DOC "The directory where cg.h resides")

	FIND_LIBRARY(CG_LIBRARY cg.lib
		PATHS
			"$ENV{PROGRAMFILES}/NVIDIA Corporation/Cg/lib/"
		DOC "The directory where cg.lib resides")

	FIND_LIBRARY(CGGL_LIBRARY cgGL.lib
		PATHS
			"$ENV{PROGRAMFILES}/NVIDIA Corporation/Cg/lib/"
		DOC "The directory where cgGL.lib resides")

	FIND_LIBRARY(CGD3D8_LIBRARY cgD3D8.lib
		PATHS
			"$ENV{PROGRAMFILES}/NVIDIA Corporation/Cg/lib/"
		DOC "The directory where cgD3D8.lib resides")

	FIND_LIBRARY(CGD3D9_LIBRARY cgD3D9.lib
		PATHS
			"$ENV{PROGRAMFILES}/NVIDIA Corporation/Cg/lib/"
		DOC "The directory where cgD3D9.lib resides")

	FIND_LIBRARY(CGD3D10_LIBRARY cgD3D10.lib
		PATHS
			"$ENV{PROGRAMFILES}/NVIDIA Corporation/Cg/lib/"
		DOC "The directory where cgD3D10.lib resides")

	FIND_LIBRARY(CGD3D11_LIBRARY cgD3D11.lib
		PATHS
			"$ENV{PROGRAMFILES}/NVIDIA Corporation/Cg/lib/"
		DOC "The directory where cgD3D11.lib resides")
ENDIF (WIN32)

IF (CG_INCLUDE_PATH)
	SET( CG_FOUND 1 CACHE STRING "Set to 1 if CG is found, 0 otherwise")
ELSE (CG_INCLUDE_PATH)
	SET( CG_FOUND 0 CACHE STRING "Set to 1 if CG is found, 0 otherwise")
ENDIF (CG_INCLUDE_PATH)


MARK_AS_ADVANCED( CG_FOUND )

