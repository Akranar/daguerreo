#include "../DisplayModeManager.h"
#include "DisplayModeManagerPimpl_DIRECTX10.h"
#include <windows.h>
#include "../WindowEvents.h"


DisplayModeManager::DisplayModeManager()
{
	data = new DisplayModeManagerPimpl();
}
DisplayModeManager::~DisplayModeManager()
{
	delete data;
}

void DisplayModeManager::SetMode(int i)
{
	DEVMODE dev;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dev);
	dev.dmPelsWidth = GetWidth(i);
	dev.dmPelsHeight = GetHeight(i);
	ChangeDisplaySettings(&dev, CDS_FULLSCREEN);
}

void DisplayModeManager::SetDefaultMode()
{
	ChangeDisplaySettings(NULL, 0);
}

int DisplayModeManager::GetWidth(int i) const
{
	return data->modes[i].width;
}
int DisplayModeManager::GetHeight(int i) const
{
	return data->modes[i].height;
}
unsigned int DisplayModeManager::GetModeCount() const
{
	return data->mode_count;
}

bool DisplayModeManager::MakeWindowFullscreen(WindowEvents * window_events, int width, int height)
{
	SetMode(SelectBest(width, height));
	window_events->EnterFullscreen();
	return true;
}
bool DisplayModeManager::ExitFullscreen(WindowEvents * window_events)
{
	window_events->ExitFullscreen();
	SetDefaultMode();
	return true;
}

