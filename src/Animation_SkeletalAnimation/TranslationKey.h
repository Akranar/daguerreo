#ifndef _TRANSLATE_KEY_H_
#define _TRANSLATE_KEY_H_

#include "ExLib_Eigen.h"
#include "ExLib_Serialization.h"

class TranslationKey//: public Serializable
{
protected:
	double time;
	Eigen::Vector3f translation;
public:
	TranslationKey() {};
	TranslationKey(double time, const Eigen::Vector3f & translation)
		:
		time(time), 
		translation(translation)
	{
	};
	inline double GetTime() const;
	inline const Eigen::Vector3f & GetTranslation() const;

	inline void SetTime(double in);
	inline void SetTranslation(Eigen::Vector3f & in);
	inline void SetTranslation(const float * in);

	//Serialization
	/*
	virtual void Serialize(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	virtual void SerializeTypeId(Stream & stream) const;
	*/
};

inline double TranslationKey::GetTime() const
{
	return time;
}

inline const Eigen::Vector3f & TranslationKey::GetTranslation() const
{
	return translation;
}

inline void TranslationKey::SetTime(double in)
{
	time = in;
}

inline void TranslationKey::SetTranslation(Eigen::Vector3f & in)
{
	translation = in;
}

inline void TranslationKey::SetTranslation(const float * in)
{
	translation[0] = in[0];
	translation[1] = in[1];
	translation[2] = in[2];
}

#endif