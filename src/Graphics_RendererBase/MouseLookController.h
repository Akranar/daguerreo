#ifndef _CAMERA_CONTROLLER_H_
#define _CAMERA_CONTROLLER_H_


class Projector;
#include "ExLib_Eigen.h"

class MouseLookController
{
public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;
private:
	unsigned int viewport_width, viewport_height;
	Projector * camera;
	Eigen::Affine3f camera_transform;
	Eigen::Vector3f global_up;


	//WASD
	double previous_time;
	float distance;
public:
	MouseLookController() 
		: 
		camera(0),
		camera_transform(Eigen::Affine3f::Identity()),
		global_up(Eigen::Vector3f(0, 1, 0)),
		previous_time(0),
		distance(15.0f)
	{

	};
	void SetProjector(Projector * _camera, const Eigen::Vector3f & world_up);
	void SetProjector(Projector * _camera, const Eigen::Vector3f & aim, const Eigen::Vector3f & world_up);
	

	void SetViewport(int width, int height);
	void OrientWithUp();
	void UpdateWorldTransform();
	void UpdateProjector();

	void Drag(int relx, int rely);
	void TranslateForward(float distance);
	void TranslateBack(float distance);

	//WASD
	void MovementUpdate(bool forward, bool left, bool right, bool back, float up, float down, float distance, double time);
	inline void IncreaseDistance(float increase);
	inline void DecreaseDistance(float decrease);
	inline void SetDistance(float dist);
	inline float GetDistance() const;
	inline void SetSpeed(float speed);
};

inline void MouseLookController::IncreaseDistance(float increase)
{
	distance += increase;
}
inline void MouseLookController::DecreaseDistance(float decrease)
{
	distance -= decrease;

	if (distance < 15.0f) distance = 15.0f;
}

inline void MouseLookController::SetDistance(float dist)
{
	distance = dist;
}
inline float MouseLookController::GetDistance() const
{
	return distance;
}

inline void MouseLookController::SetSpeed(float speed)
{
	distance = speed;
}

#endif