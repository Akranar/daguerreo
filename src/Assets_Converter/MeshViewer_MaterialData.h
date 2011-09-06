#ifndef _MESHVIEWER_MATERIAL_BUFFER_DATA_H_
#define _MESHVIEWER_MATERIAL_BUFFER_DATA_H_


class MeshViewer_MaterialData
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

	UniformBuffer material_buffer;
	RendererBase * renderer;
	Projector * camera;
	Projector * light_source;

	void Init(RendererBase * _renderer, ShaderProgram * shader_program, Projector * _camera, Projector * _light_source)
	{
		renderer = _renderer;
		camera = _camera;
		light_source = _light_source;

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

		Ka_index = shader_program->FindUniform("Ka");
		Kd_index = shader_program->FindUniform("Kd");
		Ke_index = shader_program->FindUniform("Ke");
		Ks_index = shader_program->FindUniform("Ks");
		eye_position_index = shader_program->FindUniform("eye_position");
		global_ambient_index = shader_program->FindUniform("global_ambient");
		light_color_index = shader_program->FindUniform("light_color");
		light_position_index = shader_program->FindUniform("light_position");

		DescUniformBuffer uniform_desc;
		uniform_desc.index = 0;
		uniform_desc.domain = DOMAIN_FRAGMENT;
		uniform_desc.byte_count = shader_program->uniform_blocks[uniform_desc.index].byte_count;
		renderer->CreateResource(uniform_desc, &material_buffer);
	}

	void Deinit()
	{
		renderer->Delete(&material_buffer);
	}

	void PreRender(ShaderProgram * shader_program)
	{
		{
			UniformBufferUpdater ub_update(renderer, shader_program, &material_buffer);
			ub_update.Set(Ka_index, Ka, sizeof(Ka));
			ub_update.Set(Kd_index, Kd, sizeof(Kd));
			ub_update.Set(Ke_index, Ke, sizeof(Ke));
			ub_update.Set(Ks_index, Ks, sizeof(Ks));
			ub_update.Set(eye_position_index, camera->GetPosition(), sizeof(eye_position));
			ub_update.Set(global_ambient_index, global_ambient, sizeof(global_ambient));
			ub_update.Set(light_color_index, light_color, sizeof(light_color));
			ub_update.Set(light_position_index, light_source->GetPosition(), sizeof(light_position));
			
		}//Updating Material
	}

};

#endif