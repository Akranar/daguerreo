#include "BasicPrimitiveTests.h"
#include "ExLib_BoundingVolume.h"
#include "GeometricPrimitives.h"

bool BasicPrimitiveTests::TestingSphereAgainstPlane(const BoundingSphere & sphere, const Plane & plane)
{
	/*
		Main Idea:
            -> Plugs sphere center into plane equation to get signed distance of sphere center from plane.

		Test:
			-> signed distance = Dot(C, N) - D 
	*/

	
	float dist = sphere.GetCenter().dot(plane.GetNormal()) - plane.GetD();
	return abs(dist) <= sphere.GetRadius();
	
}

bool BasicPrimitiveTests::TestingAABBAgainstPlane(const AABB & box, const Plane & plane)
{
	/*
		Main Idea: 
            -> Separating-axis test with only normal of the plane.

        Procedure:
            -> Have the axis be a line going through box center, parallel to plane's normal.
                -> L(t) = C + t * n
                    -> where C = box center.
                    -> where n = plane normal

            -> OBB projects onto axis symmetrically with radius "r".
                -> Test is now to compute "r" and check if distance from OBB center to plane is less than "r".
                    -> Can obtain "r" by projecting OBB vertices onto axis and picking one with maximum distance from C
	*/

	
	Eigen::Vector3f corner0 = box.GetExtents();
	Eigen::Vector3f corner1 = Eigen::Vector3f(box.GetExtent(0), -box.GetExtent(1), box.GetExtent(2));
	Eigen::Vector3f corner2 = Eigen::Vector3f(-box.GetExtent(0), -box.GetExtent(1), box.GetExtent(2));
	Eigen::Vector3f corner3 = Eigen::Vector3f(-box.GetExtent(0), box.GetExtent(1), box.GetExtent(2));

	float box_radius_along_axis = std::max(plane.GetNormal().dot(corner0), plane.GetNormal().dot(corner1));
	box_radius_along_axis = std::max(box_radius_along_axis, plane.GetNormal().dot(corner2));
	box_radius_along_axis = std::max(box_radius_along_axis, plane.GetNormal().dot(corner3));
	
	float dist_from_center = box.GetCenter().dot(plane.GetNormal()) - plane.GetD();
	return abs(dist_from_center) <= box_radius_along_axis;
	
}

bool BasicPrimitiveTests::TestingOBBAgainstPlane(const OBB & box, const Plane & plane)
{
	/*
		Main Idea: 
            -> Separating-axis test with only normal of the plane.

        Procedure:
            -> Have the axis be a line going through box center, parallel to plane's normal.
                -> L(t) = C + t * n
                    -> where C = box center.
                    -> where n = plane norma

            -> OBB projects onto axis symmetrically with radius "r".
                -> Test is now to compute "r" and check if distance from OBB center to plane is less than "r".
                    -> Can obtain "r" by projecting OBB vertices onto axis and picking one with maximum distance from C
	*/

	
	Eigen::Vector3f corner0 = box.GetExtent(0) * box.GetAxis(0)
								+ box.GetExtent(1) * box.GetAxis(1)
								+ box.GetExtent(2) * box.GetAxis(2);
	Eigen::Vector3f corner1 = box.GetExtent(0) * box.GetAxis(0)
								- box.GetExtent(1) * box.GetAxis(1)
								+ box.GetExtent(2) * box.GetAxis(2);
	Eigen::Vector3f corner2 = -box.GetExtent(0) * box.GetAxis(0)
								+ box.GetExtent(1) * box.GetAxis(1)
								+ box.GetExtent(2) * box.GetAxis(2);
	Eigen::Vector3f corner3 = -box.GetExtent(0) * box.GetAxis(0)
								- box.GetExtent(1) * box.GetAxis(1)
								+ box.GetExtent(2) * box.GetAxis(2);

	float box_radius_along_axis = std::max(
		plane.GetNormal().dot(corner0), 
		plane.GetNormal().dot(corner1));
	box_radius_along_axis = std::max(box_radius_along_axis, plane.GetNormal().dot(corner2));
	box_radius_along_axis = std::max(box_radius_along_axis, plane.GetNormal().dot(corner3));
	
	float dist_from_center = box.GetCenter().dot(plane.GetNormal()) - plane.GetD();
	return abs(dist_from_center) <= box_radius_along_axis;
	
}
bool BasicPrimitiveTests::TestingConeAgainstPlane(const Cone & cone, const Plane & plane)
{
	/*
		Main Idea:
            -> Plugs [...] into plane.
                -> tip of cone and 
                -> point of the circular endcap of the cone farthest in the direction -n (negative of plane's normal)

        Let:
            -> Plane:
                -> dot(n, X) = d
                    -> where d = -dot(P, n)
                    -> where n is unit.

            -> Cone:
                -> point at tip: 
                    -> T
                -> normalized axis direction:
                    -> v
                -> bottom radius:
                    -> r

        The cone:
            -> tan(alpha) = r/h
                -> where alpha = halfangle at cone apex.
            -> r = h * tan(alpha)

        Procedure for testing with negative halfspace of plane:
            -> Test two points:
                -> Tip T:
                    -> Intersects negative halfspace of plane if dot(n, T) < d
                -> Point Q on endcap farthest in direction -n:
                    -> Q = T + h * v + r * m
                        -> where m = cross(cross(n, v), v)
                    -> Intersects negative halfspace of plane if dot(n, Q) < d

        Procedure for intersecting with plane itself:
            -> Test if T and Q are on different sides of plane.
                -> If T is behind plane,
                    -> Calculate Q as point farthest in direction n.
                        -> Q = T + h * v + r * m
                            -> where m = cross(cross(-n, v), v)
	*/
	return false;
}
bool BasicPrimitiveTests::TestingSphereAgainstAABB(const BoundingSphere & sphere, const AABB & aabb)
{
	/*
		Main Idea:
            -> Compares distance between closest point in AABB to sphere center with sphere radius.
                -> Using Section BasicPrimitiveTests::SqDistanceOfPointToAABB()

        Test:
            -> P = closest point to sphere center (BasicPrimitiveTests::SqDistanceOfPointToAABB())
            -> C = sphere center
            -> return dot(P-C, P-C) <= exp(sphere_radius, 2)
	*/

	float sq_dist = SqDistanceOfPointToAABB(sphere.GetCenter(), aabb);
	return sq_dist <= sphere.GetRadius() * sphere.GetRadius();
}
bool BasicPrimitiveTests::TestingSphereAgainstOBB(const BoundingSphere & sphere, const OBB & obb)
{
	/*
		Main Idea:
            -> Compares distance between closest point in OBB to sphere center with sphere radius.
                -> Using BasicPrimitiveTests::SqDistanceOfPointToOBB()

        Test:
            -> P = closest point to sphere center (BasicPrimitiveTests::SqDistanceOfPointToOBB())
            -> C = sphere center
            -> return dot(P-C, P-C) <= exp(sphere_radius, 2)
	*/
	
	float sq_dist = SqDistanceOfPointToOBB(sphere.GetCenter(), obb);
	return sq_dist <= sphere.GetRadius() * sphere.GetRadius();
	
}
bool BasicPrimitiveTests::TestingSphereAgainstTriangle(const BoundingSphere & sphere, const Triangle & triangle)
{
	/*
		Main Idea:
            - Find point P on triangle closest to the sphere center. BasicPrimitiveTests::ClosestPointOnTriangleToPoint()
                - Compare distance between P and sphere center to sphere radius.

        Test:
            -> P = closest point to sphere center BasicPrimitiveTests::ClosestPointOnTriangleToPoint()
            -> C = sphere center
            -> return dot(P-C, P-C) <= exp(sphere_radius, 2)
	*/
	
	Eigen::Vector3f closest_point_on_triangle;
	ClosestPointOnTriangleToPoint(triangle, sphere.GetCenter(), closest_point_on_triangle);
	return (closest_point_on_triangle - sphere.GetCenter()).dot(closest_point_on_triangle - sphere.GetCenter()) <= sphere.GetRadius() * sphere.GetRadius();
	
}
bool BasicPrimitiveTests::TestingSphereAgainstPolygon(const BoundingSphere & sphere, const Polygon & polygon)
{
	/*
		For small polygons
            -> Triangulate.  Then use Section 5.2.7

        For large polygon
            -> Test if sphere intersects plane of polygon. Exit with false if not.
            -> Test each edge of polygon to see if it pierces the sphere. Exit with true if so.
            -> Project sphere center to plane of polygon.
                -> Perform point-in-polygon test (Section 5.4.1).
	*/
	return false;
}
bool BasicPrimitiveTests::TestingAABBAgainstTriangle(const AABB & aabb, const Triangle & triangle)
{
	/*
		-> Separating-axis test with 13 axis
                -> Three face normals from AABB
                -> One face normal from triangle
                -> Nine axes from cross products of edges from both.
	*/
	return false;
}
bool BasicPrimitiveTests::TestingTriangleAgainstTriangle(const Triangle & triangle_a, const Triangle & triangle_b)
{
	/*
		Method 1:
            -> Six edge-triangle tests
            -> Robustness consideration:
                -> Does not handle when triangles coplanar, or any triangles are degenerate.

        Method 2:
            -> Separating-axis with 11 axes.
                -> 2 face normals.
                -> 9 combination of edge cross-products.

        Method 3 - Interval Overlap Method:
            -> Step 1: Tests the two face normals for separating axes.
                -> Test if all vertices from one triangle are on one side of the triangle plane.
            -> Step 2: Note that the triangle planes must be intersecting at a line.
                -> L(t) = P + t * d
                    -> where d = cross(n1, n2)
                        -> where ni = normals of the triangles
                -> Compute scalar intersection intervals between each triangle and L
                    -> Triangles intersect if their scalar intersection intervals with L overlap.
            -> As optimization, can compute intervals on the principal coordinate axis most parallel with L.
                -> By first projecting onto this axis.

        Method 4 - Penetration Method:
            -> Step 1: Test if all vertices of triangle 2 are all on the same side of triangle 1's plane.
                -> Exit with no intersection if so.
            -> Step 2: Compute intersection line segment between triangle 2 and triangle 1's plane.  (HOW?)
            -> Step 3: Test if line segment intersects or is contained in triangle 2.
                -> Can optimize by projecting line segment and triangle 2 to principal plane most parallel to triangle 2 before solving for intersection.
	*/
	return false;
}