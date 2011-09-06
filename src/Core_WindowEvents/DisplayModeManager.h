#ifndef _DISPLAY_MODE_MANAGER_H_
#define _DISPLAY_MODE_MANAGER_H_

class DisplayModeManagerPimpl;
class WindowEvents;

class DisplayModeManager
{
	DisplayModeManagerPimpl * data;
public:
	DisplayModeManager();
	~DisplayModeManager();
	void SetMode(int i);
	void SetDefaultMode();
	int GetWidth(int i) const;
	int GetHeight(int i) const;
	unsigned int GetModeCount() const;
	unsigned int SelectBest(int width, int height) const;
	bool MakeWindowFullscreen(WindowEvents * window_events, int width, int height);
	bool ExitFullscreen(WindowEvents * window_events);
	
};

#endif