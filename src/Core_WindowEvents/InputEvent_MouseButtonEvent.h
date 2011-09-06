#ifndef _MOUSE_BUTTON_EVENT_H_
#define _MOUSE_BUTTON_EVENT_H_

class MouseButtonEvent
{
public:
	enum Button
	{
		BUTTON_LEFT,
		BUTTON_MIDDLE,
		BUTTON_RIGHT,
		BUTTON_4,
		BUTTON_5,
		BUTTON_COUNT
	};
	enum State
	{
		RELEASED,
		PRESSED
	};
private:
	Button button;
	State state;

public:
	inline Button GetButton() const;
	inline State GetState() const;
	inline void SetButton(Button button);
	inline void SetState(State state);
};

inline MouseButtonEvent::Button MouseButtonEvent::GetButton() const
{
	return button;
}
inline MouseButtonEvent::State MouseButtonEvent::GetState() const
{
	return state;
}
inline void MouseButtonEvent::SetButton(Button in_button)
{
	button = in_button;
}
inline void MouseButtonEvent::SetState(State in_state)
{
	state = in_state;
}



#endif