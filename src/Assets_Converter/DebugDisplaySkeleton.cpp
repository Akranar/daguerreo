#include "DebugDisplaySkeleton.h"
#include "ExLib_BoundingVolume.h"
#include "ExLib_FloatDataAccessor.h"
#include "ExLib_RendererBase.h"
#include "ExLib_Skeleton.h"

#include <iostream>

DebugDisplaySkeleton::DebugDisplaySkeletonInner::~DebugDisplaySkeletonInner()
{

}

DebugDisplaySkeleton::DebugDisplaySkeleton()
	: 
effect_instance_initialized(false),
renderer(0)
{
}

DebugDisplaySkeleton::~DebugDisplaySkeleton()
{
	skeleton_displays.clear();
}
void DebugDisplaySkeleton::Init(RendererBase * _renderer, Projector * _camera)
{
	renderer = _renderer;
	camera = _camera;
	DescShaderProgram shader_desc;
#ifdef USE_OPENGL
	shader_desc.ReadFiles("GLSL21/Default.glslv", "GLSL/Default.glslf");
#elif USE_DIRECTX10
	shader_desc.ReadFiles("HLSL/Default.vs4", "HLSL/Default.ps4");
#endif
	renderer->CreateResource(shader_desc, &shader_program);
	DescUniformBuffer uniform_desc;
	uniform_desc.index = 0;
	uniform_desc.byte_count = 64;
	uniform_desc.domain = DOMAIN_VERTEX;
	renderer->CreateResource(uniform_desc, &mvp);
	mvp_index = shader_program.FindUniform("ModelViewProjMatrix");

	DescDepthStencilState depth_desc;
	depth_desc.SetDefaults();
	depth_desc.DepthEnable = false;

	renderer->CreateState(depth_desc, &depth_stencil_state);
}
void DebugDisplaySkeleton::Update(unsigned int index)
{
	/*
	for (unsigned int i = 0; i < skeleton_displays.size(); ++i)
	{
		skeleton_displays[i]->skeleton->GetVertices(*skeleton_displays[i]->skeleton_positions);
		skeleton_displays[i]->skeleton_display_vertex_buffer->SyncData();
	}
	*/
	unsigned char * mapped = (unsigned char *) renderer->Map(&skeleton_displays[index].skeleton_display_vertex_buffer, MAP_WRITE_INVALIDATE_ALL);
	//for (unsigned int j = 0; j < skeleton_displays.size(); ++j)
	//{
		const unsigned int joint_count = skeleton_displays[index].skinned_mesh->GetJointCount();
		for (unsigned int i = 0; i < joint_count; ++i)
		{
			Eigen::Vector3f position = skeleton_displays[index].skinned_mesh->GetGlobalPose(i) * Eigen::Vector3f(0, 0, 0);
			//VEC3((*skeleton_displays[j]->skeleton_positions)[i]) = position;
			float * dest = (float *) (mapped + (i * skeleton_displays[index].skeleton_display_vertex_buffer.byte_stride));
			dest[0] = position[0];
			dest[1] = position[1];
			dest[2] = position[2];
		}
		//skeleton_displays[j]->skeleton_display_vertex_buffer->SyncData();
	//}
	renderer->Unmap(&skeleton_displays[index].skeleton_display_vertex_buffer);
}
void DebugDisplaySkeleton::PreDraw(unsigned int index)
{
	renderer->BindVertexBuffer(&skeleton_displays[index].skeleton_display_vertex_buffer);
	renderer->BindIndexBuffer(&skeleton_displays[index].skeleton_display_index_buffer);
	renderer->BindShaderProgram(&shader_program);

	//Calculate Matrices.
		Eigen::Matrix4f model = Eigen::Matrix4f::Identity() * 1.0f;
		model(3, 3) = 1.0f;
		Eigen::Matrix4f model_view_proj;
		camera->ViewProjMatrix(model_view_proj.data());
		model_view_proj = model * model_view_proj;

		{
			UniformBufferUpdater ub_update(renderer, &shader_program, &mvp);
			ub_update.Set(0, model_view_proj.data(), 64);
		}//Update Matrix
		renderer->BindUniformBuffer(&mvp, &shader_program);


	
	renderer->SetInputLayout(skeleton_displays[index].skeleton_display_vertex_buffer.format_hash, shader_program.signature_hash);

	renderer->SetState(&depth_stencil_state);
}
void DebugDisplaySkeleton::InitWithSkinnedMesh(SkinnedMesh * in_skeleton)
{

	unsigned int bone_count = in_skeleton->GetJointCount();

	skeleton_displays.push_back(DebugDisplaySkeletonInner());

	//Vertex Format
	DescVertexFormat skeleton_display_vertex_format;
	skeleton_display_vertex_format.Initialize(1);
	skeleton_display_vertex_format[0].SetName("POSITION");
	skeleton_display_vertex_format[0].SetChannels(3);
	skeleton_display_vertex_format[0].SetOffset(0);
	skeleton_display_vertex_format[0].SetSemanticIndex(0);
	skeleton_display_vertex_format[0].SetStreamIndex(0);
	skeleton_display_vertex_format[0].SetStride(sizeof(float) * 3);
	skeleton_display_vertex_format[0].SetType(DT_FLOAT);

	//Vertex Buffer
	DescVertexBuffer skeleton_display_vertex_buffer;
	skeleton_display_vertex_buffer.SetByteCount(sizeof(float) * 3 * bone_count);
	skeleton_display_vertex_buffer.SetStride(sizeof(float) * 3);
	skeleton_display_vertex_buffer.SetUsage(BUFFER_USAGE_STREAM);
	skeleton_display_vertex_buffer.SetInitialDataPointer(0);

	//Index Buffer
	DescIndexBuffer skeleton_display_index_buffer;
	skeleton_display_index_buffer.SetByteCount(sizeof(unsigned int) * 2 * bone_count);
	skeleton_display_index_buffer.SetPrimitiveType(PRIMITIVE_LINE_LIST);
	skeleton_display_index_buffer.SetUsage(BUFFER_USAGE_STREAM);
	

	FixedArray<unsigned char> index_data(sizeof(unsigned int) * 2 * bone_count);
	skeleton_display_index_buffer.SetInitialDataPointer(index_data.GetData());

	//Init Index Buffer values.
	unsigned int * i_buffer_data = (unsigned int *) index_data.GetData();
	i_buffer_data[0] = 0;
	i_buffer_data[1] = 0;
	for (unsigned int i = 1; i < bone_count; ++i)
	{
		i_buffer_data[i*2] = in_skeleton->GetSkeleton()->GetJoint(i).GetParentIndex();
		i_buffer_data[i*2+1] = i;
	}


	//Bind VertexBuffer
	renderer->CreateResource(skeleton_display_vertex_buffer, &skeleton_displays.back().skeleton_display_vertex_buffer);
	skeleton_displays.back().skeleton_display_vertex_buffer.format_hash = renderer->GetVertexFormatHash(skeleton_display_vertex_format);
	renderer->CreateResource(skeleton_display_index_buffer, &skeleton_displays.back().skeleton_display_index_buffer);
	skeleton_displays.back().skinned_mesh = in_skeleton;

	renderer->CreateInputLayout(skeleton_displays.back().skeleton_display_vertex_buffer.format_hash, &shader_program);
	
}

void DebugDisplaySkeleton::Deinit()
{
	renderer->DestroyState(&depth_stencil_state);
	for (unsigned int i = 0; i < skeleton_displays.size(); ++i)
	{
		renderer->Delete(&skeleton_displays[i].skeleton_display_vertex_buffer);
		renderer->Delete(&skeleton_displays[i].skeleton_display_index_buffer);
	}
	skeleton_displays.clear();

	renderer->Delete(&mvp);
	renderer->Delete(&shader_program);
}
