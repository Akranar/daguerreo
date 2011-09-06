#include "WingedTriangle.h"

#include <iostream>

WingedTriangle::WingedTriangle(unsigned int a, unsigned int b, unsigned int c)
{
	vertices[0] = a;
	vertices[1] = b;
	vertices[2] = c;

	neighbors[0] = 0;
	neighbors[1] = 0;
	neighbors[2] = 0;

	index_in_mesh = 0;
}

WingedTriangle::~WingedTriangle()
{
	vertices[0] = 0xDEAD;
	vertices[1] = 0xDEAD;
	vertices[2] = 0xDEAD;

	neighbors[0] = (WingedTriangle *) 0xDEAD;
	neighbors[1] = (WingedTriangle *) 0xDEAD;
	neighbors[2] = (WingedTriangle *) 0xDEAD;

	index_in_mesh = 0xDEAD;
}

void WingedTriangle::FlipEdge(int edge)
{
	/*
		Temporaries {
	*/
	WingedTriangle * neighbor = neighbors[edge];

	int edge1 = (edge+1)%3;
	int edge2 = (edge+2)%3;
	int edge_neighbor = index_in_neighbors[edge];
	int edge1_neighbor = (edge_neighbor+1)%3;
	int edge2_neighbor = (edge_neighbor+2)%3;
	
	unsigned int b = neighbor->vertices[edge2_neighbor];
	unsigned int d = vertices[edge2];
	WingedTriangle * BN = neighbor->neighbors[edge1_neighbor];
	WingedTriangle * DN = neighbors[edge1];
	int index_in_BN = neighbor->index_in_neighbors[edge1_neighbor];
	int index_in_DN = index_in_neighbors[edge1];

	/*
		Temporaries }
		Fix-ups In This and Opposite Triangle {
	*/

	//In This Triangle:
	vertices[edge1] = b;
	
	neighbors[edge1] = neighbor;
	index_in_neighbors.SetByte(edge1, edge1_neighbor);

	neighbors[edge] = BN;
	index_in_neighbors.SetByte(edge, index_in_BN);

	//In Opposite Triangle:
	neighbor->vertices[edge1_neighbor] = d;

	neighbor->neighbors[edge1_neighbor] = this;
	neighbor->index_in_neighbors.SetByte(edge1_neighbor, edge1);

	neighbor->neighbors[edge_neighbor] = DN;
	neighbor->index_in_neighbors.SetByte(edge_neighbor, index_in_DN);
	/*
		Fix-ups In This and Opposite Triangle }
		Fix-ups In Outer Neighbors {
	*/
	
	if (BN)
	{
		BN->neighbors[index_in_BN] = this;
		BN->index_in_neighbors.SetByte(index_in_BN, edge);
	}
	if (DN)
	{
		DN->neighbors[index_in_DN] = neighbor;
		DN->index_in_neighbors.SetByte(index_in_DN, edge_neighbor);
	}
	/*
		Fix-ups In Outer Neighbors }
	*/	
}

void WingedTriangle::Print() const
{
	std::cout << "WingedTriangle: " << vertices[0] << ", " << vertices[1] << ", " << vertices[2] << std::endl;
	std::cout << "              : " << neighbors[0] << ", " << neighbors[1] << ", " << neighbors[2] << std::endl;
	std::cout << "              : " << index_in_neighbors[0] << ", " << index_in_neighbors[1] << ", " << index_in_neighbors[2] << std::endl;
}

/*
void WingedTriangle::TraverseDepthFirst(std::stack<WingedTriangle *> & traversal_schedule, int prev_index) const
{
	for (unsigned int i = 0; i < 3; ++i)
	{
		if (i != prev_index)
		{
			if (GetNeighbor(i)) traversal_schedule.push(GetNeighbor(i));
		}
	}
}
*/

