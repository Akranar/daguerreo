#include "DisplayModeManagerPimpl_LINUX.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#include "WindowData_LINUX.h"

int DisplayModeManagerPimpl::manager_instances = 0;

class XF86VidModeModeInfoCompare
{
public:
    bool operator() (const XF86VidModeModeInfo & a, const XF86VidModeModeInfo & b)
    {
        if (a.hdisplay < b.hdisplay)
        {
            return true;
        }
        if (a.hdisplay > b.hdisplay)
        {
            return false;
        }
        if (a.vdisplay < b.vdisplay)
        {
            return true;
        }
        return false;
    }

};


DisplayModeManagerPimpl::DisplayModeManagerPimpl()
{
    WindowData::OpenDisplay();
    manager_instances += 1;

    XF86VidModeModeInfo normalscreen_mode;
	XF86VidModeModeInfo ** display_modes;
	int display_mode_count;

    /*
	if (!display)
	{
	    std::cout << "Error in initialization of DiplayModeManager: A WindowEvents object must be in existence during initialization." << std::endl;
	    throw 1;
	}
	*/

    XF86VidModeGetModeLine(WindowData::GetDisplay(), 0, (int*)&normalscreen_mode.dotclock, (XF86VidModeModeLine*)&normalscreen_mode.hdisplay);
	XF86VidModeGetAllModeLines(WindowData::GetDisplay(), 0, &display_mode_count, &display_modes);

    std::vector<XF86VidModeModeInfo> sort_modes;
    sort_modes.push_back(normalscreen_mode);
    for (unsigned int i = 0; i < display_mode_count; ++i)
    {
        //if (display_modes[i]->htotal == display_modes[i]->hdisplay && display_modes[i]->vtotal == display_modes[i]->vdisplay)
        //{
            std::cout << "DisplayMode: " << std::endl;
            std::cout << "  hdisplay: " << display_modes[i]->hdisplay << std::endl;
            std::cout << "  htotal: " << display_modes[i]->htotal << std::endl;
            std::cout << "  vdisplay: " << display_modes[i]->vdisplay << std::endl;
            std::cout << "  vtotal: " << display_modes[i]->vtotal << std::endl;
            sort_modes.push_back(*(display_modes[i]));
            //sort_modes.back().vtotal = sort_modes.back().vdisplay;
            //sort_modes.back().htotal = sort_modes.back().hdisplay;
        //}

    }
    XFree(display_modes);
    std::sort(sort_modes.begin()+1, sort_modes.end(), XF86VidModeModeInfoCompare());

    mode_count = sort_modes.size();
    modes = new XF86VidModeModeInfo [mode_count];
    for (unsigned int i = 0; i < mode_count; ++i)
    {
        modes[i] = sort_modes[i];
    }
}
DisplayModeManagerPimpl::~DisplayModeManagerPimpl()
{
    manager_instances -= 1;
    if (WindowData::NoWindowsAlive() && manager_instances == 0)
    {
        WindowData::CloseDisplay();
    }
    delete modes;
}


bool DisplayModeManagerPimpl::NoManagersAlive()
{
    return manager_instances == 0;

}
