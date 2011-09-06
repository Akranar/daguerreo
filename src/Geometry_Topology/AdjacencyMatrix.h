#ifndef ADJACENCY_MATRIX_H_
#define ADJACENCY_MATRIX_H_

#include "AdjacencyData.h"
#include <vector>

#include <Core_Bitfield.h>

class AdjacencyMatrix : public AdjacencyData
{
	class Iterator;
	
	unsigned int point_count;
	//PackedBits * data;
	std::vector<Bitfield<unsigned int> > data;
	void GetBitLocation(unsigned int a, unsigned int b, unsigned int & PackedBits_index, unsigned int & bit_index) const;;
public:
	AdjacencyMatrix();
	AdjacencyMatrix(unsigned int n);
	virtual ~AdjacencyMatrix();
	int EdgeExists(unsigned int a, unsigned int b) const;
	void AddEdge(unsigned int a, unsigned int b);
	void DeleteEdge(unsigned int a, unsigned int b);
	void ToggleEdge(unsigned int a, unsigned int b);
	void Reserve(unsigned int n);
	inline unsigned int GetPointCount() const;
	RefCountPointer<AdjacencyData::Iterator> GetIterator() const;
};

inline unsigned int AdjacencyMatrix::GetPointCount() const
{
	return point_count;
}

class AdjacencyMatrix::Iterator: public AdjacencyData::Iterator
{
	bool has_more;
	std::pair<unsigned int, unsigned int> current_edge;
	const AdjacencyMatrix * parent;
public:
	Iterator(const AdjacencyMatrix * parent);
	~Iterator();
	bool HasMore();
	const std::pair<unsigned int, unsigned int> & GetEdge() const;
	void Next();

};


#endif