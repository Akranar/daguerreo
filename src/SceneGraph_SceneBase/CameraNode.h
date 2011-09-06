#ifndef _CAMERA_NODE_H_
#define _CAMERA_NODE_H_

#include "Spatial.h"

class Projector;

/*
	Treats Projector As:
		-> Position = (0, 0, 0)
		-> Up = (0, 1, 0)
		-> Right = (1, 0, 0)
		-> LookDirection = (0, 0, -1)
*/

class CameraNode: public Spatial
{
private:
	Projector * camera;
public:
	CameraNode();
	virtual ~CameraNode();
	void AttachCamera(Projector * camera);
	Projector * GetCamera() const;

	void UpdateCamera() const;
	void OrientCamera(float look_direction [3]);
	void OrientWithUp();

	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
	
};

#endif