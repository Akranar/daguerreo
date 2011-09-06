#ifndef _QUICKHULL_H_
#define _QUICKHULL_H_

#include <set>
#include <vector>
#include "ExLib_Eigen.h"

class FloatDataAccessor;
class WingedTriangleMesh;
class WingedTriangle;

class QuickHull
{
public:
	static void ConvexHull(const FloatDataAccessor & points, WingedTriangleMesh & rtn_hull);
	static unsigned int MaximalSimplex(const FloatDataAccessor & points, WingedTriangleMesh & rtn_hull);
	static void CalculateHorizon(const Eigen::Vector3f & eye, 
								 const FloatDataAccessor & point_set, 
								 WingedTriangle * start_triangle, 
								 std::vector<std::pair<unsigned int, unsigned int> > & rtn_edges,
								 std::set<WingedTriangle *> & rtn_visited_triangles);
private:
	static float DistanceToTriangle(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c, const Eigen::Vector3f & point);
	static bool TriangleFacesEye(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c, const Eigen::Vector3f & eye);

	static void ExtremePointsAlongStandardAxes(const FloatDataAccessor & point_set, 
												   unsigned int & x_min_index, unsigned int & x_max_index,
												   unsigned int & y_min_index, unsigned int & y_max_index,
												   unsigned int & z_min_index, unsigned int & z_max_index
												   );
	static void SimplexHelper_2(const FloatDataAccessor & point_set, std::set<unsigned int> & unique_count, WingedTriangleMesh & rtn_hull);
	static void SimplexHelper_3(const FloatDataAccessor & point_set, std::set<unsigned int> & unique_count, WingedTriangleMesh & rtn_hull);
	static void SimplexHelper_4(const FloatDataAccessor & point_set, unsigned int min[3], unsigned int max[3], WingedTriangleMesh & rtn_hull);
};

#endif