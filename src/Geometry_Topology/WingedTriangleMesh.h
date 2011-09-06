#ifndef _WINGED_TRIANGLED_MESH_H_
#define _WINGED_TRIANGLED_MESH_H_

#include <vector>

class WingedTriangle;
typedef std::vector<WingedTriangle *> WingedTriSet;

class WingedTriangleMesh
{
	struct Edge
	{
		Edge(WingedTriangle * triangle, int index) : triangle(triangle), index(index) {};
		Edge(){};
		WingedTriangle * triangle;
		int index;
	};
	std::vector<std::vector<std::pair<unsigned int, Edge> > > edge_table;
	WingedTriSet triangles;
public:
	WingedTriangleMesh();
	WingedTriangleMesh(unsigned int vertex_capacity);
	virtual ~WingedTriangleMesh();
	void SetVertexCapacity(unsigned int n);
	void ReadIBuffer(int * ibuffer, unsigned ibuffer_size);
	void AddTriangle(unsigned int a, unsigned int b, unsigned int c);
	void RemoveTriangle(int index);
	void RemoveTriangle(WingedTriangle * triangle);
	void GetIndexBuffer(int * rtn_buffer) const;
	void ClearEdgeTable();
	void PrintContent() const;

	inline unsigned int GetSize() const;
	inline unsigned int GetIBufferSize() const;
	inline WingedTriangle * operator[] (unsigned int index);
	inline const WingedTriangle * operator[] (unsigned int index) const;
	
private:
	inline void AddEdge(unsigned int a, unsigned b, WingedTriangle * triangle, int index);
	inline void RemoveEdge(unsigned int a, unsigned b);
	bool FindEdge(unsigned int a, unsigned int b, Edge & rtn_edge);	
};



inline unsigned int WingedTriangleMesh::GetSize() const
{
	return triangles.size();
}


inline unsigned int WingedTriangleMesh::GetIBufferSize() const
{
	return triangles.size() * 3;
}

inline void WingedTriangleMesh::AddEdge(unsigned int a, unsigned b, WingedTriangle * triangle, int index)
{
	edge_table[a].push_back(std::make_pair(b, Edge(triangle, index)));
}

inline void WingedTriangleMesh::RemoveEdge(unsigned int a, unsigned b)
{
	for (unsigned int i = 0; i < edge_table[a].size(); ++i)
	{
		if(edge_table[a][i].first == b)
		{
			if (i != edge_table[a].size()-1)
			{
				edge_table[a][i] = edge_table[a].back();
			}
			edge_table[a].pop_back();
			return;
		}
	}
	//Could not find.
	throw 1;
}

inline WingedTriangle * WingedTriangleMesh::operator[] (unsigned int index)
{
	return triangles[index];
}

inline const WingedTriangle * WingedTriangleMesh::operator[] (unsigned int index) const
{
	return triangles[index];
}

#endif