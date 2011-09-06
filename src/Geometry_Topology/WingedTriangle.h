#ifndef _WINGED_TRIANGLE_H_
#define _WINGED_TRIANGLE_H_

#include <Core_ByteArray.h>
#include <stack>

class WingedTriangleMesh;

class WingedTriangle
{
	friend class WingedTriangleMesh;
	unsigned int vertices[3];
	WingedTriangle * neighbors[3];
	ByteArray<4> index_in_neighbors;
	unsigned int index_in_mesh;
public:
	WingedTriangle(unsigned int a, unsigned int b, unsigned int c);
	virtual ~WingedTriangle();
	inline WingedTriangle * GetNeighbor(int index) const;
	inline int GetIndexInNeighbor(int index) const;
	inline unsigned int GetEdgeSrc(int edge_index) const;
	inline unsigned int GetEdgeDest(int edge_index) const;
	inline bool EdgeIsBoundary(int edge_index) const;
	inline bool IsBoundary() const;
	inline unsigned int GetOppositeVertex(int edge) const;
	inline void GetEdgeCC(int edge, int & rtn_edge, WingedTriangle ** rtn_triangle) const;

	inline unsigned int GetIndexInMesh() const;
	inline void SetIndexInMesh(unsigned int index);
	
	void FlipEdge(int edge);
	void Print() const;

};

inline unsigned int WingedTriangle::GetIndexInMesh() const
{
	return index_in_mesh;
}

inline void WingedTriangle::SetIndexInMesh(unsigned int index)
{
	index_in_mesh = index;;
}

inline WingedTriangle * WingedTriangle::GetNeighbor(int index) const
{
	return neighbors[index % 3];
}

inline int WingedTriangle::GetIndexInNeighbor(int index) const
{
	return index_in_neighbors[index];
}

inline unsigned int WingedTriangle::GetEdgeSrc(int edge_index) const
{
	return vertices[edge_index];
};

inline unsigned int WingedTriangle::GetEdgeDest(int edge_index) const
{
	return vertices[(edge_index+1) % 3];
};

inline bool WingedTriangle::EdgeIsBoundary(int edge_index) const
{
	return !neighbors[edge_index];
}

inline void WingedTriangle::GetEdgeCC(int edge, int & rtn_edge, WingedTriangle ** rtn_triangle) const
{
	*rtn_triangle = neighbors[edge];
	rtn_edge = (index_in_neighbors[(edge-1)  %3]);
}

inline unsigned int WingedTriangle::GetOppositeVertex(int edge) const
{
	return neighbors[edge]->GetEdgeSrc(index_in_neighbors[(edge-1)  %3]);
}

inline bool WingedTriangle::IsBoundary() const
{
	if (!neighbors[0]) return true;
	if (!neighbors[1]) return true;
	if (!neighbors[2]) return true;
	return false;
}



/*
class WingedTriangleOrdering
{
public:
	bool operator() (const WingedTriangle & x, const WingedTriangle & y)
	{
		if (x.vertices[0] < y.vertices[0]) return true;
		if (x.vertices[0] > y.vertices[0]) return false;
		if (x.vertices[1] < y.vertices[1]) return true;
		if (x.vertices[1] > y.vertices[1]) return false;
		if (x.vertices[2] < y.vertices[2]) return true;
		if (x.vertices[2] > y.vertices[2]) return false;
		return false;
	}
};
*/


#endif