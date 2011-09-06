#ifndef _WINDOW_EVENTS_DIRECTX10_SIZING_H_
#define _WINDOW_EVENTS_DIRECTX10_SIZING_H_

//Full Screen and Size Methods
bool WindowEvents::EnterFullscreen()
{
	SetShowTitleBar(false);
	SetAlwaysOnTop(true);
	ShowWindow(data->GetWindowHandle(), SW_MAXIMIZE);
	UpdateWindow(data->GetWindowHandle());	

	SetClipMouseToClientArea(true);
	return true;
}
void WindowEvents::ExitFullscreen()
{
	ShowWindow(data->GetWindowHandle(), SW_RESTORE);
	SetAlwaysOnTop(false);
	SetShowTitleBar(true);
	UpdateWindow(data->GetWindowHandle());

	SetClipMouseToClientArea(false);
}

bool WindowEvents::IsMaximized()
{
	return IsZoomed(this->data->GetWindowHandle());
}

void WindowEvents::ToggleMaximize()
{
	if (!initialized) return;

	if (IsZoomed(this->data->GetWindowHandle()))
	{
		//Restore
		ShowWindow(this->data->GetWindowHandle(), SW_RESTORE);
		UpdateWindow(this->data->GetWindowHandle());
	}
	else
	{
		//Maximize
		ShowWindow(this->data->GetWindowHandle(), SW_MAXIMIZE);
		UpdateWindow(this->data->GetWindowHandle());
	}
}
void WindowEvents::Minimize()
{
	if (!initialized) return;

	ShowWindow(this->data->GetWindowHandle(), SW_MINIMIZE);
	UpdateWindow(this->data->GetWindowHandle());
}

void WindowEvents::SetSize(int w, int h)
{
	SetWindowPos(this->data->GetWindowHandle(), 0, 0, 0, w, h, SWP_NOMOVE | SWP_NOZORDER);
	width = w;
	height = h;
}

#endif