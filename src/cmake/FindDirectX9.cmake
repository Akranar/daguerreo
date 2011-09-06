
IF (WIN32)
	FIND_PATH(DX9_INCLUDE_PATH d3d9.h
		PATHS
			"$ENV{DXSDK_DIR}/Include"
			"$ENV{PROGRAMFILES}/Microsoft DirectX SDK/Include"
		DOC "The directory where D3D9.h resides")

	FIND_LIBRARY(D3D9_LIBRARY d3d9.lib
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{PROGRAMFILES}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3d9.lib resides")

	FIND_LIBRARY(D3DX9_LIBRARY d3dx9.lib
		PATHS
			"$ENV{DXSDK_DIR}/Lib/x86"
			"$ENV{PROGRAMFILES}/Microsoft DirectX SDK/Lib/x86"
		DOC "The directory where d3dx9.lib resides")
    
	SET(DX9_LIBRARIES ${D3D9_LIBRARY} ${D3DX9_LIBRARY})
    

ENDIF (WIN32)

IF (DX9_INCLUDE_PATH)
	SET( DX9_FOUND 1 CACHE STRING "Set to 1 if DX9 is found, 0 otherwise")
ELSE (DX9_INCLUDE_PATH)
	SET( DX9_FOUND 0 CACHE STRING "Set to 1 if DX9 is found, 0 otherwise")
ENDIF (DX9_INCLUDE_PATH)

MARK_AS_ADVANCED( DX9_FOUND )

