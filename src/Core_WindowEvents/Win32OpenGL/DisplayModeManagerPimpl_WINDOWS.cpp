#include "DisplayModeManagerPimpl_WINDOWS.h"
#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

DisplayModeManagerPimpl::DisplayModeManagerPimpl()
{
	std::set<std::pair<int, int> > unique_resolutions;
	std::vector<std::pair<int, int> > sorted_modes;

	DEVMODE display_setting;
	display_setting.dmSize = sizeof(DEVMODE);
	
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &display_setting);
	int i = 0;
	int bpp;
	bpp = display_setting.dmBitsPerPel;
	sorted_modes.push_back(std::make_pair(display_setting.dmPelsWidth, display_setting.dmPelsHeight));

	while (EnumDisplaySettings(NULL, i, &display_setting))
	{
		if (display_setting.dmBitsPerPel == bpp)
		{
			unique_resolutions.insert(std::make_pair(display_setting.dmPelsWidth, display_setting.dmPelsHeight));
		}
		++i;
	}

	for (std::set<std::pair<int, int> >::iterator it = unique_resolutions.begin(); it != unique_resolutions.end(); ++it)
	{
		sorted_modes.push_back(*it);
	}
	std::sort(sorted_modes.begin()+1, sorted_modes.end());

	mode_count = sorted_modes.size();
	modes = new DisplayMode[mode_count];

	for (unsigned int i = 0; i < sorted_modes.size(); ++i)
	{
		modes[i].width = sorted_modes[i].first;
		modes[i].height = sorted_modes[i].second;
	}

}
DisplayModeManagerPimpl::~DisplayModeManagerPimpl()
{
	delete [] modes;
}