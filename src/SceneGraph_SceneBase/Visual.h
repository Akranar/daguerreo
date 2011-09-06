#ifndef VISUAL_H_
#define VISUAL_H_

#include <vector>
#include "Spatial.h"

class VertexBuffer;
class IndexBuffer;
class BoundingVolume;
class VisualFXInstance;

class Visual: public Spatial
{
protected:
	VertexBuffer * vertex_buffer;
	IndexBuffer * index_buffer;
	BoundingVolume * model_bound;
	VisualFXInstance * fx_instance;
public:
	Visual();
	Visual(VertexBuffer * v_buffer, IndexBuffer * i_buffer);
	virtual ~Visual();
	inline VertexBuffer * GetVertexBuffer() const;
	inline void SetVertexBuffer(VertexBuffer * _vertex_buffer);
	inline IndexBuffer * GetIndexBuffer() const;
	inline void SetIndexBuffer(IndexBuffer * index_buffer);
	inline BoundingVolume * GetModelBound() const;
	inline void SetModelBound(BoundingVolume * bound);
	
	inline VisualFXInstance * GetFX() const;
	inline void SetFX(VisualFXInstance * effect);
	
	void CreateWorldFromModelBound();
	virtual void UpdateWorldTransform();
	virtual void UpdateWorldBound();
	virtual void UpdateVisibleSet(Culler * culler, bool culling_on);

	//Serialization
	
	virtual void Serialize(Stream & stream) const;
	virtual void SerializeTypeId(Stream & stream) const;
	virtual void Unserialize(Stream & stream);
	
};


inline VertexBuffer * Visual::GetVertexBuffer() const
{
	return vertex_buffer;
}
inline void Visual::SetVertexBuffer(VertexBuffer * _vertex_buffer)
{
	vertex_buffer = _vertex_buffer;
}
inline IndexBuffer * Visual::GetIndexBuffer() const
{
	return index_buffer;
}
inline void Visual::SetIndexBuffer(IndexBuffer * _index_buffer)
{
	index_buffer = _index_buffer;
}
inline BoundingVolume * Visual::GetModelBound() const
{
	return model_bound;
}
inline void Visual::SetModelBound(BoundingVolume * bound)
{
	model_bound = bound;
}
inline VisualFXInstance * Visual::GetFX() const
{
	return fx_instance;
}
inline void Visual::SetFX(VisualFXInstance * effect)
{
	fx_instance = effect;
}



#endif