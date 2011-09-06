#ifndef _MESHVIEWER_MATRIX_BUFFER_DATA_H_
#define _MESHVIEWER_MATRIX_BUFFER_DATA_H_



class MeshViewer_MatrixBufferData
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

	int mvpmatrix_location;
	int modelmatrix_location;
	int palette_location;

	int Ka_index;
	int Kd_index;
	int Ke_index;
	int Ks_index;
	int eye_position_index;
	int global_ambient_index;
	int light_color_index;
	int light_position_index;

	RendererBase * renderer;

	Projector * camera;
	Projector * light_source;

	SkinnedMesh * skinned_mesh;
	Eigen::Matrix4f skinning_matrices[60];

	std::vector<UniformBuffer> * uniform_buffers;

	void Init(RendererBase * _renderer, ShaderProgram * shader_program, Projector * _camera, Projector * _light_source, SkinnedMesh * _skinned_mesh, std::vector<UniformBuffer> * _uniform_buffers)
	{
		renderer = _renderer;
		camera = _camera;
		skinned_mesh = _skinned_mesh;
		light_source = _light_source;
		uniform_buffers = _uniform_buffers;
		

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


		mvpmatrix_location = shader_program->FindUniform("ModelViewProjMatrix");
		modelmatrix_location = shader_program->FindUniform("ModelMatrix");
		palette_location = shader_program->FindUniform("palette");

		Ka_index = shader_program->FindUniform("Ka");
		Kd_index = shader_program->FindUniform("Kd");
		Ke_index = shader_program->FindUniform("Ke");
		Ks_index = shader_program->FindUniform("Ks");
		eye_position_index = shader_program->FindUniform("eye_position");
		global_ambient_index = shader_program->FindUniform("global_ambient");
		light_color_index = shader_program->FindUniform("light_color");
		light_position_index = shader_program->FindUniform("light_position");
	}

	void Deinit()
	{
	}

	void PreRender(ShaderProgram * shader_program)
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

		}

		{
			unsigned int buffer_index = shader_program->GetUniformBufferIndex(mvpmatrix_location);
			UniformBufferUpdater ub_update(renderer, shader_program, &(*uniform_buffers)[buffer_index]);
			ub_update.Set(mvpmatrix_location, model_view_proj.data(), 64);
			ub_update.Set(modelmatrix_location, model.data(), 64);
			ub_update.Set(palette_location, skinning_matrices[0].data(), 64 * 60);
		}

		{
			unsigned int buffer_index = shader_program->GetUniformBufferIndex(Ka_index);
			UniformBufferUpdater ub_update(renderer, shader_program, &(*uniform_buffers)[buffer_index]);
			ub_update.Set(Ka_index, Ka, sizeof(Ka));
			ub_update.Set(Kd_index, Kd, sizeof(Kd));
			ub_update.Set(Ke_index, Ke, sizeof(Ke));
			ub_update.Set(Ks_index, Ks, sizeof(Ks));
			ub_update.Set(eye_position_index, camera->GetPosition(), sizeof(eye_position));
			ub_update.Set(global_ambient_index, global_ambient, sizeof(global_ambient));
			ub_update.Set(light_color_index, light_color, sizeof(light_color));
			ub_update.Set(light_position_index, light_source->GetPosition(), sizeof(light_position));
		}//Update Uniforms



	}

};

#endif