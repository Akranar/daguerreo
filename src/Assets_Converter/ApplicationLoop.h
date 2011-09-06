#ifndef _APPLICATION_LOOP_H_
#define _APPLICATION_LOOP_H_

#include "ExLib_WindowEvents.h"
#include <Core_DoubleTimer.h>

//*****************************************************************************
//	Note:
//		-> The reason this class exists and why it is a templated class
//			-> RendererBase is likely a member of GAME.
//			-> In OpenGL:
//				-> RendererBase's constructor initializes GLEW
//					-> Requires OpenGL Context Already Created.
//						-> Means WindowEvents::NewRenderContext
//							-> must already have been called.
//			-> With GAME as template parameter,
//				-> Can call GAME's constructor after RenderContext Created.
//*****************************************************************************

template <class GAME>
class ApplicationLoop
{
	double frames_per_second;
	bool show_fps_in_title;
public:
	ApplicationLoop();
	virtual ~ApplicationLoop();
	void Run();
	inline double GetFPS() const;
	inline void ShowFPSInTitle(bool value);
};

template <class GAME>
ApplicationLoop<GAME>::ApplicationLoop()
:
show_fps_in_title(true)
{

}

template <class GAME>
ApplicationLoop<GAME>::~ApplicationLoop()
{
}

template <class GAME>
inline double ApplicationLoop<GAME>::GetFPS() const
{
	return frames_per_second;
}

template <class GAME>
inline void ApplicationLoop<GAME>::ShowFPSInTitle(bool value)
{
	show_fps_in_title = value;
}

template <class GAME>
void ApplicationLoop<GAME>::Run()
{
	WindowEvents * window = new WindowEvents(800, 600);
	window->Open();

	WindowPixelFormat pixel_format;
	pixel_format.SetAll(8, 8, 8, 8, 24, 8, 1, 4);
	//pixel_format.SetAll(8, 8, 8, 8, 24, 8, 0, 1);
	RenderContext * render_context = window->NewRenderContext(&pixel_format);
	window->MakeCurrentDraw(render_context);
	window->SetVSync(true);


	char title[100];
	DoubleTimer timer;
	timer.StartTimer();
	int frames = 0;

	GAME game;

	game.Init(window);
	window->Show();
	double start_time = timer.GetTimeInSeconds();


	while (WindowEvents::ProcessEvents())
	{
		game.Update(timer.GetTimeInSeconds(), window);

		if (window->IsVisible() && window->IsAlive())
		{
			game.Draw();
			window->FlipBuffers();
		}

		frames++;
		if ((timer.GetTimeInSeconds() - start_time) > 1.0)
		{
			frames_per_second = double(frames) / (timer.GetTimeInSeconds() - start_time);
			start_time = timer.GetTimeInSeconds();
			frames = 0;

			if (show_fps_in_title && window->IsVisible())
			{
				sprintf (title, "FPS: %f", frames_per_second);
				window->SetTitle(title);
			}
		}
	}

	game.Deinit();

	delete render_context;
	delete window;
}


#endif