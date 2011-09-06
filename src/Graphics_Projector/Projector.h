#ifndef _PROJECTOR_H_
#define _PROJECTOR_H_

#include <cmath> //Note: apparently needs to be included before windows.h

class Projector
{
	//Frustum Paramers
	float left, right, top, bottom, near, far;
	//Projector Orientation
	float position[3];
	float direction[3];
	float up[3];
	bool updated;
public:
	Projector()
		:
	updated(false)
	{
	}

	inline float * GetPosition();
	inline float * GetDirection();
	inline float * GetUp();

	inline void Normalize(float vect[3]);
	inline void SetPosition(float x, float y, float z);
	inline void SetDirection(float x, float y, float z);
	inline void SetUp(float x, float y, float z);
	inline void SetPosition(const float * _position);
	inline void SetDirection(const float * _direction);
	inline void SetUp(const float * _up);
	inline void LookAt(const float * _position, const float * target, const float * _up);
	inline void LookAt(float pos_x, float pos_y, float pos_z, float target_x, float target_y, float target_z, float up_x, float up_y, float up_z);
	inline void SetUpdated(bool _updated);
	inline bool IsUpdated() const;
	inline void SetPerspective(float _left, float _right, float _bottom, float _top, float _distance_to_near, float _distance_to_far);
	inline void SetPerspective(float _fov, float _aspect_ratio, float _distance_to_near, float _distance_to_far);
	inline void SetOrthographic(float _left, float _right, float _bottom, float _top, float _distance_to_near, float _distance_to_far);

	void ViewMatrix(float * out_view_matrix);
	void PerspectiveMatrix(float * out_proj_matrix);
	void ViewProjMatrix(float * out_viewprojmatrix);
};

inline float * Projector::GetPosition()
{
	return position;
}
inline float * Projector::GetDirection()
{
	return direction;
}
inline float * Projector::GetUp()
{
	return up;
}

inline void Projector::Normalize(float vect[3])
{
	float length_sq = vect[0] * vect[0] + vect[1] * vect[1] + vect[2] * vect[2];
	if (length_sq == 0) return;
	float length = sqrt(length_sq);
	if (length - 1 < 1e-20) return;
	vect[0] /= length;
	vect[1] /= length;
	vect[2] /= length;
}
inline void Projector::SetPosition(float x, float y, float z)
{
	position[0] = x;
	position[1] = y;
	position[2] = z;
	updated = true;
}
inline void Projector::SetDirection(float x, float y, float z)
{
	direction[0] = x;
	direction[1] = y;
	direction[2] = z;
	updated = true;
}
inline void Projector::SetUp(float x, float y, float z)
{
	up[0] = x;
	up[1] = y;
	up[2] = z;
	updated = true;
}
inline void Projector::SetPosition(const float * _position)
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];
	updated = true;
}
inline void Projector::SetDirection(const float * _direction)
{
	direction[0] = _direction[0];
	direction[1] = _direction[1];
	direction[2] = _direction[2];
	updated = true;
}
inline void Projector::SetUp(const float * _up)
{
	up[0] = _up[0];
	up[1] = _up[1];
	up[2] = _up[2];
	updated = true;
}
inline void Projector::LookAt(const float * _position, const float * target, const float * _up)
{
	position[0] = _position[0];
	position[1] = _position[1];
	position[2] = _position[2];

	direction[0] = target[0] - position[0];
	direction[1] = target[1] - position[1];
	direction[2] = target[2] - position[2];

	up[0] = _up[0];
	up[1] = _up[1];
	up[2] = _up[2];

	Normalize(direction);
	Normalize(up);
	updated = true;
}
inline void Projector::LookAt(float pos_x, float pos_y, float pos_z, float target_x, float target_y, float target_z, float up_x, float up_y, float up_z)
{
	position[0] = pos_x;
	position[1] = pos_y;
	position[2] = pos_z;

	direction[0] = target_x - pos_x;
	direction[1] = target_y - pos_y;
	direction[2] = target_z - pos_z;

	up[0] = up_x;
	up[1] = up_y;
	up[2] = up_z;

	Normalize(direction);
	Normalize(up);
	updated = true;
}
inline void Projector::SetUpdated(bool _updated)
{
	updated = _updated;
}
inline bool Projector::IsUpdated() const
{
	return updated;
}

inline void Projector::SetPerspective(float _left, float _right, float _bottom, float _top, float _distance_to_near, float _distance_to_far)
{
	left = _left;
	right = _right;
	top = _top;
	bottom = _bottom;
	near = fabs(_distance_to_near);
	far = fabs(_distance_to_far);
	updated = true;
}
inline void Projector::SetPerspective(float _fov, float _aspect_ratio, float _distance_to_near, float _distance_to_far)
{
	near = _distance_to_near;
	far = _distance_to_far;
	top = float(tan(_fov * 3.1415926536 / 360) * near);
	bottom = -top;
	right = _aspect_ratio * top;
	left = -right;
	updated = true;
}
inline void Projector::SetOrthographic(float _left, float _right, float _bottom, float _top, float _distance_to_near, float _distance_to_far)
{
	left = _left;
	right = _right;
	top = _top;
	bottom = _bottom;
	near = _distance_to_near;
	far = _distance_to_far;
	updated = true;
}

#endif