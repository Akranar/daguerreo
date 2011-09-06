#include "AdjacencyMatrix.h"
#include <Core_BitField.h>

#include <cmath>

/*
	============================
		AdjacencyMatrix Notes
	============================

	Stores only lower-triangle of matrix.
		-> Row, B:
			-> range(0, point_count);
		-> Column, A:
			-> range(0, Row);
		-> For every row, B, we save (point_count - B) number of bits.
			-> Memory used up to row B = ((B+1)*(B+2)) / 2
				-> Instead of (B+1) * point_count;
		-> Each bit
			-> 1 : there is an edge.
			-> 0 : No edge.
	Calculating Access Location (row = B, column = A):
		-> First, calculate the location in the array for (row = B, column = 0): 
				-> and we store each edge as a full integer (instead of just a bit in the integer).
			-> index = B*(B+1) / 2;
		-> Next, add in A
			-> index += A
		-> Convert to bit location:
			-> i = index / bits_per_element;
			-> j = index % bits_per_element;
		-> Access the PackedBits struct at index "i".
		-> Access the bit within the PackedBits element, at "j".
*/

AdjacencyMatrix::AdjacencyMatrix()
{
	this->point_count = 0;
}

AdjacencyMatrix::AdjacencyMatrix(unsigned int n)
{
	unsigned int required_size = int(n*(n+1)*0.5f);
	required_size = (required_size % Bitfield<unsigned int>::BitCount() == 0 ? 0 : 1) + (required_size / Bitfield<unsigned int>::BitCount());
	data.reserve(required_size);
	while (required_size >= data.size())
	{
		data.push_back(Bitfield<unsigned int>());
	}
	this->point_count = (unsigned int) std::sqrt(float(data.size() * Bitfield<unsigned int>::BitCount() * 2));
}
AdjacencyMatrix::~AdjacencyMatrix()
{
}

int AdjacencyMatrix::EdgeExists(unsigned int a, unsigned int b) const
{
	unsigned int location, bit_index;
	GetBitLocation(a, b, location, bit_index);
	if (location >= data.size()) return 0;
	return data[location][bit_index];
}

void AdjacencyMatrix::AddEdge(unsigned int a, unsigned int b)
{
	unsigned int location, bit_index;
	GetBitLocation(a, b, location, bit_index);
	while (location >= data.size())
	{
		data.push_back(Bitfield<unsigned int>());
	}
	this->point_count = (unsigned int) std::sqrt(float(data.size() * Bitfield<unsigned int>::BitCount() * 2));
	data[location].SetBit(bit_index, true);
}

void AdjacencyMatrix::ToggleEdge(unsigned int a, unsigned int b)
{
	unsigned int location, bit_index;
	GetBitLocation(a, b, location, bit_index);
	while (location >= data.size())
	{
		data.push_back(Bitfield<unsigned int>());
	}
	this->point_count = (unsigned int)(   std::sqrt(   float(data.size() * Bitfield<unsigned int>::BitCount() * 2)   )   );
	data[location].ToggleBit(bit_index);
}



void AdjacencyMatrix::DeleteEdge(unsigned int a, unsigned int b)
{
	unsigned int location, bit_index;
	GetBitLocation(a, b, location, bit_index);
	if (location >= data.size()) return;
	data[location].SetBit(bit_index, false);
}

RefCountPointer<AdjacencyData::Iterator> AdjacencyMatrix::GetIterator() const
{
	return RefCountPointer<AdjacencyData::Iterator>(new AdjacencyMatrix::Iterator(this));
}


void AdjacencyMatrix::GetBitLocation(unsigned int a, unsigned int b, unsigned int & PackedBits_index, unsigned int & bit_index) const
{
	Order(a, b);

	PackedBits_index = int(b * (b+1) * 0.5f) + a;
	bit_index = PackedBits_index % Bitfield<unsigned int>::BitCount();
	PackedBits_index /= Bitfield<unsigned int>::BitCount();
}

void AdjacencyMatrix::Reserve(unsigned int n)
{
	int required_size = int(n*(n+1)*0.5f);
	required_size = (required_size % Bitfield<unsigned int>::BitCount() == 0 ? 0 : 1) + (required_size / Bitfield<unsigned int>::BitCount());
	data.reserve(required_size);
}



AdjacencyMatrix::Iterator::Iterator(const AdjacencyMatrix * parent)
: parent(parent), has_more(true)
{
	current_edge.first = 0;
	current_edge.second = 0;
	while (!parent->EdgeExists(current_edge.second, current_edge.first) && HasMore())
	{
		current_edge.first += 1;
		if (current_edge.first > current_edge.second)
		{
			current_edge.first = 0;
			current_edge.second += 1;
		}
	}
	if (!HasMore()) has_more = false;
}
AdjacencyMatrix::Iterator::~Iterator()
{

}
bool AdjacencyMatrix::Iterator::HasMore()
{
	return has_more && (current_edge.first < parent->GetPointCount() || current_edge.second < parent->GetPointCount());
}
const std::pair<unsigned int, unsigned int> & AdjacencyMatrix::Iterator::GetEdge() const
{
	return current_edge;
}
void AdjacencyMatrix::Iterator::Next()
{
	if (HasMore())
	{
		current_edge.first += 1;
		if (current_edge.first > current_edge.second)
		{
			current_edge.first = 0;
			current_edge.second += 1;
		}
		while (!parent->EdgeExists(current_edge.second, current_edge.first) && HasMore())
		{
			current_edge.first += 1;
			if (current_edge.first > current_edge.second)
			{
				current_edge.first = 0;
				current_edge.second += 1;
			}
		}
		if (!HasMore()) has_more = false;
	}
}