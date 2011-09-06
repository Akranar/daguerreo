#ifndef _DISPLAY_MODE_MANAGER_PIMPL_MACOSX_H_
#define _DISPLAY_MODE_MANAGER_PIMPL_MACOSX_H_

#include <vector>
#include <Cocoa/Cocoa.h>

class DisplayModeManager;

class DisplayModeManagerPimpl
{
public:
	unsigned int mode_count;
	CGDisplayModeRef * modes;
	DisplayModeManagerPimpl();
	~DisplayModeManagerPimpl();
	int GetBitsPerPixel(CGDisplayModeRef & mode);
};

#endif