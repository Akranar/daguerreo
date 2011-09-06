#ifndef _INPUT_MANAGER_H_
#define _INPUT_MANAGER_H_

//#define _INPUT_MANAGER_VERBOSE_
#define MAX_QUEUE_LENGTH 1000

#include "InputEvent.h"
#include "MouseState.h"
#include "KeyState.h"

#include <vector>
#include <iostream>

class InputManager
{
private:
	unsigned int next;
	unsigned int last;
	unsigned int queued_count;
	InputEvent queue[MAX_QUEUE_LENGTH];

	static MouseState mouse_state;
	static KeyState key_state;
public:
	inline InputManager();
	//inline void PushEvent(const MouseEvent & mouse_event);
	inline void PushEvent(const KeyEvent & key_event);
	inline void PushEvent(const InputEvent & input_event);

	inline const InputEvent & PeekNext() const;
	inline void ProcessNext();
	inline void PopEvent();
	inline bool HasEvents() const;

	inline const MouseState & GetMouseState() const;
	inline const KeyState & GetKeyState() const;

	inline unsigned int GetEventCount() const;
};

inline InputManager::InputManager()
{
	next = 0;
	last = 0;
	queued_count = 0;
};


inline void InputManager::PushEvent(const KeyEvent & key_event)
{
#ifdef _INPUT_MANAGER_VERBOSE_
	if (key_event.KeyUp())
	{
		std::cout << "KEY_UP" << std::endl;
	}
	else if (key_event.KeyDown())
	{
		std::cout << "KEY_DOWN" << std::endl;
	}
	else if (key_event.KeyRepeat())
	{
		std::cout << "KEY_REPEAT" << std::endl;
	}
#endif

}
/*
inline void InputManager::PushEvent(const MouseEvent & mouse_event)
{
#ifdef _INPUT_MANAGER_VERBOSE_
	if (mouse_event.LeftDown())
	{
		std::cout << "MOUSE_LEFT_BUTTON_DOWN" << std::endl;
	}
	else if (mouse_event.LeftUp())
	{
		std::cout << "MOUSE_LEFT_BUTTON_UP" << std::endl;
	}
	else if (mouse_event.MiddleDown())
	{
		std::cout << "MOUSE_MIDDLE_BUTTON_DOWN" << std::endl;
	}
	else if (mouse_event.MiddleUp())
	{
		std::cout << "MOUSE_MIDDLE_BUTTON_UP" << std::endl;
	}
	else if (mouse_event.RightDown())
	{
		std::cout << "MOUSE_RIGHT_BUTTON_DOWN" << std::endl;
	}
	else if (mouse_event.RightUp())
	{
		std::cout << "MOUSE_RIGHT_BUTTON_UP" << std::endl;
	}
	else if (mouse_event.Button4Down())
	{
		std::cout << "MOUSE_BUTTON_4_DOWN" << std::endl;
	}
	else if (mouse_event.Button4Up())
	{
		std::cout << "MOUSE_BUTTON_4_UP" << std::endl;
	}
	else if (mouse_event.Button5Down())
	{
		std::cout << "MOUSE_BUTTON_5_DOWN" << std::endl;
	}
	else if (mouse_event.Button5Up())
	{
		std::cout << "MOUSE_BUTTON_5_UP" << std::endl;
	}
	else if (mouse_event.MouseWheelDown())
	{
		std::cout << "MOUSE_WHEEL_DOWN: " << mouse_event.GetWheelMagnitude() << std::endl;
	}
	else if (mouse_event.MouseWheelUp())
	{
		std::cout << "MOUSE_WHEEL_UP: " << mouse_event.GetWheelMagnitude() << std::endl;
	}
	else if (mouse_event.MouseMove())
	{
		std::cout << mouse_event.GetX() << ", " << mouse_event.GetY() << ", " << mouse_event.GetRelX() << ", " << mouse_event.GetRelY() << std::endl;
	}
#endif
	
};
*/


inline void InputManager::ProcessNext()
{
	switch (queue[next].type)
	{
		case InputEvent::KEYBOARD:
			{
				key_state.SetState(queue[next].data.key);
			}
			break;
		case InputEvent::MOUSE_MOTION:
			{
				mouse_state.SetState(queue[next].data.mouse_motion);
			}
			break;
		case InputEvent::MOUSE_BUTTON:
			{
				mouse_state.SetState(queue[next].data.mouse_button);
			}
			break;
		default:
			break;
	};

#ifdef _INPUT_MANAGER_VERBOSE_
	std::cout << "MouseState: " << std::endl;
	std::cout << "	( " << mouse_state.mousex << ", " << mouse_state.mousey << " ) " << std::endl;
	std::cout << "	( " << mouse_state.relx << ", " << mouse_state.rely << " ) " << std::endl;
	std::cout << "	";
	for (unsigned int i = 0; i < 5; ++i)
	{
		std::cout << mouse_state.mouse_down[i] << ", ";
	}
	std::cout << std::endl;
#endif
}

inline void InputManager::PushEvent(const InputEvent & input_event)
{
	queue[last] = input_event;
	queued_count += 1;
	last = ((last + 1) % MAX_QUEUE_LENGTH);
}

inline const InputEvent & InputManager::PeekNext() const
{
	return queue[next];
}

inline void InputManager::PopEvent()
{
	next = ((next + 1) % MAX_QUEUE_LENGTH);
	queued_count -= 1;
}

inline bool InputManager::HasEvents() const
{
	return queued_count > 0;
}

inline const MouseState & InputManager::GetMouseState() const
{
	return mouse_state;
}

inline const KeyState & InputManager::GetKeyState() const
{
	return key_state;
}

inline unsigned int InputManager::GetEventCount() const
{
	return queued_count;
}

#endif
