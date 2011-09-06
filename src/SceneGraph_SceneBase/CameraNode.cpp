#include "CameraNode.h"
#include "ExLib_RendererBase.h"
#include <cmath>

#include <iostream>

/*

CameraNode::CameraNode()
:
camera(0)
{

}
CameraNode::~CameraNode()
{
	if (camera) delete camera;
}

void CameraNode::AttachCamera(Projector * in_camera)
{
	camera = in_camera;
}
Projector * CameraNode::GetCamera() const
{
	return camera;
}

void CameraNode::UpdateCamera() const
{
	Eigen::Vector3f up = GetWorldTransform().linear() * Eigen::Vector3f(0, 1, 0);
	Eigen::Vector3f position = GetWorldTransform() * Eigen::Vector3f(0, 0, 0);
	Eigen::Vector3f direction = GetWorldTransform().linear() * Eigen::Vector3f(0, 0, -1);

	up.normalize();
	direction.normalize();

	camera->SetPosition(position.data());
	camera->SetDirection(direction.data());
	camera->SetUpDirection(up.data());
	
	camera->SetViewMatrix();
}

void CameraNode::OrientCamera(float look_direction [3])
{
	Eigen::Vector3f dir = VEC3(look_direction);
	dir.normalize();
	Eigen::Vector3f axis = Eigen::Vector3f(0, 0, -1).cross(dir);
	float angle = acos(Eigen::Vector3f(0, 0, -1).dot(dir));
	GetLocalTransform().rotate(Eigen::AngleAxis<float>(angle,axis));
}

void CameraNode::OrientWithUp()
{
	//Main Idea::
	//	-> Find global up vector in Projector space.
	//		-> Set Z component to zero
	//			-> Projects onto screen.
	//	-> Rotate camera's up vector in camera space 
	//		-> to projected global_up vector in camera space.

	Eigen::Vector3f global_up_cameraspace = AFF3(GetCamera()->GetViewMatrix()).linear() * Eigen::Vector3f(0, 0, 1);
	global_up_cameraspace[2] = 0;
	global_up_cameraspace.normalize();

	Eigen::Vector3f camera_up_objectspace = Eigen::Vector3f(0, 1, 0);
	
	float angle = acos(camera_up_objectspace.dot(global_up_cameraspace));
	Eigen::Vector3f axis = camera_up_objectspace.cross(global_up_cameraspace);
	axis.normalize();

	if (angle > 0 && (axis[0] >= 0 || axis[0] < 0)) //axis[0] is a number, not -1.#IND
	{
		GetLocalTransform().rotate(Eigen::AngleAxis<float>(angle,axis));
	}
}


//Serialization

void CameraNode::Serialize(Stream & stream) const
{
	Spatial::Serialize(stream);
}
void CameraNode::Unserialize(Stream & stream)
{
	Spatial::Unserialize(stream);
}
void CameraNode::SerializeTypeId(Stream & stream) const
{
	stream.Save("CameraNode");
}


*/