#ifndef _EXAMPLE_EFFECT_CUBE_MAP_FILE_H_
#define _EXAMPLE_EFFECT_CUBE_MAP_FILE_H_

#include "ExLib_RendererBase.h"
#include "ExLib_Projector.h"

#include "Example_TextureCubeFile.h"

class Example_EffectCubeMapFile
{
public:
	int mvpmatrix_location;

	ShaderProgram shader_program;
	RendererBase * renderer;

	UniformBuffer matrix_buffer;

	Projector * camera;

	Example_TextureCubeFile texturecube;
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
		const char * filenames[6];
		
		filenames[0] = "Textures/brightday2_positive_x.png";
		filenames[1] = "Textures/brightday2_negative_x.png";
		filenames[2] = "Textures/brightday2_positive_y.png";
		filenames[3] = "Textures/brightday2_negative_y.png";
		filenames[4] = "Textures/brightday2_positive_z.png";
		filenames[5] = "Textures/brightday2_negative_z.png";
		

		texturecube.Init(renderer, &shader_program, 0, filenames);


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