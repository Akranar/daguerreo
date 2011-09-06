#include "DescVertexBuffer.h"

void DescVertexBuffer::Serialize(Stream & stream) const
{
	stream.Save(Usage);
	stream.Save(ByteCount);
	stream.Save(ByteStride);
	unsigned int init = 0;
	stream.Save(init);
}
void DescVertexBuffer::Unserialize(Stream & stream)
{
	stream.Load(Usage);
	stream.Load(ByteCount);
	stream.Load(ByteStride);
	unsigned int init = 0;
	stream.Load(init);
	InitialData = (void *) init;
}
void DescVertexBuffer::SerializeTypeId(Stream & stream) const
{
	stream.Save("DescVertexBuffer");
}