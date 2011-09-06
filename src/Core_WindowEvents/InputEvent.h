#ifndef _INPUT_EVENT_H_
#define _INPUT_EVENT_H_


#include "InputEvent_KeyEvent.h"
#include "InputEvent_MouseWheelEvent.h"
#include "InputEvent_MouseMotionEvent.h"
#include "InputEvent_MouseButtonEvent.h"
#include "InputEvent_ResizeEvent.h"
#include "InputEvent_WindowActivationEvent.h"

class InputEvent
{
public:
	enum Type
	{
		KEYBOARD,
		MOUSE_MOTION,
		MOUSE_BUTTON,
		MOUSE_WHEEL,
		RESIZE,
		WINDOW_ACTIVATION
	};
public:
	Type type;
	union
	{
		KeyEvent key;
		MouseMotionEvent mouse_motion;
		MouseButtonEvent mouse_button;
		MouseWheelEvent mouse_wheel;
		ResizeEvent resize;
		WindowActivationEvent window_activation;
	} data;

	inline void SetType(Type in_type);
	inline Type GetType() const;
};

inline InputEvent::Type InputEvent::GetType() const
{
	return type;
}
inline void InputEvent::SetType(Type in_type)
{
	type = in_type;
}

#endif