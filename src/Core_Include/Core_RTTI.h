#ifndef SKYLINE_RTTI_H
#define SKYLINE_RTTI_H

#include <string>


class RTTI
{
    const RTTI ** parents;
    unsigned int parent_count;
    std::string class_name;
public:
    RTTI(const std::string & class_name);
    RTTI(const std::string & class_name, const RTTI & parent_rtti);
    RTTI(const std::string & class_name, int parent_count, ...);
    bool IsExactly(const RTTI & to_compare) const;
    bool IsDerivedFrom(const RTTI & to_compare) const;
};


#define RTTI_DECLARATION \
    public: \
        static const RTTI rtti; \
        virtual const RTTI & GetRTTI() const \
        { \
            return rtti; \
        }; 

#define RTTI_DECLARATION_ABSTRACT \
    public: \
        static const RTTI rtti; \
        virtual const RTTI & GetRTTI() const = 0;

#define RTTI_IMPL_ROOT(name) \
    const RTTI name::rtti(#name); 

#define RTTI_IMPL_SINGLE_INHERITANCE(name, parent) \
    const RTTI name::rtti(#name, parent::rtti);

#endif
