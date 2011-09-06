#ifndef _MOUSE_MOTION_EVENT_H_
#define _MOUSE_MOTION_EVENT_H_

class MouseMotionEvent
{
private:
	short x, y;
	short relx, rely;

public:
	inline short GetX() const;
	inline short GetY() const;
	inline short GetRelX() const;
	inline short GetRelY() const;

	inline void SetX(short value);
	inline void SetY(short value);
	inline void SetRelX(short value);
	inline void SetRelY(short value);
};

inline short MouseMotionEvent::GetX() const
{
	return x;
}
inline short MouseMotionEvent::GetY() const
{
	return y;
}
inline short MouseMotionEvent::GetRelX() const
{
	return relx;
}
inline short MouseMotionEvent::GetRelY() const
{
	return rely;
}

inline void MouseMotionEvent::SetX(short value)
{
	x = value;
}
inline void MouseMotionEvent::SetY(short value)
{
	y = value;
}
inline void MouseMotionEvent::SetRelX(short value)
{
	relx = value;
}
inline void MouseMotionEvent::SetRelY(short value)
{
	rely = value;
}

#endif