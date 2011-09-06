#include "WingedTriangleMesh.h"
#include "WingedTriangle.h"

WingedTriangleMesh::WingedTriangleMesh()
{

}

WingedTriangleMesh::WingedTriangleMesh(unsigned int vertex_capacity)
{
	SetVertexCapacity(vertex_capacity);
}

WingedTriangleMesh::~WingedTriangleMesh()
{
	WingedTriSet::iterator iter = triangles.begin();
	for(;iter != triangles.end(); ++iter)
	{
		delete (*iter);
	}
	triangles.clear();
	edge_table.clear();
}


void WingedTriangleMesh::SetVertexCapacity(unsigned int n)
{
	edge_table.reserve(n);
	while (edge_table.size() < n)
		edge_table.push_back(std::vector<std::pair<unsigned int, WingedTriangleMesh::Edge> >());
}

void WingedTriangleMesh::ReadIBuffer(int * ibuffer, unsigned ibuffer_size)
{
	if (ibuffer_size % 3 != 0) throw 1;
	for (unsigned int i = 0; i < ibuffer_size; i += 3)
	{
		AddTriangle(ibuffer[i], ibuffer[i+1], ibuffer[i+2]);
	}
}

void WingedTriangleMesh::AddTriangle(unsigned int a, unsigned int b, unsigned int c)
{
	Edge edge0;
	Edge edge1;
	Edge edge2;

	bool found0 = FindEdge(b, a, edge0);
	bool found1 = FindEdge(c, b, edge1);
	bool found2 = FindEdge(a, c, edge2);
	
	//WingedTriangle * to_add = &(*insert_result.first);
	WingedTriangle * to_add = new WingedTriangle(a, b, c);
	to_add->SetIndexInMesh(triangles.size());
	//triangles.insert(to_add);
	triangles.push_back(to_add);
	AddEdge(a, b, to_add, 0);
	AddEdge(b, c, to_add, 1);
	AddEdge(c, a, to_add, 2);

	if (found0)
	{
		to_add->neighbors[0] = edge0.triangle;
		edge0.triangle->neighbors[edge0.index] = to_add;
		to_add->index_in_neighbors.SetByte(0, edge0.index);
		edge0.triangle->index_in_neighbors.SetByte(edge0.index, 0);
		
	}

	if (found1)
	{
		to_add->neighbors[1] = edge1.triangle;
		edge1.triangle->neighbors[edge1.index] = to_add;
		to_add->index_in_neighbors.SetByte(1, edge1.index);
		edge1.triangle->index_in_neighbors.SetByte(edge1.index, 1);
	}

	if (found2)
	{
		to_add->neighbors[2] = edge2.triangle;
		edge2.triangle->neighbors[edge2.index] = to_add;
		to_add->index_in_neighbors.SetByte(2, edge2.index);
		edge2.triangle->index_in_neighbors.SetByte(edge2.index, 2);
	}

}

void WingedTriangleMesh::RemoveTriangle(WingedTriangle * triangle)
{
	RemoveTriangle(triangle->GetIndexInMesh());
}

void WingedTriangleMesh::RemoveTriangle(int index)
{
	WingedTriangle * current = triangles[index];

	//Find In Edge Table and Delete
	RemoveEdge(current->GetEdgeSrc(0), current->GetEdgeDest(0));
	RemoveEdge(current->GetEdgeSrc(1), current->GetEdgeDest(1));
	RemoveEdge(current->GetEdgeSrc(2), current->GetEdgeDest(2));

	//Remove and Set Neighbors
	WingedTriangle * neighbor0 = current->neighbors[0];
	WingedTriangle * neighbor1 = current->neighbors[1];
	WingedTriangle * neighbor2 = current->neighbors[2];

	if(neighbor0) 
	{
		neighbor0->neighbors[current->index_in_neighbors[0]] = 0;
	}

	if(neighbor1) 
	{
		neighbor1->neighbors[current->index_in_neighbors[1]] = 0;
	}
	if(neighbor2) 
	{
		neighbor2->neighbors[current->index_in_neighbors[2]] = 0;
	}

	if (index != triangles.size()-1)
	{
		triangles[index] = triangles.back();
		triangles[index]->SetIndexInMesh(index);
	}
	triangles.pop_back();

	delete current;
}

bool WingedTriangleMesh::FindEdge(unsigned int a, unsigned int b, Edge & rtn_edge)
{
	
	std::vector<std::pair<unsigned int, Edge> >::iterator it = edge_table[a].begin();
	for (;it != edge_table[a].end(); ++it)
	{
		if (it->first == b) 
		{
			rtn_edge = it->second;
			return true;
		}
	}
	return false;
	
}

void WingedTriangleMesh::GetIndexBuffer(int * rtn_buffer) const
{
	int * ptr = rtn_buffer;
	WingedTriSet::const_iterator iter = triangles.begin();
	for(;iter != triangles.end(); ++iter, ptr += 3)
	{
		ptr[0] = (*iter)->vertices[0];
		ptr[1] = (*iter)->vertices[1];
		ptr[2] = (*iter)->vertices[2];
	}
}

void WingedTriangleMesh::PrintContent() const
{
	WingedTriSet::const_iterator iter = triangles.begin();
	for(;iter != triangles.end(); ++iter)
	{
		(*iter)->Print();
	}
}

void WingedTriangleMesh::ClearEdgeTable()
{
	edge_table.clear();
}