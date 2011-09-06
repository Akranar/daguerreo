#include "FileSystem_MACOSX_Cocoa.h"

#import <CoreFoundation/CoreFoundation.h>


int Cocoa_SetWorkingDirectoryResources(void)
{
    CFBundleRef mainBundle = CFBundleGetMainBundle();
    CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
    char path[PATH_MAX];
    if (!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)path, PATH_MAX))
    {
        // error!
        return 0;
    }
    CFRelease(resourcesURL);

    chdir(path);
    return 1;
}
