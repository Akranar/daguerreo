#ifndef ADJACENCY_DATA_H_
#define ADJACENCY_DATA_H_

#include <vector>
#include <Core_RefCountPointer.h>

class EdgeIterator;

class AdjacencyData
{
public:

	class Iterator
	{
	public:
		Iterator() {};
		virtual ~Iterator() {};
		virtual bool HasMore() = 0;
		virtual const std::pair<unsigned int, unsigned int> & GetEdge() const = 0;
		virtual void Next() = 0;
	};

	AdjacencyData() {};
	virtual ~AdjacencyData() {};
	virtual int EdgeExists(unsigned int a, unsigned int b) const = 0;
	virtual void AddEdge(unsigned int a, unsigned int b) = 0;
	virtual void DeleteEdge(unsigned int a, unsigned int b) = 0;
	virtual void ToggleEdge(unsigned int a, unsigned int b) = 0;
	virtual RefCountPointer<AdjacencyData::Iterator> GetIterator() const = 0;
	inline void Order(unsigned int & a, unsigned int & b) const;
};

inline void AdjacencyData::Order(unsigned int & a, unsigned int & b) const
{
	if (a > b)
	{
		int temp = b;
		b = a;
		a = temp;
	}
}

#endif