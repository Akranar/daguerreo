#ifndef _OBJECT_FACTORY_H
#define _OBJECT_FACTORY_H

#include <map>
#include <string>

#ifdef _DEBUG
#include <iostream>
#endif


template <typename Base>
class ObjectCreatorBase;

/*
	Object Factory
*/

template <typename Base>
class ObjectFactory
{
	typedef std::map<const std::string, const ObjectCreatorBase<Base> * > NAME_CREATOR_MAP;
	NAME_CREATOR_MAP type_registry;
public:
	ObjectFactory();
	virtual ~ObjectFactory();
	Base * Create(const char * name, unsigned int count = 1) const;
	bool Register(const char * name, ObjectCreatorBase<Base> * creator);
	
};

template <typename Base>
ObjectFactory<Base>::ObjectFactory()
{

}

template <typename Base>
ObjectFactory<Base>::~ObjectFactory()
{
	typename NAME_CREATOR_MAP::iterator it = type_registry.begin();

	for (;it != type_registry.end(); ++it)
	{
		delete it->second;
	}
	type_registry.clear();

}

template <typename Base>
Base * ObjectFactory<Base>::Create(const char * name, unsigned int count) const
{
	typename NAME_CREATOR_MAP::const_iterator it = type_registry.find(name);
	if (it != type_registry.end())
	{
		return it->second->Create(count);
	}
	//Assert(false, "Type not registered");
#ifdef _DEBUG
	std::cout << "Type not registered" << std::endl;
	throw 1;
#endif
	return 0;
}

template <typename Base>
bool ObjectFactory<Base>::Register(const char * name, ObjectCreatorBase<Base> * creator)
{
	type_registry[name] = creator;
	return true;
}

/*
	ObjectCreator
*/

template<typename Base>
class ObjectCreatorBase
{
public:
	virtual ~ObjectCreatorBase() {}
	virtual Base * Create(unsigned int count = 1) const = 0;
};

template<typename Object, typename Base>
class ObjectCreator : public ObjectCreatorBase<Base>
{
public:
	virtual ~ObjectCreator() {}
	Base * Create(unsigned int count = 1) const;
};

template<typename Object, typename Base>
Base * ObjectCreator<Object, Base>::Create(unsigned int count) const
{
	if (count == 1)
	{
		return new Object();
	}
	else
	{
		return new Object[count];
	}
}



#endif
