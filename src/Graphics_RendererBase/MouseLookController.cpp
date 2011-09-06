
#include "MouseLookController.h"
#include "ExLib_Projector.h"

void MouseLookController::SetProjector(Projector * _camera, const Eigen::Vector3f & world_up)
{
	Eigen::Vector3f aim = Eigen::Vector3f(_camera->GetDirection());
	SetProjector(_camera, aim, world_up);
}

void MouseLookController::SetProjector(Projector * _camera, const Eigen::Vector3f & aim, const Eigen::Vector3f & world_up)
{
	camera = _camera;
	global_up = world_up;

	Eigen::Vector3f right = aim.cross(world_up).normalized();
	Eigen::Vector3f up = right.cross(aim).normalized();
	Eigen::Vector3f pos = Eigen::Vector3f(_camera->GetPosition());

	camera_transform.matrix() << 
		right[0], up[0], -aim[0], pos[0],
		right[1], up[1], -aim[1], pos[1],
		right[2], up[2], -aim[2], pos[2],
		0, 0, 0, 1;

	UpdateProjector();
}
void MouseLookController::SetViewport(int width, int height)
{
	viewport_width = width;
	viewport_height = height;
}


//**********************************************************
//	Note: 
//	-> Eigen Block Syntax:
//		-> block<rows, columns>(row_index, column_index);
//	-> Transform Form:
//		-> [R, U, -D, P]
//	-> Object Space (Implicitely defined for this class.)
//		-> [R U D P]
//			-> 1 0  0 0
//			-> 0 1  0 0
//			-> 0 0 -1 0
//			-> 0 0  0 1
//	-> Transform Output:
//		-> [R, U, D, P]
//**********************************************************

void MouseLookController::OrientWithUp()
{
	//Recalculate R from D x global_up
	camera_transform.matrix().block<3, 1>(0, 0) = -camera_transform.matrix().block<3, 1>(0, 2).cross(global_up).normalized();
	//Recalculate U from D X R.
	camera_transform.matrix().block<3, 1>(0, 1) = camera_transform.matrix().block<3, 1>(0, 2).cross(camera_transform.matrix().block<3, 1>(0, 0));
}

void MouseLookController::UpdateProjector()
{
	Eigen::Vector3f direction = -camera_transform.matrix().block<3, 1>(0, 2);

	camera->SetPosition(camera_transform.matrix().block<3, 1>(0, 3).data());
	camera->SetDirection(direction.data());
	camera->SetUp(camera_transform.matrix().block<3, 1>(0, 1).data());
}
void MouseLookController::Drag(int relx, int rely)
{
	Eigen::Vector2f end_pos = Eigen::Vector2f(0, 0);

	end_pos += Eigen::Vector2f(relx, rely);

	Eigen::Vector3f start_3d = Eigen::Vector3f(0, 0, 1).normalized();
	Eigen::Vector3f end_3d = Eigen::Vector3f(float(end_pos[0])/float(viewport_width), float(-end_pos[1])/float(viewport_height), 0.5f);

	end_3d.normalize();
	Eigen::Vector3f axis = end_3d.cross(start_3d);
	
	if (axis.norm() > 1e-10)
	{
		axis.normalize();
		
		float angle = acos(start_3d.dot(end_3d));

		Eigen::Affine3f current_transform(Eigen::Affine3f::Identity());
		current_transform.rotate(Eigen::AngleAxis<float>(angle, axis));
		Eigen::Affine3f new_transform = camera_transform * current_transform;

		//Only allow new transform if Direction is not close to +Global Up or -Global Down.
		if (fabs(new_transform.matrix().block<3, 1>(0, 2).dot(global_up)) < 0.99)
		{
			camera_transform = new_transform;
			OrientWithUp();
			UpdateProjector();
		}
	}
}
void MouseLookController::TranslateForward(float distance)
{
	camera_transform.translate(Eigen::Vector3f(0, 0, -distance));
	UpdateProjector();
}
void MouseLookController::TranslateBack(float distance)
{
	camera_transform.translate(Eigen::Vector3f(0, 0, distance));
	UpdateProjector();
}

//WASD
void MouseLookController::MovementUpdate(bool forward, bool left, bool right, bool back, float up, float down, float distance, double time)
{
	double elapsed_time = time - previous_time;
	previous_time = time;

	Eigen::Vector3f direction = Eigen::Vector3f(float(int(right) - int(left)), float(int(up) - int(down)), float(int(back) - int(forward)));
	float length = direction.norm();
	if (length > 1e-10)
	{
		direction /= length;
		camera_transform.translate(direction * distance * float(elapsed_time));
		UpdateProjector();
	}
}