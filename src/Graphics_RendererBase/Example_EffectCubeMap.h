#ifndef _EXAMPLE_EFFECT_CUBE_MAP_H_
#define _EXAMPLE_EFFECT_CUBE_MAP_H_

#include "RendererBase.h"
#include "ExLib_Projector.h"
#include "IncludeAll_Interface.h"
#include "IncludeAll_Desc.h"
#include "Aux_UniformBufferUpdater.h"

#include "Example_TextureCube.h"

class Example_EffectCubeMap
{
public:
	int mvpmatrix_location;

	ShaderProgram shader_program;
	RendererBase * renderer;

	UniformBuffer matrix_buffer;

	Projector * camera;

	Example_TextureCube texturecube;
	int texture_index;

	ShaderProgram * Init(RendererBase * _renderer, Projector * _camera)
	{
		renderer = _renderer;
		camera = _camera;


		DescShaderProgram shader_desc;
#ifdef USE_OPENGL
		shader_desc.ReadFiles("GLSL/TextureCube.glslv", "GLSL/TextureCube.glslf");
		renderer->CreateResource(shader_desc, &shader_program);
		mvpmatrix_location = shader_program.FindUniform("ModelViewProjMatrix");
#else
		shader_desc.ReadFiles("HLSL/TextureCube.vs4", "HLSL/TextureCube.ps4");
		renderer->CreateResource(shader_desc, &shader_program);
		mvpmatrix_location = shader_program.FindUniform("ModelViewProjMatrix");
#endif
		
		DescUniformBuffer uniform_desc;
		uniform_desc.index = 0;
		uniform_desc.domain = DOMAIN_VERTEX;
		uniform_desc.byte_count = shader_program.uniform_blocks[uniform_desc.index].byte_count;
		renderer->CreateResource(uniform_desc, &matrix_buffer);


		

		//find sampler unit first;
		texturecube.Init(renderer, &shader_program, 0, 0);


		shader_program.PrintUniforms();
		shader_program.PrintBlocks();

		return &shader_program;
		
	}

	void Deinit()
	{
		texturecube.Deinit();
		renderer->Delete(&matrix_buffer);
		renderer->Delete(&shader_program);
	}

	ShaderProgram * PreRender()
	{
		renderer->BindShaderProgram(&shader_program);
		{
			//Calculate Matrices.
			Eigen::Matrix4f model = Eigen::Matrix4f::Identity() * 3;
			model(3, 3) = 1.0f;
			Eigen::Matrix4f model_view_proj;
			camera->ViewProjMatrix(model_view_proj.data());
			model_view_proj = model * model_view_proj;

			{
				UniformBufferUpdater ub_update(renderer, &shader_program, &matrix_buffer);
				ub_update.Set(mvpmatrix_location, model_view_proj.data(), 64);
			}//Update Matrix

			renderer->BindUniformBuffer(&matrix_buffer, &shader_program);
			
		}
		texturecube.PreRender();
		return &shader_program;
	}

};

#endif