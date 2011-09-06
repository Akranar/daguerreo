#include "QuickHull.h"
#include "ExLib_Eigen.h"
#include "ExLib_FloatDataAccessor.h"
#include "ExLib_Topology.h"
#include <float.h>

void QuickHull::SimplexHelper_2(const FloatDataAccessor & point_set, std::set<unsigned int> & unique_count, WingedTriangleMesh & rtn_hull)
{
	std::set<unsigned int>::iterator it = unique_count.begin();
	unsigned int a, b, c;
	a = *it;
	++it;
	b = *it;

	Eigen::Vector3f d = VEC3(point_set[a]) - VEC3(point_set[b]);
	d.normalize();

	float max_sq_dist_from_d = -FLT_MAX;
	float right_most_value = -FLT_MAX;
	for(unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		if (i != a && i != b)
		{
			Eigen::Vector3f b_to_point = VEC3(point_set[i]) - VEC3(point_set[b]);
			float sq_dist_along_d = (b_to_point).dot(d);
			float sq_dist_perp_d = b_to_point.dot(b_to_point.normalized()) - abs(sq_dist_along_d);
			if (sq_dist_perp_d > max_sq_dist_from_d || (sq_dist_perp_d == max_sq_dist_from_d && sq_dist_along_d > right_most_value))
			{
				c = i;
				max_sq_dist_from_d = sq_dist_perp_d;
				right_most_value = sq_dist_along_d;
			}
		}
	}


	Eigen::Vector3f & a_point = VEC3(point_set[a]);
	Eigen::Vector3f & b_point = VEC3(point_set[b]);
	Eigen::Vector3f & c_point = VEC3(point_set[c]);


	Eigen::Vector3f plane_n = (b_point-a_point).cross(c_point-a_point);
	float plane_d = -a_point.dot(plane_n);

	unsigned int fourth_index;
	float dist_to_tri = 0;
	for (unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		if (i != a && i != b && i != c)
		{
			float signed_dist = VEC3(point_set[i]).dot(plane_n) - plane_d;
			if (abs(signed_dist) >= abs(dist_to_tri))
			{
				dist_to_tri = signed_dist;
				fourth_index = i;
			}
		}
	}

	
	if (dist_to_tri >= 0)
	{
		rtn_hull.AddTriangle(a, b, c);
		rtn_hull.AddTriangle(a, b, fourth_index);
		rtn_hull.AddTriangle(b, c, fourth_index);
		rtn_hull.AddTriangle(c, a, fourth_index);
	}
	else
	{
		rtn_hull.AddTriangle(a, c, b);
		rtn_hull.AddTriangle(b, a, fourth_index);
		rtn_hull.AddTriangle(c, b, fourth_index);
		rtn_hull.AddTriangle(a, c, fourth_index);
	}
	
}
void QuickHull::SimplexHelper_3(const FloatDataAccessor & point_set, std::set<unsigned int> & unique_count, WingedTriangleMesh & rtn_hull)
{
	std::set<unsigned int>::iterator it = unique_count.begin();
	unsigned int a, b, c;
	a = *it;
	++it;
	b = *it;
	++it;
	c = *it;
	++it;

	Eigen::Vector3f & a_point = VEC3(point_set[a]);
	Eigen::Vector3f & b_point = VEC3(point_set[b]);
	Eigen::Vector3f & c_point = VEC3(point_set[c]);


	Eigen::Vector3f plane_n = (b_point-a_point).cross(c_point-a_point);
	float plane_d = -a_point.dot(plane_n);

	unsigned int fourth_index;
	float dist_to_tri = 0;
	for (unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		if (i != a && i != b && i != c)
		{
			float signed_dist = VEC3(point_set[i]).dot(plane_n) - plane_d;
			if (abs(signed_dist) >= abs(dist_to_tri))
			{
				dist_to_tri = signed_dist;
				fourth_index = i;
			}
		}
	}

	if (dist_to_tri >= 0)
	{
		rtn_hull.AddTriangle(a, b, c);
		rtn_hull.AddTriangle(a, b, fourth_index);
		rtn_hull.AddTriangle(b, c, fourth_index);
		rtn_hull.AddTriangle(c, a, fourth_index);
	}
	else
	{
		rtn_hull.AddTriangle(a, c, b);
		rtn_hull.AddTriangle(b, a, fourth_index);
		rtn_hull.AddTriangle(c, b, fourth_index);
		rtn_hull.AddTriangle(a, c, fourth_index);
	}
}

struct TriangleIndices
{
	unsigned int a, b, c;
	TriangleIndices(unsigned int a, unsigned int b, unsigned int c)
		: a(a), b(b), c(c)
	{};
};

struct TriComp
{
	bool operator() (const TriangleIndices & x, const TriangleIndices & y)
	{
		if (x.a < y.a) return true;
		if (x.a > y.a) return false;
		if (x.b < y.b) return true;
		if (x.b > y.b) return false;
		if (x.c < y.c) return true;
		if (x.c > y.c) return false;
		return false;
	}
};


void QuickHull::SimplexHelper_4(const FloatDataAccessor & point_set, unsigned int min[3], unsigned int max[3], WingedTriangleMesh & rtn_hull)
{
	/*
		Calculate d = axis direction that is longest. {
	*/
	unsigned int max_index = 0;
	float max_d_length = 0;
	for (unsigned int i = 0; i < 3; ++i)
	{
		float dist = (VEC3(point_set[max[i]]) - VEC3(point_set[min[i]])).squaredNorm();
		if (dist >= max_d_length)
		{
			max_d_length = dist;
			max_index = i;
		}
	}

	if (max_index == 1)
	{
		std::swap(min[0], min[1]);
		std::swap(max[0], max[1]);
	}
	if (max_index == 2)
	{
		std::swap(min[0], min[2]);
		std::swap(max[0], max[2]);
	}

	Eigen::Vector3f d = VEC3(point_set[max[0]]) - VEC3(point_set[min[0]]);
	d.normalize();
	/*
		Calculate d = axis direction that is longest. }
		Calculate third point that is farthest away from d. {
	*/

	unsigned int min_maxes_without_0s[4];
	min_maxes_without_0s[0] = max[1];
	min_maxes_without_0s[1] = max[2];
	min_maxes_without_0s[2] = min[1];
	min_maxes_without_0s[3] = min[2];
	
	Eigen::Vector3f e[4];
	e[0] = VEC3(point_set[max[1]]) - VEC3(point_set[min[0]]);
	e[1] = VEC3(point_set[max[2]]) - VEC3(point_set[min[0]]);
	e[2] = VEC3(point_set[min[1]]) - VEC3(point_set[min[0]]);
	e[3] = VEC3(point_set[min[2]]) - VEC3(point_set[min[0]]);

	unsigned int third_index;
	float max_sq_dist_from_d = -FLT_MAX;
	float right_most_value = -FLT_MAX;
	for(unsigned int i = 0; i < 4; ++i)
	{
		float sq_dist_along_d = (e[i]).dot(d);
		float sq_dist_perp_d = (e[i]).dot(e[i].normalized()) - abs(sq_dist_along_d);
		if (sq_dist_perp_d > max_sq_dist_from_d || (sq_dist_perp_d == max_sq_dist_from_d && sq_dist_along_d > right_most_value))
		{
			third_index = i;
			max_sq_dist_from_d = sq_dist_perp_d;
			right_most_value = sq_dist_along_d;
		}
	}

	TriangleIndices tri = TriangleIndices(min[0], max[0], min_maxes_without_0s[third_index]);
	/*
		Calculate third point that is farthest away from d. }
		Calculate fourth point that is farthest away from Triangle. {
	*/

	Eigen::Vector3f & a = VEC3(point_set[tri.a]);
	Eigen::Vector3f & b = VEC3(point_set[tri.b]);
	Eigen::Vector3f & c = VEC3(point_set[tri.c]);

	Eigen::Vector3f plane_n = (b-a).cross(c-a);
	float plane_d = -a.dot(plane_n);

	unsigned int fourth_index;
	float dist_to_tri = 0;
	for (unsigned int i = 0; i < 4; ++i)
	{
		if (i != third_index)
		{
			float signed_dist = VEC3(point_set[min_maxes_without_0s[i]]).dot(plane_n) - plane_d;
			if (abs(signed_dist) >= abs(dist_to_tri))
			{
				dist_to_tri = signed_dist;
				fourth_index = i;
			}
		}
	}
	/*
		Calculate fourth point that is farthest away from Triangle. }
	*/

	if (dist_to_tri >= 0)
	{
		rtn_hull.AddTriangle(tri.a, tri.c, tri.b);
		rtn_hull.AddTriangle(tri.a, tri.b, min_maxes_without_0s[fourth_index]);
		rtn_hull.AddTriangle(tri.b, tri.c, min_maxes_without_0s[fourth_index]);
		rtn_hull.AddTriangle(tri.c, tri.a, min_maxes_without_0s[fourth_index]);
	}
	else
	{
		rtn_hull.AddTriangle(tri.a, tri.b, tri.c);
		rtn_hull.AddTriangle(tri.b, tri.a, min_maxes_without_0s[fourth_index]);
		rtn_hull.AddTriangle(tri.c, tri.b, min_maxes_without_0s[fourth_index]);
		rtn_hull.AddTriangle(tri.a, tri.c, min_maxes_without_0s[fourth_index]);
	}

}