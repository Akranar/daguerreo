#ifndef _KEY_EVENT_H_
#define _KEY_EVENT_H_

#include "KeyID.h"

class KeyEvent
{
public:
	enum Type
	{
		KEY_UP,
		KEY_DOWN,
		KEY_REPEAT
	};
private:
	Type type;
	KeyID::Key key;

public:
	inline void SetType(Type type);
	inline void SetKey(KeyID::Key key);
	
	inline bool KeyUp() const;
	inline bool KeyDown() const;
	inline bool KeyRepeat() const;

	inline KeyID::Key GetKey() const;
	inline Type GetType() const;
};

inline void KeyEvent::SetType(Type in_type)
{
	type = in_type;
}
inline void KeyEvent::SetKey(KeyID::Key in_key)
{
	key = in_key;
};

inline bool KeyEvent::KeyUp() const
{
	return type == KEY_UP;
}
inline bool KeyEvent::KeyDown() const
{
	return type == KEY_DOWN;
}
inline bool KeyEvent::KeyRepeat() const
{
	return type == KEY_REPEAT;
}

inline KeyID::Key KeyEvent::GetKey() const
{
	return key;
}

inline KeyEvent::Type KeyEvent::GetType() const
{
	return type;
}

#endif
