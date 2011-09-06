#ifndef _SERIALIZABLE_H_
#define _SERIALIZABLE_H_

class Stream;

class Serializable
{
public:
	virtual ~Serializable() {};
	virtual void Serialize(Stream & stream) const = 0;
	virtual void SerializeTypeId(Stream & stream) const = 0;
	virtual void Unserialize(Stream & stream) = 0;
};



#endif