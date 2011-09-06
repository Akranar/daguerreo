#include "Core_RTTI.h"
#include <stdarg.h>

RTTI::RTTI(const std::string & class_name) :
    class_name(class_name)
{
    parent_count = 0;    
}

RTTI::RTTI(const std::string & class_name, const RTTI & parent_rtti) :
    class_name(class_name), parent_count(1)
{
    parents = new const RTTI*[parent_count];
	parents[0] = &parent_rtti;
}

RTTI::RTTI(const std::string & class_name, int parent_count, ...) :
    class_name(class_name)
{
    this->parent_count = parent_count;
    if (parent_count < 1)
    {
        this->parent_count = 0;
        return;
    }
    else
    {
        parents = new const RTTI*[parent_count];

        va_list v;
        va_start(v, parent_count);
        for (unsigned int index = 0; index < this->parent_count; ++index)
        {
            parents[index] = va_arg(v, RTTI*);
        }
        va_end(v);
    }

}

bool RTTI::IsExactly(const RTTI & to_compare) const
{
    return this == &to_compare;
}

bool RTTI::IsDerivedFrom(const RTTI & to_compare) const
{
    if (this == &to_compare)
    {
        return true;
    }
    for (unsigned int index_to_parent = 0; index_to_parent < parent_count; ++index_to_parent)
    {
        if (this->parents[index_to_parent]->IsDerivedFrom(to_compare)) return true;
    }

    return false;    
}
