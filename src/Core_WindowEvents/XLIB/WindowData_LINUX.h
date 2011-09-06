#ifndef _WINDOW_DATA_LINUX_H_
#define _WINDOW_DATA_LINUX_H_

#include <X11/Xlib.h>
#include <X11/extensions/xf86vmode.h>
#include <GL/glx.h>
#include <GL/glxext.h>

#include <map>

#include <iostream>

class WindowEvents;

class WindowData
{
	friend class WindowEvents;
	WindowEvents * window_events;

    //Instance Variables
    Window wnd;
    bool auto_repeat_on;
	int prev_mousex;
	int prev_mousey;
	bool cursor_shown;
	int fullscreen_mode;
	bool title_bar_visible;
	int title_bar_height;

	//Static
	static Display * display;
	static WindowEvents * active_window;
	static std::map<Window, WindowEvents *> * handle_map_window;

    static PFNGLXSWAPINTERVALSGIPROC glXSwapIntervalSGI;
public:
    WindowData(WindowEvents * window_events);
	~WindowData();

	//Window and Display
	inline Window & GetWindow();
	inline void SetWindow(Window & in_window);
	void RecreateWindow(const GLXFBConfig & glx_fbconfig);
	static inline Display * GetDisplay();

    //Window Events
	static WindowEvents * FindWindowEvents(Window & win);

    //Mouse cursor
	inline int GetPrevMouseX() const;
	inline int GetPrevMouseY() const;
	inline void SetPrevMouseX(int x);
	inline void SetPrevMouseY(int x);
	static void SetVSync(bool sync);

	//Title Bar
	inline bool GetTitleVisible() const;
	inline void SetTitleVisible(bool visible);

	//Display
	static void OpenDisplay();
	static void CloseDisplay();
	static bool NoWindowsAlive();
};

inline Display * WindowData::GetDisplay()
{
	return display;
}

inline Window & WindowData::GetWindow()
{
	return wnd;
}

inline void WindowData::SetWindow(Window & in_window)
{
	wnd = in_window;
}

inline int WindowData::GetPrevMouseX() const
{
	return prev_mousex;
}
inline int WindowData::GetPrevMouseY() const
{
	return prev_mousey;
}
inline void WindowData::SetPrevMouseX(int x)
{
	prev_mousex = x;
}
inline void WindowData::SetPrevMouseY(int y)
{
	prev_mousey = y;
}
inline bool WindowData::GetTitleVisible() const
{
    return title_bar_visible;
}
inline void WindowData::SetTitleVisible(bool visible)
{
    title_bar_visible = visible;
}



#endif
