#ifndef _DISPLAY_MODE_MANAGER_PIMPL_WINDOWS_H_
#define _DISPLAY_MODE_MANAGER_PIMPL_WINDOWS_H_

#include <vector>
#include <windows.h>

class DisplayModeManager;

class DisplayModeManagerPimpl
{
public:
	struct DisplayMode
	{
		int width;
		int height;
	};
	unsigned int mode_count;
	DisplayMode * modes;
	DisplayModeManagerPimpl();
	~DisplayModeManagerPimpl();
};

#endif