#ifndef _MOUSE_STATE_H_
#define _MOUSE_STATE_H_

#include "InputEvent_MouseMotionEvent.h"
#include "InputEvent_MouseButtonEvent.h"

class MouseState
{
public:
	int mousex;
	int mousey;
	bool mouse_down[5];

	inline MouseState();
	inline void SetState(MouseMotionEvent & mouse_motion);
	inline void SetState(MouseButtonEvent & mouse_button);

	inline int GetX() const;
	inline int GetY() const;
	inline bool GetLeft() const;
	inline bool GetMiddle() const;
	inline bool GetRight() const;
	inline bool GetFourth() const;
	inline bool GetFifth() const;
};

inline MouseState::MouseState()
:
mousex(0),
mousey(0)
{
	mouse_down[0] = MouseButtonEvent::RELEASED;
	mouse_down[1] = MouseButtonEvent::RELEASED;
	mouse_down[2] = MouseButtonEvent::RELEASED;
	mouse_down[3] = MouseButtonEvent::RELEASED;
	mouse_down[4] = MouseButtonEvent::RELEASED;
}


inline void MouseState::SetState(MouseMotionEvent & mouse_motion)
{
	mousex = mouse_motion.GetX();
	mousey = mouse_motion.GetY();
}
inline void MouseState::SetState(MouseButtonEvent & mouse_button)
{
	mouse_down[mouse_button.GetButton()] = mouse_button.GetState() == MouseButtonEvent::PRESSED;
}

inline int MouseState::GetX() const
{
	return mousex;
}
inline int MouseState::GetY() const
{
	return mousey;
}

inline bool MouseState::GetLeft() const
{
	return mouse_down[0];
}
inline bool MouseState::GetMiddle() const
{
	return mouse_down[1];
}
inline bool MouseState::GetRight() const
{
	return mouse_down[2];
}
inline bool MouseState::GetFourth() const
{
	return mouse_down[3];
}
inline bool MouseState::GetFifth() const
{
	return mouse_down[4];
}


#endif