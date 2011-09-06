#include "FileSystem.h"

extern "C"
{
    #include "FileSystem_MACOSX_Cocoa.h"
}

bool FileSystem::SetWorkingDirectoryResources()
{
    return Cocoa_SetWorkingDirectoryResources();
}
