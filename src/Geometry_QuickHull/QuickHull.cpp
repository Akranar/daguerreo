#include "QuickHull.h"
#include "ExLib_Topology.h"
#include "ExLib_FloatDataAccessor.h"
#include <Core_Assert.h>

#include <set>
#include <map>
#include <vector>
#include <list>
#include <stack>

#include "OutsideSet.h"

void QuickHull::ConvexHull(const FloatDataAccessor & point_set, WingedTriangleMesh & rtn_hull)
{
	/*
        3.9.2 - The Quickhill Algorithm
            Procedure:
                -> Obtain bounding box to point set as first approximination of convex hull,
                    -> by finding the 4 extreme points in each direction (in 2D).
                        -> These points must lie in convex hull since they are extreme.
                -> Points lying inside this approximation hull cannot be on convex hull, remove from further consideration.
                -> Refine approximination with points outside of current hull:
                    -> For each edge:
                        -> Find point farthest away in the direction perpendicular to the edge.  Add it to the hull.
                -> Repeatedly refine until no more points.

            Robustness Considerations:
                Problem:
                    -> Initial hull may not always be quad.
                        -> May have triangle if same point is extreme for two directions.
                        -> May have a degenerate hull if two points are the extreme for two directions.
                    -> May need to break ties between vertices for refining hull.
                        -> Select the one projecting farthest along the parallel of the edge.

		http://algolist.manual.ru/maths/geom/convhull/qhull3d.php

		Algorithm Implementation Details:
			-> Maximal Simplex as Starting Point
				-> Will be a tetrahedron. Otherwise, return as degeneracy.
			-> For each triangle in convex hull,
				-> Define "OutsideSet" as the set of points outside of that face.
					-> Signed distance from the face is greater than 0.
				-> Divide points into OutsideSets.
				-> Invariant:
					-> The ordering of OutsideSets and Triangles in WingedTriangleMesh's storage always match.

					Preserved by Mimicking WingedTriangleMesh's removal scheme when removing OutsideSets:
						-> WingedTriangleMesh's fast array element removal:
							-> If element is not at end of array,
								-> Move last element into removed element's place.

	*/

	//Calculate Maximal Simplex as Starting Point.
	int simplex_dimension = MaximalSimplex(point_set, rtn_hull);

	if (simplex_dimension < 4)
	{
		//DEGENERATE HULL.
		return;
	}

	//Construct Outside Sets
	typedef std::vector<OutsideSet *> OUTSIDE_SETS;
	OUTSIDE_SETS outside_sets;

	for (unsigned int i = 0; i < rtn_hull.GetSize(); ++i)
	{
		outside_sets.push_back(new OutsideSet(rtn_hull[i]));
	}

	//For each point, if Signed distance from a face of the hull is greater than 0,
	//	-> Place in that face's Outside Set.
	for (unsigned int i = 0; i < point_set.GetCount(); ++i)
	{
		Eigen::Vector3f & current_point = VEC3(point_set[i]);

		for (unsigned int j = 0; j < rtn_hull.GetSize(); ++j)
		{
			unsigned int a_index = rtn_hull[j]->GetEdgeSrc(0);
			unsigned int b_index = rtn_hull[j]->GetEdgeSrc(1);
			unsigned int c_index = rtn_hull[j]->GetEdgeSrc(2);

			if (i == a_index || i == b_index || i == c_index) continue; //DEGENERATE TRIANGLE

			Eigen::Vector3f & a = VEC3(point_set[rtn_hull[j]->GetEdgeSrc(0)]);
			Eigen::Vector3f & b = VEC3(point_set[rtn_hull[j]->GetEdgeSrc(1)]);
			Eigen::Vector3f & c = VEC3(point_set[rtn_hull[j]->GetEdgeSrc(2)]);
			
			float distance = DistanceToTriangle(a, b, c, current_point);
			if (distance > 1e-10) //Robustness epsilon
			{
				
				outside_sets[j]->Insert(i, distance);
				break;
			}
		}
	}

	
	//Main Loop
	int pass = 0;
	for (unsigned int i = 0; i < outside_sets.size();)
	{
		if (outside_sets[i]->Size() == 0)
		{
			++i;
			continue;
		}
		
		//Calculate Horizon of Hull from point of view of "farthest point" in this Outside Set.
		//	-> The triangles in "visible_triangles" will be removed and replaced.
		//		-> With a cone from "farthest point" to Hull horizon.
		//	Invariant:
		//		-> Horizon Edges will always form a counter-clockwise loop.
		unsigned int farthest = outside_sets[i]->GetFarthest();
		std::vector<std::pair<unsigned int, unsigned int> > horizon_edges;
		std::set<WingedTriangle *> visited_triangles;
		CalculateHorizon(VEC3(point_set[farthest]), 
								 point_set, 
								 outside_sets[i]->GetTriangle(), 
								 horizon_edges, 
								 visited_triangles);

		//Create new triangles in the form of a cone.
		//	-> Cone Point is Farthest point in this OutsideSet.
		//	-> The circular base is made from the horizon edges.
		std::vector<unsigned int> new_triangles;
		for (unsigned int j = 0; j < horizon_edges.size(); ++j)
		{
			unsigned int src = horizon_edges[j].first;
			unsigned int dest = horizon_edges[j].second;

			new_triangles.push_back(farthest);
			new_triangles.push_back(src);
			new_triangles.push_back(dest);
		}

		
		std::vector<unsigned int> outside_set_union;
		Assert(visited_triangles.size() > 0, "Visited Triangles empty.");
		unsigned int min_modified_index = INT_MAX;
		
		//Removal of Visible Triangles
		//	-> The triangles enclosed by the horizon edge loop are removed.
		//	-> Also remove the triangle's corresponding OutsideSet.
		//		-> Mimick WingedTriangleMesh's removal scheme to preserve order correspondance
		//			-> Between WingedTriangles and OutsideSets.
		for (std::set<WingedTriangle *>::iterator it = visited_triangles.begin(); it != visited_triangles.end(); ++it)
		{
			WingedTriangle * to_remove = *it;

			unsigned int triangle_index = to_remove->GetIndexInMesh();
			if (triangle_index < min_modified_index) min_modified_index = triangle_index;

			OutsideSet * to_remove_set = outside_sets[triangle_index];
			Assert(to_remove_set, "set doesn't exist");
			Assert(to_remove_set->GetTriangle() == *it, "Mismatch set and triangle");

			//Save the points in each OutsideSet to be deleted.
			//	-> Later redivide them
			//		-> Among new triangles in the Cone.
			for (unsigned int j = 0; j < to_remove_set->Size(); ++j)
			{
				outside_set_union.push_back((*to_remove_set)[j]);
			}

			//Delete OutsideSet
			if (to_remove->GetIndexInMesh() != outside_sets.size()-1)
			{
				delete to_remove_set;
				outside_sets[to_remove->GetIndexInMesh()] = outside_sets.back();
			}
			else
			{
				delete outside_sets[to_remove->GetIndexInMesh()];
			}
			outside_sets.pop_back();

			//Delete Triangle
			rtn_hull.RemoveTriangle(to_remove);
		}

		
		//Add New Triangles and Corresponding OutsideSets.
		OUTSIDE_SETS new_outside_sets;
		for (unsigned int j = 0; j < new_triangles.size(); j += 3)
		{
			rtn_hull.AddTriangle(new_triangles[j], new_triangles[j+1], new_triangles[j+2]);
			WingedTriangle * just_added_triangle = rtn_hull[rtn_hull.GetSize()-1];
			new_outside_sets.push_back(new OutsideSet(just_added_triangle));

			Assert((new_outside_sets.size() + outside_sets.size() -1) == just_added_triangle->GetIndexInMesh(), "Mismatch Occurred");
		}

		for (unsigned int j = 0; j < new_outside_sets.size(); ++j)
		{
			outside_sets.push_back(new_outside_sets[j]);
		}
		
		//Redivide Points from Deleted OutsideSets into the Newly Created Outside Sets.
		for (unsigned int j = 0; j < outside_set_union.size(); ++j)
		{
			unsigned int current_index = outside_set_union[j];
			Eigen::Vector3f & current_point = VEC3(point_set[current_index]);

			for (unsigned int k = 0; k < new_triangles.size(); k += 3)
			{
				unsigned int a_index = new_triangles[k];
				unsigned int b_index = new_triangles[k+1];
				unsigned int c_index = new_triangles[k+2];

				if (current_index == a_index || current_index == b_index || current_index== c_index) continue; //DEGENERATE TRIANGLE

				Eigen::Vector3f & a = VEC3(point_set[a_index]);
				Eigen::Vector3f & b = VEC3(point_set[b_index]);
				Eigen::Vector3f & c = VEC3(point_set[c_index]);
				
				float distance = DistanceToTriangle(a, b, c, current_point);
				if (distance > 1e-10) //Robustness epsilon
				{
					new_outside_sets[k/3]->Insert(outside_set_union[j], distance);
					break;
				}
			}
		}

		//Due to the Element Removal Schemes of WingedTriangleMesh and OutsideSet's Array,
		//	-> Must restart looping from the earliest index that we removed a WingedTriangle/Outside Set from.
		i = min_modified_index;
	}
	
	//Clean up
	for (unsigned int i = 0; i < outside_sets.size(); ++i)
	{
		delete outside_sets[i];
	}
	outside_sets.clear();
}

unsigned int QuickHull::MaximalSimplex(const FloatDataAccessor & point_set, WingedTriangleMesh & rtn_hull)
{
	unsigned int min[3];
	unsigned int max[3];
	ExtremePointsAlongStandardAxes(point_set, min[0], max[0], min[1], max[1], min[2], max[2]);

	std::set<unsigned int> unique_count;
	unique_count.insert(min[0]);
	unique_count.insert(max[0]);
	unique_count.insert(min[1]); 
	unique_count.insert(max[1]);
	unique_count.insert(min[2]);
	unique_count.insert(max[2]);

	if (unique_count.size() == 1)
	{
		return 1;
	}
	else if (unique_count.size() == 2)
	{
		SimplexHelper_2(point_set, unique_count, rtn_hull);
		return 2;
	}
	else if (unique_count.size() == 3)
	{
		SimplexHelper_3(point_set, unique_count, rtn_hull);
		return 3;
	}
	else if (unique_count.size() >= 4)
	{
		SimplexHelper_4(point_set, min, max, rtn_hull);
		return 4;
	}
	return 0;
}

void QuickHull::CalculateHorizon(const Eigen::Vector3f & eye, 
								 const FloatDataAccessor & point_set, 
								 WingedTriangle * start_triangle, 
								 std::vector<std::pair<unsigned int, unsigned int> > & rtn_edges,
								 std::set<WingedTriangle *> & rtn_visited_triangles)
{
	/*
		Algorithm Overview:
			-> Depth first traversal using stack
				-> Start from triangle whose this OutsideSet belongs to.
			-> For each visited triangle, 
				-> Invariant:
					-> Visited triangles always face the Eye.
				-> If an adjacent triangle does not face Eye.
					-> Means edge between them part of horizon.
					-> And that the neighbor is not visible.
	*/
	std::stack<WingedTriangle *> traversal_stack;
	traversal_stack.push(start_triangle);

	while (!traversal_stack.empty())
	{
		WingedTriangle * current = traversal_stack.top();
		traversal_stack.pop();
		bool already_visited = !(rtn_visited_triangles.insert(current).second);
		if (already_visited) continue;

		unsigned int current_indices[3];
		current_indices[0] = current->GetEdgeSrc(0);
		current_indices[1] = current->GetEdgeSrc(1);
		current_indices[2] = current->GetEdgeSrc(2);

		Assert(QuickHull::TriangleFacesEye(
										VEC3(point_set[current_indices[0]]), 
										VEC3(point_set[current_indices[1]]), 
										VEC3(point_set[current_indices[2]]), 
										eye), 
				"Visiting Non-visible triangle.");

		for (unsigned int i = 0; i < 3; ++i)
		{
			WingedTriangle * neighbor = current->GetNeighbor(i);

			unsigned int neighbor_indices[3];
			neighbor_indices[0] = neighbor->GetEdgeSrc(0);
			neighbor_indices[1] = neighbor->GetEdgeSrc(1);
			neighbor_indices[2] = neighbor->GetEdgeSrc(2);

			Assert(neighbor != 0, "Neighbor should never be null");

			bool edge_is_horizon = !(QuickHull::TriangleFacesEye(
														VEC3(point_set[neighbor_indices[0]]), 
														VEC3(point_set[neighbor_indices[1]]), 
														VEC3(point_set[neighbor_indices[2]]), 
														eye));
			if (edge_is_horizon)
			{
				rtn_edges.push_back(std::make_pair(current->GetEdgeSrc(i), current->GetEdgeDest(i)));
			}
			else
			{
				traversal_stack.push(neighbor);
			}
		}
	}

}


void QuickHull::ExtremePointsAlongStandardAxes(const FloatDataAccessor & point_set, 
												   unsigned int & x_min_index, unsigned int & x_max_index,
												   unsigned int & y_min_index, unsigned int & y_max_index,
												   unsigned int & z_min_index, unsigned int & z_max_index
												   )
{
	x_min_index = 0;
	x_max_index = 0;
	y_min_index = 0;
	y_max_index = 0;
	z_min_index = 0;
	z_max_index = 0;

	for (unsigned int i = 1; i < point_set.GetCount(); ++i)
	{
		Eigen::Vector3f & point = VEC3(point_set[i]);
		if (point[0] < VEC3(point_set[x_min_index])[0]) x_min_index = i;
		if (point[0] > VEC3(point_set[x_max_index])[0]) x_max_index = i;
		if (point[1] < VEC3(point_set[y_min_index])[1]) y_min_index = i;
		if (point[1] > VEC3(point_set[y_max_index])[1]) y_max_index = i;
		if (point[2] < VEC3(point_set[z_min_index])[2]) z_min_index = i;
		if (point[2] > VEC3(point_set[z_max_index])[2]) z_max_index = i;
	}
}

float QuickHull::DistanceToTriangle(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c, const Eigen::Vector3f & point)
{
	Eigen::Vector3f normal = (b-a).cross(c-a);
	normal.normalize();
	float D = -normal.dot(a);
	return normal.dot(point) + D;
}

bool QuickHull::TriangleFacesEye(const Eigen::Vector3f & a, const Eigen::Vector3f & b, const Eigen::Vector3f & c, const Eigen::Vector3f & eye)
{
	Eigen::Vector3f normal = (b-a).cross(c-a);
	normal.normalize();
	float D = -normal.dot(a);
	return normal.dot(eye) + D > 1e-10;
}

