#ifndef ADJACENCY_LIST_H_
#define ADJACENCY_LIST_H_

#include "AdjacencyData.h"

#include <map>
#include <set>

class AdjacencyList : public AdjacencyData
{
	class Iterator;

	friend class AdjacencyList::Iterator;
	mutable std::map<unsigned int, std::set<unsigned int> > data;
public:
	AdjacencyList();
	virtual ~AdjacencyList();
	int EdgeExists(unsigned int a, unsigned int b) const;
	void AddEdge(unsigned int a, unsigned int b);
	void DeleteEdge(unsigned int a, unsigned int b);
	void ToggleEdge(unsigned int a, unsigned int b);
	RefCountPointer<AdjacencyData::Iterator> GetIterator() const;

};


class AdjacencyList::Iterator: public AdjacencyData::Iterator
{
	bool has_more;
	std::pair<unsigned int, unsigned int> current_edge;
	std::map<unsigned int, std::set<unsigned int> >::iterator it_a;
	std::set<unsigned int>::iterator it_b;
	const AdjacencyList * parent;
public:
	Iterator(const AdjacencyList * parent);
	~Iterator();
	bool HasMore();
	const std::pair<unsigned int, unsigned int> & GetEdge() const;
	void Next();
};


#endif