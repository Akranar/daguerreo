#ifndef _WINDOW_DATA_DIRECTX10_H_
#define _WINDOW_DATA_DIRECTX10_H_

#include <windows.h>
#include <map>



class WindowData
{
	friend class WindowEvents;
	HWND handle_window;
	HDC handle_device_context;
	bool mouse_position_must_update;
	static std::map<HWND, WindowEvents *> * handle_map_window;

	static int vsync;
public:
	WindowData();
	~WindowData();
	static WindowEvents * FindWindowEvents(HWND & hWnd);
	inline const HWND & GetWindowHandle() const;
	inline const HDC & GetDeviceContext() const;
	inline void GetWindowHandle(HWND & hwnd);
	inline void GetDeviceContext(HDC & hdc);
	inline bool MouseRequiresUpdate() const;
	inline void SetMouseRequiresUpdate(bool value);

	inline void SetVSync(bool sync);
	inline int GetVSync() const;
};

inline const HWND & WindowData::GetWindowHandle() const
{
	return handle_window;
}
inline const HDC & WindowData::GetDeviceContext() const
{
	return handle_device_context;
}

inline void WindowData::GetWindowHandle(HWND & hwnd)
{
	handle_window = hwnd;
}
inline void WindowData::GetDeviceContext(HDC & hdc)
{
	handle_device_context = hdc;
}

inline bool WindowData::MouseRequiresUpdate() const
{
	return mouse_position_must_update;
}

inline void WindowData::SetMouseRequiresUpdate(bool value)
{
	mouse_position_must_update = value;
}

inline void WindowData::SetVSync(bool sync)
{
	vsync = sync ? 1 : 0;
}
inline int WindowData::GetVSync() const
{
	return vsync;
}



#endif