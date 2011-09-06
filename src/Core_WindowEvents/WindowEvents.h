#ifndef _WINDOWEVENTS_H_
#define _WINDOWEVENTS_H_

#include "InputManager.h"

class WindowData;

class RenderContext;
class WindowPixelFormat;

class WindowEvents
{
private:
	int width, height;
	WindowData * data;
	bool auto_repeat;
	bool initialized;
	bool visible;
	bool fullscreen;
	static int window_count;

	InputManager input_manager;
	RenderContext * render_context;

	//Style
	bool maximizable;
	bool minimizable;
	bool resizable;
public:
	WindowEvents();
	WindowEvents(int width, int height);

	~WindowEvents();

	static int ProcessEvents();
	void FlipBuffers();

	inline int GetWidth();
	inline int GetHeight();
	inline bool AutoRepeat() const;
	inline void SetAutoRepeat(bool on);
	inline InputManager & GetInputManager();
	void SetTitle(const char * text);

	void MakeActive();
	void MakeCurrentDraw();
	void MakeCurrentDraw(RenderContext * render_context);
	RenderContext * NewRenderContext(const WindowPixelFormat * pixel_format);
	RenderContext * NewRenderContextShared(RenderContext * share_render_context);
	void SetRenderContext(RenderContext * render_context);
	bool Open();
	void SetVSync(bool sync);
    inline RenderContext * GetRenderContext() const;

	inline bool IsAlive() const;
	inline bool IsVisible() const;
	void Show();
	void Hide();
	void Close();
	bool IsMaximized();
	void ToggleMaximize();
	void Minimize();
	bool EnterFullscreen();
	void ExitFullscreen();
	void SetSize(int w, int h);

	static int GetWindowCount();
	static WindowEvents * GetActive();

	void SetMousePosition(int x, int y);
	void SetShowMouse(bool show);

	//

	inline bool Maximizable() const;
	inline bool Minimizable() const;
	inline bool Resizable() const;

	bool TitleBarVisible() const;
	void SetTopLeftPosition(int x, int y);
	void SetAlwaysOnTop(bool top_most);
	void SetShowTitleBar(bool show);
	void SetClipMouseToClientArea(bool clip);
	void Configure(bool can_maximize, bool can_minimize, bool resizable);
	void GetTopLeftPosition(int & x, int & y) const;
	void ScreenToClientCoordinates(int screen_x, int screen_y, int & rtn_client_x, int & rtn_client_y) const;
	void ClientToScreenCoordinates(int client_x, int client_y, int & rtn_screen_x, int & rtn_screen_y) const;

	void GetClientTopLeft(int & x, int & y) const;
	void GetFrameWidths(int & left, int & bottom, int & right, int & top) const;
	void GetClientDimensions(unsigned int & w, unsigned int & h) const;

};

inline int WindowEvents::GetWidth()
{
	return width;
};
inline int WindowEvents::GetHeight()
{
	return height;
};

inline bool WindowEvents::AutoRepeat() const
{
	return auto_repeat;
};

inline void WindowEvents::SetAutoRepeat(bool on)
{
	auto_repeat = on;
};

inline bool WindowEvents::IsAlive() const
{
	return initialized;
}

inline bool WindowEvents::IsVisible() const
{
	return IsAlive() && visible;
}

inline bool WindowEvents::Maximizable() const
{
	return maximizable;
}
inline bool WindowEvents::Minimizable() const
{
	return minimizable;
}
inline bool WindowEvents::Resizable() const
{
	return resizable;
}

inline InputManager & WindowEvents::GetInputManager()
{
	return input_manager;
}

inline RenderContext * WindowEvents::GetRenderContext() const
{
    return render_context;
}


#endif
