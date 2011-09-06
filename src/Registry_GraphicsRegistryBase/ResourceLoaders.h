#ifndef _RESOURCE_LOADERS_H_
#define _RESOURCE_LOADERS_H_

#include "ExLib_BinaryFileStream.h"
#include "ExLib_Serialization.h"
#include "ExLib_RendererBase.h"
#include "ExLib_TextureLoader.h"
#include <Core_ObjectFactory.h>
#include <Core_Exception.h>

class VertexBuffer;
class IndexBuffer;
class Texture1D;
class Texture2D;
class Texture3D;
class TextureCube;
class ShaderProgram;

template <class BASE>
class ObjectFactory;

class Serializable;



//*********************************************
//	VertexBufferLoader
//*********************************************
class VertexBufferLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * resource_manager, VertexBuffer * resource)
	{
		DescVertexFormat vertex_format_desc;
		DescVertexBuffer vertex_buffer_desc;
		FixedArray<unsigned char> vertex_data;
		BinaryFileStream resource_stream;
		resource_stream.StartRead(filename, *factory);
		{
			resource_stream.Load(vertex_format_desc);
			resource_stream.Load(vertex_buffer_desc);
			STLSerializer<FixedArray<unsigned char> > vertex_data_serial(resource_stream);
			vertex_data_serial.Load(vertex_data);
		}
		resource_stream.Close();
		vertex_buffer_desc.InitialData = vertex_data.GetData();
		resource_manager->GetRenderer()->CreateResource(vertex_buffer_desc, resource);
		resource->format_hash = resource_manager->GetRenderer()->GetVertexFormatHash(vertex_format_desc);
		CONDITIONAL_EXCEPTION(resource->PRIVATE == 0, "Registry: Error Loading Vertex Buffer.");
	}
	template <class MANAGER_TYPE>
	void Unload(VertexBuffer * resource, MANAGER_TYPE * resource_manager)
	{
		resource_manager->GetRenderer()->Delete(resource);
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};

//*********************************************
//	IndexBufferLoader
//*********************************************
class IndexBufferLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * resource_manager, IndexBuffer * resource)
	{
		DescIndexBuffer index_buffer_desc;
		FixedArray<unsigned char> index_data;
		BinaryFileStream resource_stream;
		resource_stream.StartRead(filename, *factory);
		{
			resource_stream.Load(index_buffer_desc);
			STLSerializer<FixedArray<unsigned char> > index_data_serial(resource_stream);
			index_data_serial.Load(index_data);
		}
		resource_stream.Close();
		index_buffer_desc.InitialData = index_data.GetData();
		resource_manager->GetRenderer()->CreateResource(index_buffer_desc, resource);
		CONDITIONAL_EXCEPTION(resource->PRIVATE == 0, "Registry: Error Loading Index Buffer.");
	}
	template <class MANAGER_TYPE>
	void Unload(IndexBuffer * resource, MANAGER_TYPE * resource_manager)
	{
		resource_manager->GetRenderer()->Delete(resource);
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};

//*********************************************
//	Texture1DLoader
//*********************************************

class Texture1DLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	Texture1D * Load(const char * filename, MANAGER_TYPE * resource_manager)
	{
		return 0;
	}
	template <class MANAGER_TYPE>
	void Unload(Texture1D * resource, MANAGER_TYPE * resource_manager)
	{
		
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};


//*********************************************
//	Texture2DLoader
//*********************************************

class Texture2DLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * resource_manager, Texture2D * resource)
	{
		DescTexture2D texdesc;
		texdesc.BindFlags = BIND_SHADER_RESOURCE;
		texdesc.MipLevels = 0;
		texdesc.Usage = TEXTURE_USAGE_WRITE_ONLY;
		FixedArray<unsigned char> texdata;
		TextureLoader::LoadTexture2D(filename, texdesc, texdata);
		resource_manager->GetRenderer()->CreateResource(texdesc, resource);
		CONDITIONAL_EXCEPTION(resource->PRIVATE == 0, "Registry: Error Loading Texture2D.");
	}
	template <class MANAGER_TYPE>
	void Unload(Texture2D * resource, MANAGER_TYPE * resource_manager)
	{
		resource_manager->GetRenderer()->Delete(resource);
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};

//*********************************************
//	Texture3DLoader
//*********************************************

class Texture3DLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	Texture3D * Load(const char * filename, MANAGER_TYPE * resource_manager)
	{
		return 0;
	}
	template <class MANAGER_TYPE>
	void Unload(Texture3D * resource, MANAGER_TYPE * resource_manager)
	{
		//if (resource->IsBound()) resource_manager->Unbind(resource);
		//delete resource;
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};


//*********************************************
//	TextureCubeLoader
//*********************************************

class TextureCubeLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * resource_manager, TextureCube * resource)
	{
		std::string file = filename;
		int index = file.find(".");
		int index_1 = index+1;
		std::string std_name = file.substr(0, index);
		std::string std_extension = file.substr(index_1, file.length()-index_1);

		std::string positive_x = std_name + "_positive_x" + '.' + std_extension;
		std::string negative_x = std_name + "_negative_x" + '.' + std_extension;
		std::string positive_y = std_name + "_positive_y" + '.' + std_extension;
		std::string negative_y = std_name + "_negative_y" + '.' + std_extension;
		std::string positive_z = std_name + "_positive_z" + '.' + std_extension;
		std::string negative_z = std_name + "_negative_z" + '.' + std_extension;


		const char * filenames[6];
		filenames[0] = positive_x.c_str();
		filenames[1] = negative_x.c_str();
		filenames[2] = positive_y.c_str();
		filenames[3] = negative_y.c_str();
		filenames[4] = positive_z.c_str();
		filenames[5] = negative_z.c_str();

		DescTextureCube texdesc;
		texdesc.BindFlags = BIND_SHADER_RESOURCE;
		texdesc.MipLevels = 0;
		texdesc.Usage = TEXTURE_USAGE_WRITE_ONLY;
		
		FixedArray<unsigned char> texdata;
		TextureLoader::LoadTextureCube(filenames, texdesc, texdata);
		resource_manager->GetRenderer()->CreateResource(texdesc, resource);
		CONDITIONAL_EXCEPTION(resource->PRIVATE == 0, "Registry: Error Loading TextureCube.");
	}
	template <class MANAGER_TYPE>
	void Unload(TextureCube * resource, MANAGER_TYPE * resource_manager)
	{
		resource_manager->GetRenderer()->Delete(resource);
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};

//*********************************************
//	ShaderProgramLoader
//*********************************************

class ShaderProgramLoader
{
	ObjectFactory<Serializable> * factory;
public:
	template <class MANAGER_TYPE>
	void Load(const char * filename, MANAGER_TYPE * resource_manager, ShaderProgram * resource)
	{
		std::string string_filename = filename;
		int divide = string_filename.find('|');
		CONDITIONAL_EXCEPTION(divide < 0, "Registry: Error Loading ShaderProgram. Didn't find '|' Character.");
		std::string first = string_filename.substr(0, divide);
		std::string second = string_filename.substr(divide + 1, string_filename.length() - divide);
		DescShaderProgram desc;
		desc.ReadFiles(first.c_str(), second.c_str());
		resource_manager->GetRenderer()->CreateResource(desc, resource);
		CONDITIONAL_EXCEPTION(resource->PRIVATE == 0, "Registry: Error Loading ShaderProgram.");
	}
	template <class MANAGER_TYPE>
	void Unload(ShaderProgram * resource, MANAGER_TYPE * resource_manager)
	{
		resource_manager->GetRenderer()->Delete(resource);
	}
	inline void SetFactory(ObjectFactory<Serializable> * in)
	{
		factory = in;
	}
};




#endif