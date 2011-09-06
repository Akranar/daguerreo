#ifndef _MOUSE_LOOK_CONTROLLER_DEFINES_H_
#define _MOUSE_LOOK_CONTROLLER_DEFINES_H_

#define MOUSELOOK_INIT \
	scene_data.camera.SetPerspective(45.0f, 1.0f, 1.0, 500000); \
	scene_data.camera.LookAt(1.0f, -10.0f, 10.0f, 0, 0, 0, 0, 0, 1); \
	camera_controller.SetProjector(&scene_data.camera, Eigen::Vector3f(0, 0, 1)); \
	camera_controller.SetSpeed(20); \
	scene_data.light_source = scene_data.camera;

#define MOUSELOOK_CONTROLLER_VAR_DECL \
	MouseLookController camera_controller; \
	int start_x, start_y; \
	SceneData scene_data;

#define UPDATE_CAMERA_DECL \
	void UpdateCamera(const InputEvent & input_event, WindowEvents * window_events, double seconds_elapsed) \
	{ \
		const MouseState & mouse_state = window_events->GetInputManager().GetMouseState(); \
		const KeyState & key_state = window_events->GetInputManager().GetKeyState(); \
		switch (input_event.GetType()) \
		{ \
			case InputEvent::RESIZE: \
			{ \
				camera_controller.SetViewport(input_event.data.resize.GetWidth(), input_event.data.resize.GetHeight()); \
				scene_data.light_source.SetPerspective(45.0f, float(input_event.data.resize.GetWidth()) / float(input_event.data.resize.GetHeight()), 1.0f, 500000.0f); \
			} \
			break; \
			case InputEvent::WINDOW_ACTIVATION: \
			{ \
				if (!input_event.data.window_activation.GetActive()) \
				{ \
					window_events->SetShowMouse(true); \
				} \
			} \
			break; \
			case InputEvent::MOUSE_BUTTON: \
			{ \
				switch (input_event.data.mouse_button.GetButton()) \
				{ \
					case MouseButtonEvent::BUTTON_LEFT: \
						if (input_event.data.mouse_button.GetState() == MouseButtonEvent::PRESSED) \
						{ \
							start_x = mouse_state.GetX(); \
							start_y = mouse_state.GetY(); \
							window_events->SetShowMouse(false); \
						} \
						else \
						{ \
							window_events->SetShowMouse(true); \
						} \
					break; \
					case MouseButtonEvent::BUTTON_RIGHT: \
						if (input_event.data.mouse_button.GetState() == MouseButtonEvent::PRESSED) \
						{ \
							start_x = mouse_state.GetX(); \
							start_y = mouse_state.GetY(); \
						} \
					break; \
				} \
			} \
			break; \
			case InputEvent::MOUSE_MOTION: \
			{ \
				if (mouse_state.GetLeft()) \
				{ \
					camera_controller.Drag(input_event.data.mouse_motion.GetRelX(), input_event.data.mouse_motion.GetRelY()); \
					window_events->SetMousePosition(start_x, start_y); \
				} \
			} \
			break; \
		} \
	}

#endif