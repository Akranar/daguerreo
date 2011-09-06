#ifndef BASIC_PRIMITIVE_TESTS_H_
#define BASIC_PRIMITIVE_TESTS_H_

#include "ExLib_Eigen.h"


class BoundingVolume;
class BoundingSphere;
class AABB;
class OBB;

class Cone;
class Cylinder;
class Line;
class LineSegment;
class Plane;
class Polygon;
class Polyhedron;
class Ray;
class Rectangle3D;
class Tetrahedron;
class Triangle;
class Quadrilateral;


class BasicPrimitiveTests
{
public:
	static bool TestBounding(BoundingVolume * a, BoundingVolume * b);
	//Homogenous Tests
	static bool TestingAABBAgainstAABB(const AABB & aabb_a, const AABB & aabb_b);
	static bool TestingOBBAgainstOBB(const OBB & obb_a, const OBB & obb_b);
	static bool TestingSphereAgainstSphere(const BoundingSphere & sphere_a, const BoundingSphere & sphere_b);
	//Heterogenous Tests
	static bool TestingAABBAgainstOBB(const AABB & aabb, const OBB & obb);
	 //Closest-point Computations
	static void ClosestPointOnPlaneToPoint(const Plane & plane, const Eigen::Vector3f & point, float & rtn_t, Eigen::Vector3f & rtn_point);
		static void DistanceOfPointOnPlaneToPoint(const Plane & plane, const Eigen::Vector3f & point, float & rtn_t);
	static void ClosestPointOnLineSegmentToPoint(const LineSegment & segment, const Eigen::Vector3f & point, float & rtn_t, Eigen::Vector3f & rtn_point);
		static float SqDistanceOfPointToSegment(const LineSegment & segment, const Eigen::Vector3f & point);
	static void ClosestPointOnAABBToPoint(const AABB & aabb, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point);
	static float SqDistanceOfPointToAABB(const Eigen::Vector3f & point, const AABB & aabb);
	static void ClosestPointOnOBBToPoint(const OBB & obb, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point);
        static float SqDistanceOfPointToOBB(const Eigen::Vector3f & point, const OBB & obb);
        static void ClosestPointOn3DRectangleToPoint(const Rectangle3D & rect, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point);
    static void ClosestPointOnTriangleToPointUnoptimized(const Triangle & triangle, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point);
	static void ClosestPointOnTriangleToPoint(const Triangle & triangle, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point);
    static void ClosestPointOnTetrahedronToPoint(const Tetrahedron & tetrahedron, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point);
    static void ClosestPointOfTwoLines(const Line & line_a, const Line & line_b, float & rtn_s, float & rtn_t);
    static void ClosestPointOfTwoLineSegments(const LineSegment & segment_a, const LineSegment & segment_b, float & s, float & t);

        static void Segment2DIntersection();
    static void ClosestPointsOfALineSegmentAndATriangle(const LineSegment & segment, const Triangle & triangle, Eigen::Vector3f & rtn_point_a, Eigen::Vector3f & rtn_point_b);
    static void ClosestPointsOfTwoTriangles(const Triangle & triangle_a, const Triangle & triangle_b, Eigen::Vector3f & rtn_point_a, Eigen::Vector3f & rtn_point_b);

	static void ClosestPointOnConvexPolyhedronToPoint(const Polyhedron & polyhedron, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point);
    //Testing Primitives
	static bool TestingSphereAgainstPlane(const BoundingSphere & sphere, const Plane & plane);
	static bool TestingAABBAgainstPlane(const AABB & box, const Plane & plane);
    static bool TestingOBBAgainstPlane(const OBB & box, const Plane & plane);
    static bool TestingConeAgainstPlane(const Cone & cone, const Plane & plane);
    static bool TestingSphereAgainstAABB(const BoundingSphere & sphere, const AABB & aabb);
    static bool TestingSphereAgainstOBB(const BoundingSphere & sphere, const OBB & obb);
    static bool TestingSphereAgainstTriangle(const BoundingSphere & sphere, const Triangle & triangle);
    static bool TestingSphereAgainstPolygon(const BoundingSphere & sphere, const Polygon & polygon);
    static bool TestingAABBAgainstTriangle(const AABB & aabb, const Triangle & triangle);
    static bool TestingTriangleAgainstTriangle(const Triangle & triangle_a, const Triangle & triangle_b);
    //Intersecting Lines, Rays, and (Directed) Segments
    static bool IntersectingSegmentAgainstPlane(const LineSegment & segment, const Plane & plane, float & rtn_t);
    static bool IntersectingRayAgainstSphere(const Ray & ray, const BoundingSphere & sphere, float & rtn_t);
	static bool IntersectingLineAgainstTriangle(const Line & line, const Triangle & triangle, Eigen::Vector3f & rtn_point);
    static bool IntersectingLineAgainstQuadrilateral(const Line & line, const Quadrilateral & quadrilateral, Eigen::Vector3f & rtn_point);

    static bool IntersectingRayAgainstAABB(const Ray & ray, const AABB & aabb, float & rtn_t);
	static bool IntersectingRayAgainstOBB(const Ray & ray, const OBB & obb, float & rtn_t);
	static int IntersectingSegmentAgainstSphere(const LineSegment & segment, const BoundingSphere & sphere, float & rtn_t1, float & rtn_t2);
	static int IntersectingSegmentAgainstAABB(const LineSegment & segment, const AABB & aabb, float & rtn_t1, float & rtn_t2);
    static int IntersectingSegmentAgainstOBB(const LineSegment & segment, const OBB & obb, float & rtn_t1, float & rtn_t2);

	static bool IntersectingRayAgainstAABB(const Ray & ray, const AABB & aabb);
	static bool IntersectingRayAgainstOBB(const Ray & ray, const OBB & obb);
	static int IntersectingSegmentAgainstAABB(const LineSegment & segment, const AABB & aabb);
    static int IntersectingSegmentAgainstOBB(const LineSegment & segment, const OBB & obb);

    static bool IntersectingRayAgainstTriangle(const Ray & ray, const Triangle & triangle, Eigen::Vector3f & rtn_barycentric_coords);
    static bool IntersectingRayAgainstCylinder(const Ray & ray, const Cylinder & cylinder, float & rtn_t);
    static bool IntersectingRayAgainstConvexPolyhedron(const Ray & ray, const Polyhedron & polyhedron, float & rtn_t);
	static bool IntersectingSegmentAgainstTriangle(const LineSegment & segment, const Triangle & triangle, Eigen::Vector3f & rtn_barycentric_coords);
    static int IntersectingSegmentAgainstCylinder(const LineSegment & segment, const Cylinder & cylinder, float & rtn_t1, float & rtn_t2);
    static int IntersectingSegmentAgainstConvexPolyhedron(const LineSegment & segment, const Polyhedron & polyhedron, float & rtn_t1, float & rtn_t2);
    //Additional Tests
	static bool TestingPointInPolygon(const Eigen::Vector3f & point, const Polygon & polygon);
	static bool TestingPointInTriangle(const Eigen::Vector3f & point, const Triangle & triangle);
	static bool TestingPointInPolyhedron(const Eigen::Vector3f & point, const Polyhedron & polyhedron);
    static bool IntersectionOfTwoPlanes(const Plane & plane_a, const Plane & plane_b);
    static bool IntersectionOfThreePlanes(const Plane & plane_a, const Plane & plane_b, const Plane & plane_c);

	static bool IntersectLineAgainstLine(const Line & line_a, const Line & line_b, float & rtn_s, float & rtn_t);
	static bool IntersectLineAgainstSegment(const Line & line_a, const LineSegment & segment, float & rtn_s, float & rtn_t);
    //Dynamic Intersection Tests
    static bool IntersectingMovingSphereAgainstPlane(const BoundingSphere & sphere, const Plane & plane);
    static bool IntersectingMovingAABBAgainstPlane(const AABB & aabb, const Plane & plane);
    static bool IntersectingMovingSphereAgainstSphere(const BoundingSphere & sphere_a, const BoundingSphere & sphere_b);
    static bool IntersectingMovingSphereAgainstTriangle(const BoundingSphere & sphere, const Triangle & triangle);
    static bool IntersectingMovingSphereAgainstAABB(const BoundingSphere & sphere, const AABB & aabb);
    static bool IntersectingMovingAABBAgainstAABB(const AABB & aabb_a, const AABB & aabb_b);
private:
	static float Clamp(float to_clamp, float min, float max);
	static void Swap(float & x, float & y);
	static float ScalarTripleProduct(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c);
	static bool Between0And1(float arg);
};

#endif