#ifndef _VISIBLE_OBJECT_H_
#define _VISIBLE_OBJECT_H_

class GlobalEffect;
class Visual;

class VisibleObject
{
private:
	Visual * visual;
	GlobalEffect * global_effect;
public:
	inline explicit VisibleObject(Visual * visual);
	inline explicit VisibleObject(GlobalEffect * global_effect);
	inline bool IsSentinel() const;
	inline Visual * GetVisual() const;
	inline GlobalEffect * GetGlobalEffect() const;
};

inline VisibleObject::VisibleObject(Visual * visual)
: 
visual(visual),
global_effect(0)
{

}
inline VisibleObject::VisibleObject(GlobalEffect * global_effect)
:
visual(),
global_effect(global_effect)
{

}

inline bool VisibleObject::IsSentinel() const
{
	return global_effect != 0;
}
inline Visual * VisibleObject::GetVisual() const
{
	return visual;
}
inline GlobalEffect * VisibleObject::GetGlobalEffect() const
{
	return global_effect;
}

#endif