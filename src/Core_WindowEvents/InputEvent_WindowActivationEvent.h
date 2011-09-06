#ifndef _WINDOW_ACTIVATION_EVENT_H_
#define _WINDOW_ACTIVATION_EVENT_H_

class WindowActivationEvent
{
private:
	bool active;
public:
	inline void SetActive(bool value);
	inline bool GetActive() const;
};

inline void WindowActivationEvent::SetActive(bool value)
{
	active = value;
}
inline bool WindowActivationEvent::GetActive() const
{
	return active;
}

#endif