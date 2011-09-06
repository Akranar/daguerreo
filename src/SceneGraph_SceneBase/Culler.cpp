#include "Culler.h"
#include "VisibleSet.h"
#include "ExLib_RendererBase.h"
#include "ExLib_BoundingVolume.h"

Culler::Culler()
: 
camera(0),
visible_set(new VisibleSet())
{
}

Culler::~Culler()
{
	delete visible_set;
}

void Culler::SetFrustum()
{
	/*
	const float * near_plane = camera->GetNearPlane();
	const float * far_plane = camera->GetFarPlane();
	const float * left_plane = camera->GetLeftPlane();
	const float * bottom_plane = camera->GetBottomPlane();
	const float * right_plane = camera->GetRightPlane();
	const float * top_plane = camera->GetTopPlane();

	for (unsigned int i = 0; i < 4; ++i)
	{
		frustum[0][i] = near_plane[i];
		frustum[1][i] = far_plane[i];
		frustum[2][i] = left_plane[i];
		frustum[3][i] = bottom_plane[i];
		frustum[4][i] = right_plane[i];
		frustum[5][i] = top_plane[i];
	}
	*/
}

bool AABBInHalfSpace(const AABB & box, const float * plane)
{
	/*
		Main Idea: 
            -> Separating-axis test with only normal of the plane.
	*/

	float radius = box.GetExtent(0) * abs(plane[0])
				+ box.GetExtent(1) * abs(plane[1])
				+ box.GetExtent(2) * abs(plane[2]);
	float signed_distance = box.GetCenter()[0] * plane[0]
							+ box.GetCenter()[1] * plane[1]
							+ box.GetCenter()[2] * plane[2] 
							+ plane[3];
	return signed_distance + radius >= 0; //Only test if box is in positive side of plane.
}

/*
bool SphereInHalfSpace(const BoundingSphere & sphere, const Plane & plane)
{
	float signed_distance = plane.GetNormal().dot(sphere.GetCenter()) + plane.GetD();
	return signed_distance + sphere.GetRadius() >= 0; //Only test if sphere is in positive side of plane.
}
*/

bool Culler::IsVisible(const BoundingVolume * bound) const
{
	if (!bound) return false;
	const AABB * box = static_cast<const AABB *>(bound);

	if (!AABBInHalfSpace(*box, frustum[0])) return false;
	if (!AABBInHalfSpace(*box, frustum[1])) return false;
	if (!AABBInHalfSpace(*box, frustum[2])) return false;
	if (!AABBInHalfSpace(*box, frustum[3])) return false;
	if (!AABBInHalfSpace(*box, frustum[4])) return false;
	if (!AABBInHalfSpace(*box, frustum[5])) return false;

	return true;

}