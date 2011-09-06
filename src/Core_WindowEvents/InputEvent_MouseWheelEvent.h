#ifndef _MOUSE_WHEEL_EVENT_H_
#define _MOUSE_WHEEL_EVENT_H_

class MouseWheelEvent
{
public:
	enum Direction
	{
		UP,
		DOWN
	};
private:
	Direction direction;

public:
	
	inline Direction GetDirection() const;
	inline void SetDirection(Direction in_direction);
};

inline MouseWheelEvent::Direction MouseWheelEvent::GetDirection() const
{
	return direction;
}
inline void MouseWheelEvent::SetDirection(Direction in_direction)
{
	direction = in_direction;
}



#endif