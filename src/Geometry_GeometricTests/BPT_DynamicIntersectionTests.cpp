#include "BasicPrimitiveTests.h"

bool BasicPrimitiveTests::IntersectingMovingSphereAgainstPlane(const BoundingSphere & sphere, const Plane & plane)
{
	/*
		Main Idea:
            -> Interpret the sphere center's movement as a line segment.
            -> Intersect the line segment with the plane using Section 5.3.1
                -> Displaced by sphere's radius.

        Categorize the Sphere's movement:
            Since [...] indicates what side the plane the sphere is on
                -> dot(n, C) - d

            Toward plane:
                -> dot(n, v) * dot(n, C-d) > 0

            Away from plane:
                -> dot(n, v) * dot(n, C-d) < 0

            Parallel to plane:
                -> dot(n, v) = 0

        If Sphere is moving towards plane:
            -> Displace plane by sphere's radius towards sphere
            -> Interpret sphere center's movement as line segment.
                -> Intersect the line segment with plane.
                    -> Use Section 5.3.1
                    -> Insert movement segment as X into displaced plane equation.
                        -> Solve for "t"
	*/
	return false;
}
bool BasicPrimitiveTests::IntersectingMovingAABBAgainstPlane(const AABB & aabb, const Plane & plane)
{
	/*
		Main Idea:
            -> Separating axis test with plane normal "n" as sole axis.
            -> Reduce the test to a moving-sphere-against-plane test.
                -> By finding the projection radius of the AABB with respect to the plane's normal.

        Let:
            Plane: 
                -> dot(n, X) = d
            AABB:
                -> Center:
                    -> C
                -> Local axis vectors:
                    -> u0, u1, u2
                -> Extents:
                    -> e0, e1, e2

        Test:
            -> Get projection radius of ABB with respect to the plane's normal:
                -> r = e0 * |dot(u0,n)| + e1 * |dot(u1, n)| + e2 * |dot(u2, n)|
                
                -> Since u0, u1, u2 are axis-aligned and unit length:
                    -> r = e0 * |n.x| + e1 * |n.y| + e2 * |n.z|

            Test for initial contact:
                -> length(dot(n, C) - d) <= r
           
            If AABB moving towards plane:
                -> Witness point for contact on AABB will be:
                    -> Q = C(t) - r * n
                        -> where C(t) = center location at time "t" during intersection.

                    -> Q on place when:
                        -> dot(n, Q) = d
                            -> Solve for "t":
                                -> t = (    r + d - dot(n, C)    /    dot(n, v)    )

	*/
	return false;
}
bool BasicPrimitiveTests::IntersectingMovingSphereAgainstSphere(const BoundingSphere & sphere_a, const BoundingSphere & sphere_b)
{
	/*
		Main Idea:
            Method 1:
                -> Get the displacement vector between sphere centers by expressing sphere movements as line segments.
                    -> Find the time when the spheres come into contact,
                        -> When the displacement vector's length = sum of their radii.
                        -> Requires quadratic formula.
            
            Method 2:
                -> Turn problem into moving sphere against stationary sphere 
                    -> subtract one velocity from both.

                -> Turn problem into moving point (ray) against stationary sphere 
                    -> shrink radius of one and grow radius of the other.
                    -> Possible because spheres first come into contact when distance between them = radius_0 + radius_1

                -> Solve with Section 5.3.2

        Method 1 Details:
            Let:
                -> d(t) = displacement vector between sphere centers.

            Contact Condition:
                -> dot(   d(t), d(t))   ) = exp( r0 + r1 , 2)
                    -> where ri = radii.
                    -> This equation is squared on both sides to avoid square roots.

                -> Solve for "t"
                    -> Encounters quadratic equation in "t".
                        -> t = (    -b [plus/minus] sqrt(b*b - a*c)    /    a    )
                            -> where a = dot(v, v)
                            -> where b = dot(v, s)
                            -> where c = dot(s, s) - r*r

                        -> Number of real roots => number of intersections.
                            -> For two real root, first contact at smaller "t" then separation at larger "t".
	*/
	return false;
}
bool BasicPrimitiveTests::IntersectingMovingSphereAgainstTriangle(const BoundingSphere & sphere, const Triangle & triangle)
{
	/*
		Main Idea:
            -> Both methods model the movement of the first point on the sphere (not sphere center) to come in contact with the polygon's plane as a line segment.
                -> Call this line segment L.

            -> Both methods split the test into two cases:
                -> If the intersection is within the polygon.
                -> If the intersection is not inside the polygon.

        Movement of first point of sphere to make contact:
            -> L(t) = D + t * v.
                -> where v = sphere velocity.
                -> where D = C - r * n
                    -> where r = sphere radius
                    -> where n = plane normal

        Method 1:
            -> Intersect L with polygon's plane, then test if intersection is within polygon.
                -> If inside, intersects.

                -> If outside, must check if sphere later intersects polygon at different point.
                    -> If if it does, it will be at point Q on that triangle/polygon that is closest to L's interesction with the plane.
                        -> Test if sphere hits Q by shooting a ray in verse from Q towards sphere.
                            -> Ray: Q - t * v

        Method 2:

            Main Idea:
                -> Ray test against the Minkowsky sum of Sphere and Triangle.

            -> Intersect L with polygon's plane, then test if intersection is within polygon.
                -> If inside, intersects.
                -> If outside, continue.

            -> Test moving sphere against each triangle edge.
                -> Turn this into a test between the ray of the sphere's movement against the edges turned into cylinders with sphere's radius.
                -> If no intersection, continue.

            -> Test moving sphere with each vertex.
                -> Equivalent to testing ray with vertices with sphere's radius.
	*/
	return false;
}
bool BasicPrimitiveTests::IntersectingMovingSphereAgainstAABB(const BoundingSphere & sphere, const AABB & aabb)
{
	/*
		Main Idea:
                -> Ray test with Minkowsky sum of Sphere and AABB   
                    -> Minkowsky sum will be the box with faces expanded outwards by sphere's radius WITH corners beveled.

        Let:
            -> E = Minkowsky sum of sphere and AABB, without the beveled corners.
                -> In other words, E = AABB with faces expanded outwards by sphere's radius.
            
            -> L = Line Segment representing sphere center's movement.

        Procedure:
            -> Intersect L with E.
            -> If intersect,
                -> Must split test into 3 cases based on if intersection is in face, vertex, or edge Voronoi region of AABB.

                    Face Voronoi region:
                        -> Intersects.

                    Edge Voronoi region:
                        -> Must intersect with the capsule of sphere's radius determined by the edge.

                    Vertex Vornoi region:
                        -> Must test for intersection with capsules of sphere's radius from all three edges coincident to the vertex.

                            Potential speed-up:
                                -> Before testing the three capsules,
                                    -> Test L with vertex with sphere's radius.
                                        -> If intersects, then test ends with this intersection.
                                        -> If no intersection, continue testing the three edge capsules.
	*/
	return false;
}
bool BasicPrimitiveTests::IntersectingMovingAABBAgainstAABB(const AABB & aabb_a, const AABB & aabb_b)
{
	/*
		Main Idea:
            -> Moving Separating-Axis test using the three principle axes.
	*/
	return false;
}