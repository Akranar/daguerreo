#ifndef _DISPLAY_MODE_MANAGER_PIMPL_LINUX_H_
#define _DISPLAY_MODE_MANAGER_PIMPL_LINUX_H_

#include <vector>
//#include <X11/Xlib.h>
//#include <X11/extensions/Xrandr.h>

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>


class DisplayModeManager;

class DisplayModeManagerPimpl
{
    static int manager_instances;
public:
	unsigned int mode_count;
	 XF86VidModeModeInfo * modes;

	DisplayModeManagerPimpl();
	~DisplayModeManagerPimpl();

	static bool NoManagersAlive();
};

#endif
