#ifndef _RESIZE_EVENT_H_
#define _RESIZE_EVENT_H_

class ResizeEvent
{
public:
	int width;
	int height;

	inline int GetWidth() const;
	inline int GetHeight() const;

	inline void SetWidth(int width);
	inline void SetHeight(int height);
};

inline int ResizeEvent::GetWidth() const
{
	return width;
}
inline int ResizeEvent::GetHeight() const
{
	return height;
}

inline void ResizeEvent::SetWidth(int in_width)
{
	width = in_width;
}
inline void ResizeEvent::SetHeight(int in_height)
{
	height = in_height;
}


#endif