#ifndef _DESC_VISUAL_H_
#define _DESC_VISUAL_H_

#include <string>
#include "ExLib_Serialization.h"

class BoundingVolume;

class DescVisual : public Serializable
{
	std::string vertex_buffer_id;
	std::string index_buffer_id;
	std::string visual_fx_instance_id;
	BoundingVolume * bound;
public:
	DescVisual() : bound(0) {};
	~DescVisual();

	inline const std::string & GetVertexBufferID() const;
	inline const std::string & GetIndexBufferID() const;
	inline const std::string & GetVisualFXInstanceID() const;
	inline BoundingVolume * GetBoundingVolume() const;

	inline void SetVertexBufferID(const std::string & id);
	inline void SetIndexBufferID(const std::string & id);
	inline void SetVisualFXInstanceID(const std::string & id);
	inline void SetBoundingVolume(BoundingVolume * _bound);

	inline BoundingVolume * ReliquishBoundOwnership();

	void CreateBoundingVolume(unsigned int type, unsigned char * position_data, unsigned int vertex_stride, unsigned int vertex_count);

	//Serialization
	virtual void Serialize(Stream & stream) const;
	virtual void SerializeTypeId(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
};

inline const std::string & DescVisual::GetVertexBufferID() const
{
	return vertex_buffer_id;
}
inline const std::string & DescVisual::GetIndexBufferID() const
{
	return index_buffer_id;
}
inline const std::string & DescVisual::GetVisualFXInstanceID() const
{
	return visual_fx_instance_id;
}
inline BoundingVolume * DescVisual::GetBoundingVolume() const
{
	return bound;
}

inline void DescVisual::SetVertexBufferID(const std::string & id)
{
	vertex_buffer_id = id;
}
inline void DescVisual::SetIndexBufferID(const std::string & id)
{
	index_buffer_id = id;
}
inline void DescVisual::SetVisualFXInstanceID(const std::string & id)
{
	visual_fx_instance_id = id;
}
inline void DescVisual::SetBoundingVolume(BoundingVolume * _bound)
{
	bound = _bound;
}

inline BoundingVolume * DescVisual::ReliquishBoundOwnership()
{
	BoundingVolume * rtn = bound;
	bound = 0;
	return rtn;
}

#endif