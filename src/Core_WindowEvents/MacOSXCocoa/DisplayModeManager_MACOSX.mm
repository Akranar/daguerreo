#include "../DisplayModeManager.h"
#include "DisplayModeManagerPimpl_MACOSX.h"
#include <iostream>
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
	CGDisplayErr error = CGDisplaySetDisplayMode(kCGDirectMainDisplay, data->modes[i], NULL);
	
    if (error != CGDisplayNoErr)
    {
        std::cout << "Error Changing Display Mode." << std::endl;
    }
}

void DisplayModeManager::SetDefaultMode()
{
	CGDisplayErr error = CGDisplaySetDisplayMode(kCGDirectMainDisplay, data->modes[0], NULL);
	
    if (error != CGDisplayNoErr)
    {
        std::cout << "Error Changing Display Mode." << std::endl;
    }
}

int DisplayModeManager::GetWidth(int i) const
{
	return CGDisplayModeGetWidth(data->modes[i]);
}
int DisplayModeManager::GetHeight(int i) const
{
	return CGDisplayModeGetHeight(data->modes[i]);
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
	SetDefaultMode();
	window_events->ExitFullscreen();
	
	return true;
}

