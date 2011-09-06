#include "BasicPrimitiveTests.h"
#include "GeometricPrimitives.h"
#include "ExLib_BoundingVolume.h"
#include <math.h>
#include <algorithm>

#define EPSILON 1e-6

void BasicPrimitiveTests::ClosestPointOnPlaneToPoint(const Plane & plane, const Eigen::Vector3f & point, float & rtn_t, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
            -> Project point to plane orthogonally

        Let:
            -> Plane: dot(n, (X-P)) = 0
            -> Q = arbitrary point in space.
            -> R = closest point on plane to Q.
                -> R = Q - t * n

        Derivation:
            -> dot(n, (Q - t * n) - P) = 0                              [Substitute R in Plane]
            -> t = (    dot(n, Q - P)    /    dot(n, n)    )            [Solve for t]
            -> R = Q - (    dot(n, Q-P)    /    dot(n, n)    ) * n      [Plug t into R]
			-> R = Q - (    dot(n, Q) + D    /    dot(n, n)    ) * n      [D = -dot(n, P)]	

		Simplification if plane normal is known to be unit length:
                -> t = (     dot(n, Q) + D    );
	*/
	float t = (    (plane.GetNormal().dot(point) + plane.GetD())     );
	rtn_t = t;
	rtn_point = point - t * plane.GetNormal();
}

void BasicPrimitiveTests::DistanceOfPointOnPlaneToPoint(const Plane & plane, const Eigen::Vector3f & point, float & rtn_t)
{
	rtn_t = plane.GetNormal().dot(point) + plane.GetD();
}

void BasicPrimitiveTests::ClosestPointOnLineSegmentToPoint(const LineSegment & segment, const Eigen::Vector3f & point, float & rtn_t, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
                -> Project orthogonally point to line
                -> Deal with end point intervals of line

            Let:
                -> Line Segment:
                    -> P(t) = A + t * (B - A)
                        -> where 0 <= t =< 1
                        -> where A and B are segment's endpoints.
                -> Target Point:
                    -> C

            Procedure:
                -> Find time "t" corresponding to projection on segment:
                    -> t = (    dot(C-A, n)    /    length(B-A)    )
                        -> where n = unit vector in direction AB.
                -> Clamp "t" between 0 and 1.
                -> Plug "t" into P(t) to find point.
	*/

	Eigen::Vector3f AB_unit = segment.GetPointB() - segment.GetPointA();
	float length_AB = AB_unit.norm();
	AB_unit.normalize();

	float t = (     (point - segment.GetPointA()).dot(AB_unit)     /     length_AB     );
	t = std::min<float>(1, t);
	t = std::max<float>(0, t);
	rtn_t = t;

	rtn_point = segment.GetPointA() + t * segment.GetPointB() - segment.GetPointA();
}

float BasicPrimitiveTests::SqDistanceOfPointToSegment(const LineSegment & segment, const Eigen::Vector3f & point)
{
	/*
		Main Idea:
			-> Finds distance of point without finding projected point
				-> Through algebra simplification

		Three cases:
			-> dot(AC, AB) <= 0
				-> A is point closest to C.
					-> squared distance = dot(AC, AC)
	        
			-> dot(AC, AB) >= dot(AB, AB)
				-> B is point closest to C.
					-> squared distance = dot(BC, BC)

			-> 0 <= dot(AC, AB) <= dot(AB, AB)
				-> D is closest point to C
					-> where D = A + t * AB
						-> where t = (    dot(AC, AB)    /    dot(AB, AB)    )
					-> squared distance = dot(CD, CD)
						-> = dot(AC, AC) - (    dot(AC, AB) * dot(AC, AB) / dot(AB, AB)    )
	*/


	float ACdotAB = (point-segment.GetPointA()).dot(segment.GetPointB()-segment.GetPointA());
	
	if (ACdotAB <= 0)
	{
		return (point - segment.GetPointA()).squaredNorm();
	}
	float ABdotAB = (segment.GetPointB()-segment.GetPointA()).squaredNorm();
	if (ACdotAB >= ABdotAB)
	{
		return (point - segment.GetPointB()).squaredNorm();
	}

	return (point - segment.GetPointA()).squaredNorm() - (ACdotAB * ACdotAB / ABdotAB);
}

void BasicPrimitiveTests::ClosestPointOnAABBToPoint(const AABB & aabb, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
			-> Clamps point to AABB boundaries.
	*/

	const Eigen::Vector3f & center = aabb.GetCenter();
	float extent[3];
	extent[0] = aabb.GetExtent(0);
	extent[1] = aabb.GetExtent(1);
	extent[2] = aabb.GetExtent(2);

	rtn_point[0] = Clamp(point[0], center[0] - extent[0], center[0] + extent[0]);
	rtn_point[1] = Clamp(point[1], center[1] - extent[1], center[1] + extent[1]);
	rtn_point[2] = Clamp(point[2], center[2] - extent[2], center[2] + extent[2]);
}

float BasicPrimitiveTests::SqDistanceOfPointToAABB(const Eigen::Vector3f & point, const AABB & aabb)
{
	/*
		Main Idea:
            -> Finds distance of point without finding closest point.
                -> For each axis, count any excess squared distance outside box extents.
					-> A^2 + B^2 = C^2
	*/

	float rtn_squared_distance = 0;
	for (unsigned int i = 0; i < 3; ++i)
	{
		if (point[i] < aabb.GetCenter()[i] - aabb.GetExtent(i)) 
			rtn_squared_distance += (point[i] - (aabb.GetCenter()[i] - aabb.GetExtent(i))) * (point[i] - (aabb.GetCenter()[i] - aabb.GetExtent(i)));
		if (point[i] > aabb.GetCenter()[i] + aabb.GetExtent(i)) 
			rtn_squared_distance += ((aabb.GetCenter()[i] + aabb.GetExtent(i)) - point[i]) * ((aabb.GetCenter()[i] + aabb.GetExtent(i)) - point[i]);
	}
	return rtn_squared_distance;
}

void BasicPrimitiveTests::ClosestPointOnOBBToPoint(const OBB & obb, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
			- Uses basis representation of OBB
				- Finds point in terms of OBB basis coefficients, then clamps values.

		Let:
			-> B = OBB given by:
				-> Center point:
					-> C
				-> Orthogonal unit orientation vectors:
					-> u0, u1, u2
				-> Scalar box halfwidth extents for each axis:
					-> e0, e1, e2

		Procedure:
			-> Point P in terms of OBB:
				-> P = C + x * u0 + y * u1 + z * u2

				-> Example Solving for X:
					-> P - C = x * u0 + y * u1 + z * u2
					-> dot(P - C, u0) = dot(u0, x * u0 + y * u1 + z * u2)                       [dot product both sides with u0]
					-> dot(P - C, u0) = x * dot(u0, u0) + y * dot(u1, u0) + z * dot(u2, u0)     [expand dot product]
					-> x = dot(P-C, u0)                                                         [Simplfy using dot(ui, ui) = 1, dot(ui, uj) = 0]

				-> Obtain:
					-> x = dot(P-C, u0)
					-> y = dot(P-C, u1)
					-> y = dot(P-C, u2)

			-> Clamp P in terms of OBB to OBB boundaries.
	*/

	Eigen::Vector3f coords;
	Eigen::Vector3f p_minus_center = point - obb.GetCenter();

	coords[0] = Clamp(p_minus_center.dot(obb.GetAxis(0)), -obb.GetExtent(0), obb.GetExtent(0));
	coords[1] = Clamp(p_minus_center.dot(obb.GetAxis(1)), -obb.GetExtent(1), obb.GetExtent(1));
	coords[2] = Clamp(p_minus_center.dot(obb.GetAxis(2)), -obb.GetExtent(2), obb.GetExtent(2));

	rtn_point = obb.GetCenter() + coords[0] * obb.GetAxis(0) + coords[1] * obb.GetAxis(1) + coords[2] * obb.GetAxis(2); 
}

float BasicPrimitiveTests::SqDistanceOfPointToOBB(const Eigen::Vector3f & point, const OBB & obb)
{
	/*
		 Main Idea:
            -> Finds squared distance of point without finding closest point.
                -> Projects the displacement vector of the point from the OBB center onto each of OBB's axes.
                    -> Accumulates the excess squared length that each projected vector has beyond the extent of the OBB
	*/
	Eigen::Vector3f coords;
	Eigen::Vector3f p_minus_center = point - obb.GetCenter();

	coords[0] = p_minus_center.dot(obb.GetAxis(0));
	coords[1] = p_minus_center.dot(obb.GetAxis(1));
	coords[2] = p_minus_center.dot(obb.GetAxis(2));

	float rtn_squared_distance = 0;
	for (unsigned int i = 0; i < 3; ++i)
	{
		if (coords[i] < -obb.GetExtent(i)) 
			rtn_squared_distance += (-obb.GetExtent(i) - coords[i]);
		if (coords[i] > obb.GetExtent(i)) 
			rtn_squared_distance += (coords[i] - obb.GetExtent(i));
	}
	return rtn_squared_distance;
}

void BasicPrimitiveTests::ClosestPointOn3DRectangleToPoint(const Rectangle3D & rect, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
            -> Uses the fact that Rectangles can be seen as OBB but with zero extent on one of its axes.
	*/

	Eigen::Vector2f coords;
	Eigen::Vector3f p_minus_center = point - rect.GetCenter();

	coords[0] = Clamp(p_minus_center.dot(rect.GetAxisA()), -rect.GetExtentA(), rect.GetExtentA());
	coords[1] = Clamp(p_minus_center.dot(rect.GetAxisB()), -rect.GetExtentB(), rect.GetExtentB());

	rtn_point = rect.GetCenter() + coords[0] * rect.GetAxisA() + coords[1] * rect.GetAxisB();
}


void BasicPrimitiveTests::ClosestPointOnTriangleToPointUnoptimized(const Triangle & triangle, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
                -> Computes which voronoi region the point is in using barycentric coordinates
                    -> Then returns the orthogonal projection onto that voronoi feature.

        Determining Voronoi Region:
            -> Barycentric coordinates of orthogonal projection R of point P onto triangle ABC
                    -> where R = P - t * n
                        -> where n = triangle plane's normal.
                -> Given by ratios of anything proportional to the signed areas of triangles RAB, ARC, ABR to signed area of ABC (Section 3.4)
                        -> u = RAB / ABC
                        -> v = ARC / ABC
                        -> w = ABR / ABC
                    -> Use cross products.

                -> However:
                    -> dot(n, cross(A-R, B-R)) = dot(n, cross(A-P, B-P))
                        -> Means we can get barycentric coordinates of R directly from P

        Procedure:
            -> Check if P in vertex Voronoi regions first by comparing "t" values of P projected onto triangle sides.
                -> P in vertex Voronoi region of A when "t" value of P projected onto edges AB and AC are both <= 0.
                -> Try to early out.

            -> Check if P in edge Voronoi regions
                -> Within feature region of AB when
                    -> P outside AB
                        -> triple scalar product [N PA PB] <= 0
                    -> dot(AP, AB) and dot(BP, BA) >= 0

            -> If not found Voronoi region yet, must be within triangle.
                -> Return barycentric coordinates.

        Optimizations:
            -> When you have something like:
                -> dot(cross(a, b), cross(c, d))
            -> Can use Lagrange Identity to get rid of cross products by doing more dot products:
                -> dot(cross(a, b), cross(c, d)) = dot(a, c) * dot(b, d) - dot(a, d) * dot(b, c)
	*/

	/*
		Orthogonal project of P onto edge AB:
			-> r_ab = A + T * (B - A)
				-> where T = (t_ab)    /     (t_ab + t_ba)
	*/
	Eigen::Vector3f edge_ab = triangle.GetVertex(1) - triangle.GetVertex(0);
	Eigen::Vector3f edge_ac = triangle.GetVertex(2) - triangle.GetVertex(0);
	Eigen::Vector3f edge_bc = triangle.GetVertex(2) - triangle.GetVertex(1);

	float t_ab = (point - triangle.GetVertex(0)).dot(edge_ab);
	float t_ba = (point - triangle.GetVertex(1)).dot(triangle.GetVertex(0) - triangle.GetVertex(1));

	float t_ac = (point - triangle.GetVertex(0)).dot(edge_ac);
	float t_ca = (point - triangle.GetVertex(2)).dot(triangle.GetVertex(0) - triangle.GetVertex(2));

	if (t_ab <= 0 && t_ac <= 0)
	{
		//In Voronoi region of Vertex A.
		rtn_point = triangle.GetVertex(0);
		return;
	}

	float t_bc = (point - triangle.GetVertex(1)).dot(edge_bc);
	float t_cb = (point - triangle.GetVertex(2)).dot(triangle.GetVertex(1) - triangle.GetVertex(2));

	

	if (t_ba <= 0 && t_bc <= 0)
	{
		//In Voronoi region of Vertex B.
		rtn_point = triangle.GetVertex(1);
		return;
	}
	if (t_ca <= 0 && t_cb <= 0)
	{
		//In Voronoi region of Vertex C.
		rtn_point = triangle.GetVertex(2);
		return;
	}


	Eigen::Vector3f triangle_normal = (triangle.GetVertex(0) - triangle.GetVertex(1)).cross(triangle.GetVertex(0) - triangle.GetVertex(2));

	//TSP = Triple Scalar Product
	float TSP_N_PA_PB = triangle_normal.dot(    (triangle.GetVertex(0) - point).cross(triangle.GetVertex(1) - point)    );
	if (TSP_N_PA_PB <= 0 && t_ab >= 0 && t_ba >= 0)
	{
		rtn_point = triangle.GetVertex(0) + (t_ab / t_ab + t_ba) * edge_ab;
		return;
	}

	float TSP_N_PB_PC = triangle_normal.dot(    (triangle.GetVertex(1) - point).cross(triangle.GetVertex(2) - point)    );
	if (TSP_N_PB_PC <= 0 && t_bc >= 0 && t_cb >= 0)
	{
		rtn_point = triangle.GetVertex(0) + (t_bc / t_bc + t_bc) * edge_bc;
		return;
	}

	float TSP_N_PC_PA = triangle_normal.dot(    (triangle.GetVertex(2) - point).cross(triangle.GetVertex(0) - point)    );
	if (TSP_N_PC_PA <= 0 && t_ac >= 0 && t_ca >= 0)
	{
		rtn_point = triangle.GetVertex(0) + (t_ac / t_ac + t_ca) * edge_ac;
		return;
	}

	float a = TSP_N_PB_PC / (TSP_N_PA_PB + TSP_N_PB_PC + TSP_N_PC_PA);
	float b = TSP_N_PC_PA / (TSP_N_PA_PB + TSP_N_PB_PC + TSP_N_PC_PA);
	float c = 1.0f - a - b; // = TSP_N_PA_PB / (TSP_N_PA_PB + TSP_N_PB_PC + TSP_N_PC_PA);

	rtn_point = a * triangle.GetVertex(0) + b * triangle.GetVertex(1) + c * triangle.GetVertex(2);
	return;
	
}

void BasicPrimitiveTests::ClosestPointOnTriangleToPoint(const Triangle & triangle, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point)
{
	Eigen::Vector3f edge_ab = triangle.GetVertex(1) - triangle.GetVertex(0);
	Eigen::Vector3f edge_ac = triangle.GetVertex(2) - triangle.GetVertex(0);

	Eigen::Vector3f ap = point - triangle.GetVertex(0);
	float d1 = edge_ab.dot(ap);
	float d2 = edge_ac.dot(ap);
	if (d1 <= 0 && d2 <= 0)
	{
		rtn_point = triangle.GetVertex(0);
		return;
	}

	Eigen::Vector3f bp = point - triangle.GetVertex(1);
	float d3 = edge_ab.dot(bp);
	float d4 = edge_ac.dot(bp);
	if (d3 >= 0 && d4 <= d3)
	{
		rtn_point = triangle.GetVertex(1);
		return;
	}

	float vc = d1 * d4 - d3 * d2;
	if (vc <= 0 && d1 >= 0 && d3 <= 0)
	{
		rtn_point = triangle.GetVertex(0) + (d1 / (d1 - d3)) * edge_ab;
		return;
	}

	Eigen::Vector3f cp = point - triangle.GetVertex(2);
	float d5 = edge_ab.dot(cp);
	float d6 = edge_ac.dot(cp);
	if (d6 >= 0 && d5 <= d6)
	{
		rtn_point = triangle.GetVertex(2);
		return;
	}

	float vb = d5 * d2 - d1 * d6;
	if (vb <= 0 && d2 >= 0 && d6 <= 0)
	{
		rtn_point = triangle.GetVertex(0) + (d2 / (d2 - d6)) * edge_ac;
		return;
	}

	float va = d3 * d6 - d5 * d4;
	if (va <= 0 && (d4 - d3) >= 0 && (d5 - d6) >= 0)
	{
		rtn_point = triangle.GetVertex(1) + ((d4 - d3) / ((d4 - d3) - (d5 - d6))) * edge_ac;
		return;
	}

	float denom = 1.0f / (va + vb + vc);
	float v = vb * denom;
	float w = vc * denom;
	rtn_point = triangle.GetVertex(0) + edge_ab * v + edge_ac * w;
	return;
}

void BasicPrimitiveTests::ClosestPointOnTetrahedronToPoint(const Tetrahedron & tetrahedron, const Eigen::Vector3f & point, Eigen::Vector3f & rtn_point)
{
	/*
		Main Idea:
            -> Computes which voronoi region the point is in, then projects the point onto the voronoi region's feature.

        Example:
                -> P in vertex A Voronoi Region if:
                    -> dot(P-A, B-A) <= 0
                    -> dot(P-A, C-A) <= 0
                    -> dot(P-A, D-A) <= 0

                -> P in edge AB Voronoi Region if:
                    Between the two planes at edge points A and B with normals + or - BA
                        -> dot(P-A, B-A) >= 0
                        -> dot(P-B, A-B) >= 0
                    Outside of Edge AB with respect to triangle ABC:
                        -> dot(cross(P-B, P-A), normal_ABC) >= 0
                            -> where normal_ABC = cross(B-A, C-A)
                    Outside of Edge AB with respect to triangle ADB:
                        -> dot(cross(P-A, P-B), normal_ADB) >= 0
                            -> where normal_ADB = cross(D-A, B-A)

                -> P in Face ABC Voronoi Region if:
                    -> P inside all edges of triangle ABC:
                        -> dot(cross(P-B, P-A), normal_ABC) <= 0
                        -> dot(cross(P-A, P-C), normal_ABC) <= 0
                        -> dot(cross(P-C, P-B), normal_ABC) <= 0

		Procedure:
			Use Lagrange Identity to Reduce # of Cross-Products:
				Dot(Cross(A, B), Cross(C, D) => Dot(A, C) * Dot(B, D) - Dot(A, D) * Dot(B, C)

			Construct Calculations from These Dot Products:
				-> d1 = Dot(P-A, B-A)
				-> d2 = Dot(P-A, C-A) 
				-> d3 = Dot(P-A, D-A) 

				-> d4 = Dot(P-B, B-A)
				-> d5 = Dot(P-B, C-A) 
				-> d6 = Dot(P-B, D-A) 

				-> d7 = Dot(P-C, B-A)
				-> d8 = Dot(P-C, C-A) 
				-> d9 = Dot(P-C, D-A) 

				-> d10 = Dot(P-D, B-A)
				-> d11 = Dot(P-D, C-A) 
				-> d12 = Dot(P-D, D-A) 

			Distributive Property Of Dot Products:
				-> Use to make other dot products:
					-> Dot(X, Z) - Dot(Y, Z) = Dot(X-Y, Z)
					-> Ex) Dot(P-A, C-A) - Dot(P-A, B-A) = Dot(P-A, C-B)

			Check Voronoi Regions:
				-> Vertex A
					-> d1, d2, d3 <= 0
				-> Vertex B
					-> -d4, d5-d4, d6-d4 <= 0
				-> Vertex C
					-> -d8, d7-d8, d9-d8 <= 0
				-> Vertex D
					-> -d12, d10-d12, d11-d12 <= 0
				-> Edge AB
					-> d1 >= 0
					-> d4 <= 0
					From Triangle ABC:
						-> d4 * d2 - d5 * d1 >= 0
					From Triangle ADB:
						-> d3 * d4 - d1 * d6 >= 0
				-> Edge AC
					-> d2 >= 0
					-> d8 <= 0
					From Triangle ABC:
						-> d1 * d8 - d2 * d7 >= 0
					From Triangle ACD:
						-> d8 * d3 - d9 * d2 >= 0
				-> Edge AD
					-> d3 >= 0
					-> d12 <= 0
					From Triangle ACD:
						-> d2 * d12 - d3 * d11 >= 0
					From Triangle ADB:
						-> d12 * d1 - d10 * d3 >= 0
				-> Edge BC
					-> d5 - d4 >= 0
					-> d8 - d7 <= 0
					From Triangle ABC:
						-> d7 * d5 - d8 * d4 >= 0
					From Triangle BDC:
						-> (d6-d4)*(d8-d7) - (d5-d4)*(d9-d7) >= 0
				-> Edge BD
					-> d6-d4 >= 0
					-> d12-d10 <= 0
					From Triangle ADB:
						-> d6 * d10 - d4 * d12 >= 0
					From Triangle BDC:
						-> (d12-d10)*(d5-d4) - (d11-d10)*(d6-d4) >= 0
				-> Edge CD
					-> d9 - d8 >= 0
					-> d12 - d11 <= 0
					From Triangle BDC:
						-> (d12-d10)*(d5-d4) - (d11-d10)*(d6-d4) >= 0
					From Triangle ACD:
						-> d11 * d9 - d12 * d8 >= 0
				-> Face ABC
					-> AB: d4 * d2 - d5 * d1 <= 0
					-> AC: d1 * d8 - d2 * d7 <= 0
					-> BC: d7 * d5 - d8 * d4 <= 0
				-> Face ADB
					-> AB: d3 * d4 - d1 * d6 <= 0
					-> AD: d12 * d1 - d10 * d3 <= 0
					-> BD: d6 * d10 - d4 * d12 <= 0
				-> Face ACD
					-> AC: d8 * d3 - d9 * d2 <= 0
					-> AD: d2 * d12 - d3 * d11 <= 0
					-> CD: d11 * d9 - d12 * d8 <= 0
				-> Face BDC
					-> BC: (d6-d4)*(d8-d7) - (d5-d4)*(d9-d7) <= 0
					-> BD: (d12-d10)*(d5-d4) - (d11-d10)*(d6-d4) <= 0
					-> CD: (d12-d10)*(d5-d4) - (d11-d10)*(d6-d4) <= 0
	*/
	float d1 = (point-tetrahedron.GetVertex(0)).dot(tetrahedron.GetVertex(1) - tetrahedron.GetVertex(0));
	float d2 = (point-tetrahedron.GetVertex(0)).dot(tetrahedron.GetVertex(2) - tetrahedron.GetVertex(0));
	float d3 = (point-tetrahedron.GetVertex(0)).dot(tetrahedron.GetVertex(3) - tetrahedron.GetVertex(0));

	if (d1 <= 0 && d2 <= 0 && d3 <= 0)
	{
		//Vertex A Region
		rtn_point = tetrahedron.GetVertex(0);
		return;
	}

	float d4 = (point-tetrahedron.GetVertex(1)).dot(tetrahedron.GetVertex(1) - tetrahedron.GetVertex(0));
	float d5 = (point-tetrahedron.GetVertex(1)).dot(tetrahedron.GetVertex(2) - tetrahedron.GetVertex(0));
	float ABC_AB = d4 * d2 - d5 * d1;
	float d6 = (point-tetrahedron.GetVertex(1)).dot(tetrahedron.GetVertex(3) - tetrahedron.GetVertex(0));

	if (d4 >= 0 && d5 <= d4 && d6 <= d4)
	{
		//Vertex B Region
		rtn_point = tetrahedron.GetVertex(1);
		return;
	}

	float ADB_AB = d3 * d4 - d1 * d6;
	if (ABC_AB >= 0 && ADB_AB >= 0 && d1 >= 0 && d4 <= 0)
	{
		//Edge AB Region
		rtn_point = tetrahedron.GetVertex(0) + (d1 / d1 - d4) * tetrahedron.GetVertex(1) - tetrahedron.GetVertex(0);
		return;
	}
	float d7 = (point-tetrahedron.GetVertex(2)).dot(tetrahedron.GetVertex(1) - tetrahedron.GetVertex(0));
	float d8 = (point-tetrahedron.GetVertex(2)).dot(tetrahedron.GetVertex(2) - tetrahedron.GetVertex(0));
	float ABC_AC = d1 * d8 - d2 * d7;
	float ABC_BC = d7 * d5 - d8 * d4;

	if (ABC_AB <= 0 && ABC_BC <= 0 && ABC_AC <= 0)
	{
		//Face ABC Region
		float denom = 1.0f / (ABC_AB + ABC_BC + ABC_AC);
		float c = ABC_AB * denom;
		float b = ABC_AC * denom;
		float a = 1.0f - b - c;
		rtn_point = a * tetrahedron.GetVertex(0) + b * tetrahedron.GetVertex(1) + c * tetrahedron.GetVertex(2);
		return;
	}

	float d9 = (point-tetrahedron.GetVertex(2)).dot(tetrahedron.GetVertex(3) - tetrahedron.GetVertex(0));

	if (d8 >= 0 && d7 <= d8 && d9 <= d8)
	{
		//Vertex C Region
		rtn_point = tetrahedron.GetVertex(2);
		return;
	}

	float BDC_BC = (d6-d4)*(d8-d7) - (d5-d4)*(d9-d7);
	if (BDC_BC >= 0 && ABC_BC >= 0 && d5 >= d4 && d8 <= d7)
	{
		//Edge BC Region
		rtn_point = tetrahedron.GetVertex(1) + ((d5-d4) / (d5-d4) - (d8-d7)) * tetrahedron.GetVertex(2) - tetrahedron.GetVertex(1);
		return;
	}
	float ACD_AC = d8 * d3 - d9 * d2;
	if (ABC_AC >= 0 && ACD_AC >= 0 && d2 >= 0 && d8 <= 0)
	{
		//Edge AC Region
		rtn_point = tetrahedron.GetVertex(0) + (d2 / d2 - d8) * tetrahedron.GetVertex(2) - tetrahedron.GetVertex(0);
		return;
	}
	float d10 = (point-tetrahedron.GetVertex(3)).dot(tetrahedron.GetVertex(1) - tetrahedron.GetVertex(0));
	float d11 = (point-tetrahedron.GetVertex(3)).dot(tetrahedron.GetVertex(2) - tetrahedron.GetVertex(0));
	float d12 = (point-tetrahedron.GetVertex(3)).dot(tetrahedron.GetVertex(3) - tetrahedron.GetVertex(0));

	if (d12 >= 0 && d10 <= d12 && d11 <= d12)
	{
		//Vertex C Region
		rtn_point = tetrahedron.GetVertex(3);
		return;
	}

	float ADB_BD = d6 * d10 - d4 * d12;
	float ADB_AD = d12 * d1 - d10 * d3;

	if (ADB_AD <= 0 && ADB_AB <= 0 && ADB_BD <= 0)
	{
		//Face ADB Region
		float denom = 1.0f / (ADB_AD + ADB_AB + ADB_BD);
		float b = ADB_AD * denom;
		float d = ADB_AB * denom;
		float a = 1.0f - d - b;
		rtn_point = a * tetrahedron.GetVertex(0) + d * tetrahedron.GetVertex(3) + b * tetrahedron.GetVertex(1);
		return;
	}

	float ACD_AD = d2 * d12 - d3 * d11;
	if (ADB_AD >= 0 && ACD_AD >= 0 && d3 >= 0 && d12 <= 0)
	{
		//Edge AD Region
		rtn_point = tetrahedron.GetVertex(0) + (d3 / d3 - d12) * tetrahedron.GetVertex(3) - tetrahedron.GetVertex(0);
		return;
	}
	float ACD_CD = d11 * d9 - d12 * d8;

	if (ACD_AC <= 0 && ACD_AD <= 0 && ACD_CD <= 0)
	{
		//Face ADB Region
		float denom = 1.0f / (ACD_AC + ACD_AD + ACD_CD);
		float d = ACD_AC * denom;
		float c = ACD_AD * denom;
		float a = 1.0f - c - d;
		rtn_point = a * tetrahedron.GetVertex(0) + c * tetrahedron.GetVertex(2) + d * tetrahedron.GetVertex(3);
		return;
	}

	float BDC_BD = (d12-d10)*(d5-d4) - (d11-d10)*(d6-d4);
	if (BDC_BD >= 0 && ADB_BD >= 0 && d6 >= d4 && d12 <= d10)
	{
		//Edge BD Region
		rtn_point = tetrahedron.GetVertex(1) + ((d6-d4) / (d6-d4) - (d12-d10)) * tetrahedron.GetVertex(3) - tetrahedron.GetVertex(1);
		return;
	}
	float BDC_CD = (d12-d10)*(d5-d4) - (d11-d10)*(d6-d4);
	if (ACD_CD >= 0 && BDC_CD >= 0 && d9 >= d8 && d12 <= d11)
	{
		//Edge CD Region
		rtn_point = tetrahedron.GetVertex(2) + ((d9-d8) / (d9-d8) - (d12-d11)) * tetrahedron.GetVertex(3) - tetrahedron.GetVertex(2);
		return;
	}

	if (BDC_BD <= 0 && BDC_BC <= 0 && BDC_CD <= 0)
	{
		//Face BDC Region
		float denom = 1.0f / (BDC_BD + BDC_BC + BDC_CD);
		float c = BDC_BD * denom;
		float d = BDC_BC * denom;
		float b = 1.0f - c - d;
		rtn_point = b * tetrahedron.GetVertex(1) + d * tetrahedron.GetVertex(2) + c * tetrahedron.GetVertex(3);
		return;
	}

	//Point is Inside Tetrahedron.
	rtn_point = point;
	return;
}


void BasicPrimitiveTests::ClosestPointOfTwoLines(const Line & line_a, const Line & line_b, float & s, float & t)
{
	/*
		Main Idea:
                - Finds a point on each line such that the vector between those points are perpendicular to both lines.

        Let:
            -> Line 1: L1 = P1 + s * d1
                -> where d1 = Q1-P1

            -> Line 2: L2 = P2 + t * d2
                -> where d2 = Q2-P2

            -> Vector between the two lines:
                -> v(s, t) = L1(s) - L2(t)

        Derivation:
            Two perpendicularity constraints:
                -> dot(d1, v(s, t)) = 0
                -> dot(d2, v(s, t)) = 0

            -> Plug-in Equations to form 2x2 system of linear equations.
            -> Solve for s, t with Cramer's Rule:
                    -> where a = dot(d1, d1)
                    -> where b = dot(d1, d2)
                    -> where c = dot(d1, P1-P2)
                    -> where d = a * e - exp(b, 2)
                    -> where e = dot(d2, d2)
                    -> where f = dot(d2, P1-P2)
                -> s = (b * f - c * e) / d
                -> t = (a * f - b * c) / d

		Lines Parallel Case:
			-> d = 0
				-> Handle separately and return any point on line A and its projection onto B.
	*/

	float a = line_a.GetDirection().dot(line_a.GetDirection());
	float b = line_a.GetDirection().dot(line_b.GetDirection());
	float c = line_a.GetDirection().dot(line_a.GetPoint() - line_b.GetPoint());
	float e = line_b.GetDirection().dot(line_b.GetDirection());
	float d = a * e - pow(b, 2);
	float f = line_b.GetDirection().dot(line_a.GetPoint() - line_b.GetPoint());

	if (abs(d) > EPSILON)
	{
		s = (b * f - c * e) / d;
		t = (a * f - b * c) / d;
	}
	else
	{
		s = 0.0f;
		t = line_b.GetDirection().dot(line_a.GetPoint() - line_b.GetPoint()) / line_b.GetDirection().norm();
	}
}

void BasicPrimitiveTests::ClosestPointOfTwoLineSegments(const LineSegment & segment_a, const LineSegment & segment_b, float & s, float & t)
{
	/*
		Main Idea:
                -> Finds a point on each line such that the vector between those points are perpendicular to both lines.
                    -> Must use clamping procedures for endpoint cases.

		Derivation:
            Two perpendicularity constraints:
                -> dot(d1, v(s, t)) = 0
                -> dot(d2, v(s, t)) = 0

            -> Plug-in Equations to form 2x2 system of linear equations.
            -> Solve for s, t with Cramer's Rule:
                    -> where a = dot(d1, d1)
                    -> where b = dot(d1, d2)
                    -> where c = dot(d1, P1-P2)
                    -> where d = a * e - exp(b, 2)
                    -> where e = dot(d2, d2)
                    -> where f = dot(d2, P1-P2)
                -> s = (b * f - c * e) / d
                -> t = (a * f - b * c) / d

        Clamping:
            -> Every time the closest point for one segment is clamped to an endpoint, then closest point on the other segment must be recomputed.
	*/

	Eigen::Vector3f a_direction = segment_a.GetPointB() - segment_a.GetPointA();
	Eigen::Vector3f b_direction = segment_b.GetPointB() - segment_b.GetPointA();

	float a = a_direction.dot(a_direction);
	float e = b_direction.dot(b_direction);
	float f = b_direction.dot(segment_a.GetPointA() - segment_b.GetPointA());

	if (a <= EPSILON && e <= EPSILON)
	{
		//both a and e degenerate to points.
		s = 0;
		t = 0;
		return;
	}
	if (a <= EPSILON)
	{
		//a degenerates to point.
		s = 0.0f;
		t = Clamp(f / e, 0.0f, 1.0f); //s = 0; t = (b*s + f) / e = f / e
	}
	else
	{
		float c = a_direction.dot(segment_a.GetPointA() - segment_b.GetPointA());
		if (e <= EPSILON)
		{
			//e degenerates to point.
			t = 0.0f;
			s = Clamp(-c / a, 0.0f, 1.0f); // t = 0; s = (b*t - c) / a = -c / a
		}
		else
		{
			float b = a_direction.dot(b_direction);
			float d = a * e - b * b;

			if (abs(d) > EPSILON)
			{
				s = Clamp((b * f - c * e) / d, 0.0f, 1.0f);
			}
			else
			{
				s = 0.0f;
			}

			t = (b * s + f) / e;

			if (t < 0.0f)
			{
				t = 0.0f;
				s = Clamp(-c / a, 0.0f, 1.0f);
			}
			else if (t > 1.0f)
			{
				t = 1.0f;
				s = Clamp((b - c) / a, 0.0f, 1.0f);
			}
		}
	}
}


void ClosestPointsOfALineSegmentAndATriangle(const LineSegment & segment, const Triangle & triangle, Eigen::Vector3f & rtn_point_a, Eigen::Vector3f & rtn_point_b)
{
	/*
		Main Idea:
            - Separates test into two cases:
                - Minimum distance occurs between an endpoint of the segment and the interior of the triangle
                - Minimum distance occurs between the segment and an edge of the triangle

        Let:
            -> Line segment:
                -> PQ
            -> Triangle:
                -> ABC

        Procedure:
            -> Compute the closest pairs of points between:
                -> PQ and edge AB
                -> PQ and edge BC
                -> PQ and edge CA
                -> P and triangle plane
                -> Q and triangle plane
            -> Return the pair with overall smallest minimum distance as result.

        Remaining case:
            -> When segment intersects triangle.
                -> Return any intersection point.

        -> Can reduce number of tests by examining which Voronoi regions the segment endpoints lie in.
            -> In order to determine which tests are necessary.
                -> When both project into triangle, no segment-edge tests needed.
                -> When only one endpoint projects into the triangle, only one segment-edge test is required.
                -> When both project outside of triangle, one of the segment-edge tests not needed.
	*/
}
void ClosestPointsOfTwoTriangles(const Triangle & triangle_a, const Triangle & triangle_b, Eigen::Vector3f & rtn_point_a, Eigen::Vector3f & rtn_point_b)
{
	/*
		Main Idea:
            Uses realization that the closest pair of points occur either 
                -> on edge from each triangle
                -> as vertex of one triangle and a point interior to the other triangle.

        Test
            -> Compute closest points among all pairs of edges
                -> Nine edge-edge tests
            -> Compute closest points on one triangle to each vertex on the other
                -> Six vertex-triangle tests
            -> Then out of these closest pairs, find the pair with minimum distance.

        -> Additional test needed to rule out intersection of the two triangles.
	*/
}