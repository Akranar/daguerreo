#ifndef _KEY_STATE_H_
#define _KEY_STATE_H_

#include "KeyID.h"
#include "InputEvent_KeyEvent.h"

class KeyState
{
	bool key_down[KeyID::KEY_COUNT];
public:
	inline KeyState();
	inline void SetState(KeyEvent & key_event);
	inline bool GetState(KeyID::Key key) const;
};

inline KeyState::KeyState()
{
	for (unsigned int i = 0; i < KeyID::KEY_COUNT; ++i)
	{
		key_down[i] = false;
	}
}
inline void KeyState::SetState(KeyEvent & key_event)
{
	if (key_event.KeyDown())
	{
		key_down[key_event.GetKey()] = true;
	}
	else if (key_event.KeyUp())
	{
		key_down[key_event.GetKey()] = false;
	}
};

inline bool KeyState::GetState(KeyID::Key key) const
{
	return key_down[key];
}


#endif