#include "../DisplayModeManager.h"
#include "DisplayModeManagerPimpl_LINUX.h"

#include "WindowData_LINUX.h"
#include "../WindowEvents.h"


DisplayModeManager::DisplayModeManager()
{
	data = new DisplayModeManagerPimpl();
}
DisplayModeManager::~DisplayModeManager()
{
    SetDefaultMode();
	delete data;
}

void DisplayModeManager::SetMode(int i)
{
    XF86VidModeSwitchToMode(WindowData::GetDisplay(), 0, &data->modes[i]);
    XFlush(WindowData::GetDisplay());
}

void DisplayModeManager::SetDefaultMode()
{
    XF86VidModeSwitchToMode(WindowData::GetDisplay(), 0, &data->modes[0]);
    XFlush(WindowData::GetDisplay());
}

int DisplayModeManager::GetWidth(int i) const
{
    return data->modes[i].hdisplay;
}
int DisplayModeManager::GetHeight(int i) const
{
    return data->modes[i].vdisplay;
}
unsigned int DisplayModeManager::GetModeCount() const
{
    return data->mode_count;
}

int temp_x, temp_y;

bool DisplayModeManager::MakeWindowFullscreen(WindowEvents * window_events, int width, int height)
{
    int i = SelectBest(width, height);
    window_events->SetShowTitleBar(false);
    window_events->SetSize(data->modes[i].hdisplay, data->modes[i].vdisplay);
    window_events->SetTopLeftPosition(0, 0);
    SetMode(i);
    XF86VidModeSetViewPort(WindowData::GetDisplay(), 0, 0, 0);
    window_events->SetClipMouseToClientArea(true);
    return true;
}
bool DisplayModeManager::ExitFullscreen(WindowEvents * window_events)
{
    window_events->SetSize(window_events->GetWidth(), window_events->GetHeight());
    window_events->SetShowTitleBar(true);
    window_events->SetClipMouseToClientArea(false);
    SetDefaultMode();
    return true;
}
