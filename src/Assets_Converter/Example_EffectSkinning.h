#ifndef _EXAMPLE_EFFECT_SKINNING_H_
#define _EXAMPLE_EFFECT_SKINNING_H_

//#include "ExLib_RendererBase.h"
//#include "Projector.h"
//#include "Example_Texture2DFile.h"

class Example_EffectSkinning
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

	SkinnedMesh * skinned_mesh;
	Eigen::Matrix4f skinning_matrices[60];

	ShaderProgram * Init(RendererBase * _renderer, Projector * _camera, Projector * _light_source, SkinnedMesh * _skinned_mesh)
	{
		renderer = _renderer;
		camera = _camera;
		light_source = _light_source;
		skinned_mesh = _skinned_mesh;

		Ka[0] = 0.4f;
		Ka[1] = 0.4f;
		Ka[2] = 0.4f;

		Kd[0] = 0.5f;
		Kd[1] = 0.5f;
		Kd[2] = 0.5f;

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
			Eigen::Matrix4f model = Eigen::Matrix4f::Identity() * 1.0f;
			model(3, 3) = 1.0f;
			Eigen::Matrix4f model_view_proj;
			camera->ViewProjMatrix(model_view_proj.data());
			model_view_proj = model * model_view_proj;

			int skin_size = sizeof(skinning_matrices);
			
			for (unsigned int i = 0; i < skinned_mesh->GetJointCount(); ++i)
			{
				skinning_matrices[i] = (skinned_mesh->GetGlobalPose(i) * skinned_mesh->GetSkeleton()->GetJoint(i).GetInverseBindMatrix()).matrix().transpose();
				//skinning_matrices[i] = skinned_mesh->GetGlobalPose(i).matrix();
				//skinning_matrices[i] = model;
				//skinning_matrices[i] = Eigen::Matrix4f::Identity();
			}
			
			//skinning_matrices[53] = Eigen::Matrix4f::Identity();
			/*
			skinning_matrices[54] = Eigen::Matrix4f::Identity();
			skinning_matrices[52] = Eigen::Matrix4f::Zero();

			skinning_matrices[16] = Eigen::Matrix4f::Zero();
			skinning_matrices[17] = Eigen::Matrix4f::Zero();

			skinning_matrices[15] = Eigen::Matrix4f::Zero();
			skinning_matrices[14] = Eigen::Matrix4f::Zero();

			skinning_matrices[0] = Eigen::Matrix4f::Zero();
			skinning_matrices[1] = Eigen::Matrix4f::Zero();

			skinning_matrices[3] = Eigen::Matrix4f::Zero(); //torso
			skinning_matrices[4] = Eigen::Matrix4f::Zero(); //crouch
			skinning_matrices[5] = Eigen::Matrix4f::Zero(); //left thigh
			*/
			//skinning_matrices[53] = Eigen::Matrix4f::Zero();
			//skinning_matrices[53] = Eigen::Matrix4f::Zero();
			/*
			for (unsigned int i = 0; i < 10; ++i)
			{
				skinning_matrices[i] = Eigen::Matrix4f::Zero();
			}
			*/

			{
				UniformBufferUpdater ub_update(renderer, &shader_program, &matrix_buffer);
				ub_update.Set(mvpmatrix_location, model_view_proj.data(), 64);
				ub_update.Set(modelmatrix_location, model.data(), 64);
				ub_update.Set(palette_location, skinning_matrices[0].data(), 64 * 60);
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