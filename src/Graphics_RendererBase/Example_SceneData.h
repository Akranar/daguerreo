#ifndef _EXAMPLE_SCENE_DATA_H_
#define _EXAMPLE_SCENE_DATA_H_

#include "MouseLookController.h"
#include "ExLib_Projector.h"
#include "ExLib_WindowEvents.h"

class Example_SceneData
{
public:
	Projector camera;
	Projector light_source;

	MouseLookController camera_controller;
	int start_x, start_y;

	void Init()
	{
		camera.SetPerspective(45.0f, 1.0f, 1.0, 500000);
		camera.LookAt(1.0f, -10.0f, 10.0f, 0, 0, 0, 0, 0, 1);
		camera_controller.SetProjector(&camera, Eigen::Vector3f(0, 0, 1));
		camera_controller.SetSpeed(20);
		light_source = camera;
	}

	void UpdateCameraPosition(WindowEvents * window_events, double seconds_elapsed)
	{
		InputManager * input_manager = &window_events->GetInputManager();
		const KeyState & key_state = input_manager->GetKeyState();
		camera_controller.MovementUpdate(key_state.GetState(KeyID::KEY_W),
								key_state.GetState(KeyID::KEY_A),
								key_state.GetState(KeyID::KEY_D),
								key_state.GetState(KeyID::KEY_S),
								key_state.GetState(KeyID::KEY_E),
								key_state.GetState(KeyID::KEY_C),
								camera_controller.GetDistance(),
								seconds_elapsed
								);
	}

	void UpdateCameraOrientation(WindowEvents * window_events, double seconds_elapsed) 
	{ 
		const MouseState & mouse_state = window_events->GetInputManager().GetMouseState(); 
		const KeyState & key_state = window_events->GetInputManager().GetKeyState(); 
		const InputEvent & input_event = window_events->GetInputManager().PeekNext();
		switch (input_event.GetType()) 
		{ 
			case InputEvent::RESIZE: 
			{ 
				camera_controller.SetViewport(input_event.data.resize.GetWidth(), input_event.data.resize.GetHeight()); 
				light_source.SetPerspective(45.0f, float(input_event.data.resize.GetWidth()) / float(input_event.data.resize.GetHeight()), 1.0f, 500000.0f); 
			} 
			break; 
			case InputEvent::WINDOW_ACTIVATION: 
			{ 
				if (!input_event.data.window_activation.GetActive()) 
				{ 
					window_events->SetShowMouse(true); 
				} 
			} 
			break; 
			case InputEvent::MOUSE_BUTTON: 
			{ 
				switch (input_event.data.mouse_button.GetButton()) 
				{ 
					case MouseButtonEvent::BUTTON_LEFT: 
						if (input_event.data.mouse_button.GetState() == MouseButtonEvent::PRESSED) 
						{ 
							start_x = mouse_state.GetX(); 
							start_y = mouse_state.GetY(); 
							window_events->SetShowMouse(false); 
						} 
						else 
						{ 
							window_events->SetShowMouse(true); 
						} 
					break; 
					case MouseButtonEvent::BUTTON_RIGHT: 
						if (input_event.data.mouse_button.GetState() == MouseButtonEvent::PRESSED) 
						{ 
							start_x = mouse_state.GetX(); 
							start_y = mouse_state.GetY(); 
						} 
					break; 
				} 
			} 
			break; 
			case InputEvent::MOUSE_MOTION: 
			{ 
				if (mouse_state.GetLeft()) 
				{ 
					camera_controller.Drag(input_event.data.mouse_motion.GetRelX(), input_event.data.mouse_motion.GetRelY()); 
					window_events->SetMousePosition(start_x, start_y); 
				} 
			} 
			break; 
		} 
	}
};

#endif