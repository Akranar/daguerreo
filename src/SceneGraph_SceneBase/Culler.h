#ifndef _CULLER_H_
#define _CULLER_H_

class Projector;
class Spatial;
class SceneNode;
class VisibleSet;
class BoundingVolume;

class Culler
{
protected:
	VisibleSet * visible_set;
	Projector * camera;
	float frustum[6][4]; //Plane equations: [Nx, Ny, Nz, D]
						//	-> where Plane: dot(N, x) + D = 0
public:
	Culler();
	virtual ~Culler();

	inline void SetCamera(Projector * in_camera);
	inline const Projector * GetCamera() const;
	inline VisibleSet * GetVisibleSet() const;

	void SetFrustum();
	bool IsVisible(const BoundingVolume * bound) const;

};

inline void Culler::SetCamera(Projector * in_camera)
{
	camera = in_camera;
}
inline const Projector * Culler::GetCamera() const
{
	return camera;
}
inline VisibleSet * Culler::GetVisibleSet() const
{
	return visible_set;
}

#endif