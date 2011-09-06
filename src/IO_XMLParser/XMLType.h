#ifndef _XMLTYPE_H_
#define _XMLTYPE_H_

class XMLType
{
	std::string name;
public:
	XMLType(const std::string & name) : name(name) {};
	const std::string & GetName() const
	{
		return name;
	}
	bool IsEqual(const XMLType & type) const
	{
		return this == &type;
	}
};

#define ELEMENT_TYPE_DECL \
	public: \
		static const XMLType type; \
		virtual const XMLType & GetType() const { return type; };

#define ELEMENT_TYPE_IMPL(name) \
	const XMLType name::type(#name);

#endif