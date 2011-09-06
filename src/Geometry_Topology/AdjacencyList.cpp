#include "AdjacencyList.h"


AdjacencyList::AdjacencyList()
{

}
AdjacencyList::~AdjacencyList()
{

}

int AdjacencyList::EdgeExists(unsigned int a, unsigned int b) const
{
	Order(a, b);
	std::map<unsigned int, std::set<unsigned int> >::iterator it = data.find(a);
	if (it == data.end()) return 0;
	std::set<unsigned int>::const_iterator it2 = it->second.find(b);
	return (it2 != it->second.end());
}
void AdjacencyList::AddEdge(unsigned int a, unsigned int b)
{
	Order(a, b);
	std::map<unsigned int, std::set<unsigned int> >::iterator it = data.find(a);
	if (it == data.end())
	{
		std::set<unsigned int> set_a;
		set_a.insert(b);
		data[a] = set_a;
		return;
	}
	it->second.insert(b);
}

void AdjacencyList::DeleteEdge(unsigned int a, unsigned int b)
{
	Order(a, b);
	std::map<unsigned int, std::set<unsigned int> >::iterator it = data.find(a);
	if (it == data.end())
	{
		return;
	}
	else
	{
		std::set<unsigned int>::const_iterator it2 = it->second.find(b);
		if (it2 != it->second.end())
		{
			it->second.erase(it2);
		}
	}
}

void AdjacencyList::ToggleEdge(unsigned int a, unsigned int b)
{
	Order(a, b);
	std::map<unsigned int, std::set<unsigned int> >::iterator it = data.find(a);
	if (it == data.end())
	{
		std::set<unsigned int> set_a;
		set_a.insert(b);
		data[a] = set_a;
		return;
	}
	else
	{
		std::set<unsigned int>::const_iterator it2 = it->second.find(b);
		if (it2 != it->second.end())
		{
			it->second.erase(it2);
		}
		else
		{
			it->second.insert(b);
		}
	}
	
}

RefCountPointer<AdjacencyData::Iterator> AdjacencyList::GetIterator() const
{
	return RefCountPointer<AdjacencyData::Iterator>(new AdjacencyList::Iterator(this));
}

AdjacencyList::Iterator::Iterator(const AdjacencyList * parent)
: parent(parent), has_more(true)
{
	it_a = parent->data.begin();
	if (it_a == parent->data.end())
	{
		has_more = false;
		return;
	}
	it_b = it_a->second.begin();
	if (it_b == it_a->second.end())
	{
		has_more = false;
		return;
	}
	current_edge.first = it_a->first;
	current_edge.second = *it_b;
	
}
AdjacencyList::Iterator::~Iterator()
{

}
bool AdjacencyList::Iterator::HasMore()
{
	return has_more && (it_a != parent->data.end());
}
const std::pair<unsigned int, unsigned int> & AdjacencyList::Iterator::GetEdge() const
{
	return current_edge;
}
void AdjacencyList::Iterator::Next()
{
	if (HasMore())
	{
		++it_b;
		if (it_b == it_a->second.end())
		{
			++it_a;
			if (it_a == parent->data.end())
			{
				has_more = false;
				return;
			}
			it_b = it_a->second.begin();
		}
		current_edge.first = it_a->first;
		current_edge.second = *it_b;
	}
}
