#ifndef _EXAMPLE_EFFECT_FRAGMENT_TEXTURE_SKINNING_H_
#define _EXAMPLE_EFFECT_FRAGMENT_TEXTURE_SKINNING_H_

#include "ExLib_RendererBase.h"
#include "ExLib_Projector.h"


#include "Example_Texture2DFile.h"

class Example_EffectFragmentTextureSkinning
{
public:
	float Ka [3];
	float Kd [3];
	float Ke [3];
	float Ks [4];
	float eye_position [3];
	float global_ambient [3];
	float light_color [3];
	float light_position [3];

	int Ka_index;
	int Kd_index;
	int Ke_index;
	int Ks_index;
	int eye_position_index;
	int global_ambient_index;
	int light_color_index;
	int light_position_index;

	int mvpmatrix_location;
	int modelmatrix_location;
	int palette_location;

	ShaderProgram shader_program;
	RendererBase * renderer;

	UniformBuffer matrix_buffer;
	UniformBuffer material_buffer;

	Projector * camera;
	Projector * light_source;

	Example_Texture2DFile texture2d;
	int texture_index;

	ShaderProgram * Init(RendererBase * _renderer, Projector * _camera, Projector * _light_source)
	{
		renderer = _renderer;
		camera = _camera;
		light_source = _light_source;

		Ka[0] = 0.2f;
		Ka[1] = 0.2f;
		Ka[2] = 0.2f;

		Kd[0] = 0.5f;
		Kd[1] = 0.5f;
		Kd[2] = 1.0f;

		Ke[0] = 0.0f;
		Ke[1] = 0.0f;
		Ke[2] = 0.0f;

		Ks[0] = 0.4f;
		Ks[1] = 0.4f;
		Ks[2] = 0.4f;
		Ks[3] = 50.0f;

		eye_position[0] = 0.0f;
		eye_position[1] = -15.0f;
		eye_position[2] = 0.0f;

		global_ambient[0] = 0.5f;
		global_ambient[1] = 0.25f;
		global_ambient[2] = 0.25f;

		light_color[0] = 1.0f;
		light_color[1] = 1.0f;
		light_color[2] = 1.0f;

		light_position[0] = 1.0f;
		light_position[1] = -4.0f;
		light_position[2] = 4.0f;


		DescShaderProgram shader_desc;
#ifdef USE_OPENGL
		//shader_desc.ReadFiles("GLSL/LightingDemoCG.glslv", "GLSL/LightingDemoCG.glslf");
		shader_desc.ReadFiles("GLSL/Skinning.glslv", "GLSL/LightingDemo.glslf");
#else
		//shader_desc.ReadFiles("HLSL/LightingDemoCG.vs4", "HLSL/LightingDemoCG.ps4");
		shader_desc.ReadFiles("HLSL/LightingDemo.vs4", "HLSL/LightingDemo.ps4");
#endif
		renderer->CreateResource(shader_desc, &shader_program);

		mvpmatrix_location = shader_program.FindUniform("ModelViewProjMatrix");
		modelmatrix_location = shader_program.FindUniform("ModelMatrix");
		palette_location = shader_program.FindUniform("palette");

		Ka_index = shader_program.FindUniform("Ka");
		Kd_index = shader_program.FindUniform("Kd");
		Ke_index = shader_program.FindUniform("Ke");
		Ks_index = shader_program.FindUniform("Ks");
		eye_position_index = shader_program.FindUniform("eye_position");
		global_ambient_index = shader_program.FindUniform("global_ambient");
		light_color_index = shader_program.FindUniform("light_color");
		light_position_index = shader_program.FindUniform("light_position");


		DescUniformBuffer uniform_desc;
		uniform_desc.index = 0;
		uniform_desc.domain = DOMAIN_VERTEX;
		uniform_desc.byte_count = shader_program.uniform_blocks[uniform_desc.index].byte_count;
		renderer->CreateResource(uniform_desc, &matrix_buffer);

		uniform_desc.domain = DOMAIN_FRAGMENT;
		uniform_desc.byte_count = shader_program.uniform_blocks[uniform_desc.index].byte_count;
		renderer->CreateResource(uniform_desc, &material_buffer);

		

		//find sampler unit first;
		texture2d.Init(renderer, &shader_program, 0, "Textures/boy_10.png");


		shader_program.PrintUniforms();
		shader_program.PrintBlocks();

		return &shader_program;
		
	}

	void Deinit()
	{
		texture2d.Deinit();
		renderer->Delete(&matrix_buffer);
		renderer->Delete(&material_buffer);
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

			Eigen::Matrix4f identities[60];
			for (unsigned int i = 0; i < 60; ++i)
			{
				identities[i] = Eigen::Matrix4f::Identity();
			}

			{
				UniformBufferUpdater ub_update(renderer, &shader_program, &matrix_buffer);
				ub_update.Set(mvpmatrix_location, model_view_proj.data(), 64);
				ub_update.Set(modelmatrix_location, model.data(), 64);
				ub_update.Set(palette_location, identities[0].data(), 64 * 60);
			}//Update Matrix
			
			{
				UniformBufferUpdater ub_update(renderer, &shader_program, &material_buffer);
				ub_update.Set(Ka_index, Ka, sizeof(Ka));
				ub_update.Set(Kd_index, Kd, sizeof(Kd));
				ub_update.Set(Ke_index, Ke, sizeof(Ke));
				ub_update.Set(Ks_index, Ks, sizeof(Ks));
				ub_update.Set(eye_position_index, camera->GetPosition(), sizeof(eye_position));
				ub_update.Set(global_ambient_index, global_ambient, sizeof(global_ambient));
				ub_update.Set(light_color_index, light_color, sizeof(light_color));
				ub_update.Set(light_position_index, light_source->GetPosition(), sizeof(light_position));
				
			}//Updating Material

			renderer->BindUniformBuffer(&material_buffer, &shader_program);
			renderer->BindUniformBuffer(&matrix_buffer, &shader_program);
		}
		texture2d.PreRender();
		return &shader_program;
	}

};

#endif