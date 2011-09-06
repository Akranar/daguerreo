#include "AssimpResources_Tests.h"
#include "AssimpResources.h"
#include "AssimpScenePrinter.h"

#include <iostream>

#include "AssimpSceneData.h"

#include "ApplicationLoop.h"
#include "ExLib_WindowEvents.h"
#include "../Example_RendererBaseUsage/RendererBaseUsage_LIB.h"

#include "Example_AssimpMesh.h"

#include "Example_EffectSkinning.h"
#include "DebugDisplaySkeleton.h"
#include "ExLib_RendererFX.h"
#include "ExLib_GraphicsRegistryBase.h"
#include "ExLib_SceneRegistryBase.h"
#include "ExLib_GraphicsRegistryFX.h"
#include "ExLib_RendererSG.h"

#include "MeshViewer_MatrixBufferData.h"
#include "MeshViewer_MaterialData.h"

#include "ExLib_BoundCreation.h"

class MeshViewFX
{
public:
	typedef RendererSG<RendererFX<RendererBase> > Renderer;
	typedef SceneRegistryBase<GraphicsRegistryFX<GraphicsRegistryBase<Renderer> > > Registry;
	Renderer renderer;
	Registry registry;

	Example_SceneData scene_data;

	bool has_skeleton;
	bool has_animation;
	bool show_skeleton;

	//Skin effect uniform updaters.
	MeshViewer_MatrixBufferData matrix_buffer_data;

	//Skin
	std::vector<UniformBuffer> skin_uniforms;
	VisualFXClass skinning_fxclass;
	std::vector<VisualFXInstance> skinning_fxinsts;
	std::vector<Visual*> skins;

	//Skeleton and animation.
	Skeleton * skeleton;
	SkinnedMesh * skinned_mesh;
	SkeletalAnimation * anim;
	double animation_time;

	//Skeleton display.
	DebugDisplaySkeleton debug_skel;

	//Skybox.
	Example_MeshBox mesh_box;
	VisualFXClass cube_map_fxclass;
	VisualFXInstance cube_map_fxinst;
	Visual cube_visual;

	//Terrain.
	Example_MeshQuadUpY mesh_quad;
	VisualFXClass texture2d_fx_class;
	VisualFXInstance texture2d_fxinst;
	Visual terrain_visual;

	//Uniform buffer shared by Skybox and Terrain.
	UniformBuffer model_view_proj;

	//SceneNode
	SceneNode node;

	void InitSkinning(unsigned int skin_mesh_count)
	{
		#ifdef USE_OPENGL21
		ShaderProgram * skinning_shader = registry.Retrieve(registry.GetShaderProgramCatalog(), "GLSL/Skinning.glslv|GLSL/LightingDemo.glslf");
		#elif USE_DIRECTX10
		ShaderProgram * skinning_shader = registry.Retrieve(registry.GetShaderProgramCatalog(), "HLSL/Skinning.vs4|HLSL/LightingDemo.ps4");
		#else
		ShaderProgram * skinning_shader = 0;
		#endif

		skinning_fxclass.Initialize(1);
		skinning_fxclass.GetTechnique(0)->Initialize(1);
		skinning_fxclass.GetTechnique(0)->GetPass(0)->Initialize(skinning_shader);

		for (unsigned int i = 0; i < skinning_shader->GetUniformBlockCount(); ++i)
		{
			skin_uniforms.push_back(UniformBuffer());

			DescUniformBuffer uniform_desc;
			uniform_desc.byte_count = skinning_shader->GetUniformBlockSize(i);
			uniform_desc.domain = 0;
			uniform_desc.index = i;

			renderer.CreateResource(uniform_desc, &skin_uniforms.back());
		}

		matrix_buffer_data.Init(&renderer, skinning_shader, &scene_data.camera, &scene_data.light_source, skinned_mesh, &skin_uniforms);

		std::vector<std::string> texture_files;
		
		
		texture_files.push_back("Textures/guard1_body.png");
		texture_files.push_back("Textures/guard1_face.png");
		texture_files.push_back("Textures/guard1_helmet.png");
		texture_files.push_back("Textures/iron_grill.png");
		texture_files.push_back("Textures/round_grill.png");
		texture_files.push_back("Textures/guard1_body.png"); //Sword
		

		while(texture_files.size() < skin_mesh_count)
		{
			texture_files.push_back("Textures/boy_10.png");
		}

		for (unsigned int i = 0; i < skin_mesh_count; ++i)
		{
			skinning_fxinsts.push_back(VisualFXInstance());
			skinning_fxinsts.back().Initialize(&skinning_fxclass, 0);
			for (unsigned int j = 0; j < skinning_shader->GetUniformBlockCount(); ++j)
			{
				skinning_fxinsts.back().SetUniformBuffer(0, j, &skin_uniforms[j]);
			}
			skinning_fxinsts.back().SetTexture(0, 0, registry.Retrieve(registry.GetTexture2DCatalog(), texture_files[i].c_str()));
		}

		for (unsigned int i = 0; i < skin_mesh_count; ++i)
		{
			std::stringstream visual_file;
			visual_file << "assimpmesh_" << i << ".vsr";
			Visual * visual = registry.Retrieve(registry.GetVisualCatalog(), visual_file.str().c_str());
			visual->SetFX(&skinning_fxinsts[i]);
			for (unsigned int j = 0; j < skinning_fxinsts[i].GetTechnique()->GetPassCount(); ++j)
			{
				renderer.CreateInputLayout(visual->GetVertexBuffer()->format_hash, skinning_fxinsts[i].GetTechnique()->GetPass(j)->GetShaderProgram());
			}
			skins.push_back(visual);
			node.AttachChild(visual);
		}

	}

	void InitSkyCube()
	{
		#ifdef USE_OPENGL21
		ShaderProgram * cube_shader = registry.Retrieve(registry.GetShaderProgramCatalog(), "GLSL/TextureCube.glslv|GLSL/TextureCube.glslf");
		#elif USE_DIRECTX10
		ShaderProgram * cube_shader = registry.Retrieve(registry.GetShaderProgramCatalog(), "HLSL/TextureCube.vs4|HLSL/TextureCube.ps4");
		#else
		ShaderProgram * cube_shader = 0;
		#endif
		cube_map_fxclass.Initialize(1);
		cube_map_fxclass.GetTechnique(0)->Initialize(1);
		cube_map_fxclass.GetTechnique(0)->GetPass(0)->Initialize(cube_shader);

		cube_map_fxinst.Initialize(&cube_map_fxclass, 0);
		cube_map_fxinst.SetTexture(0, 0, registry.Retrieve(registry.GetTextureCubeCatalog(), "Textures/brightday2.png"));
		cube_map_fxinst.SetUniformBuffer(0, 0, &model_view_proj);

		cube_visual.SetVertexBuffer(&mesh_box.vertex_buffer);
		cube_visual.SetIndexBuffer(&mesh_box.index_buffer);
		cube_visual.SetModelBound(0);
		cube_visual.SetFX(&cube_map_fxinst);

		renderer.CreateInputLayout(cube_visual.GetVertexBuffer()->format_hash, cube_shader);

	}


	void InitTerrainPatch()
	{
		#ifdef USE_OPENGL21
		ShaderProgram * texture_2d = registry.Retrieve(registry.GetShaderProgramCatalog(), "GLSL/Texture2D.glslv|GLSL/Texture2D.glslf");
		#elif USE_DIRECTX10
		ShaderProgram * texture_2d = registry.Retrieve(registry.GetShaderProgramCatalog(), "HLSL/Texture2D.vs4|HLSL/Texture2D.ps4");
		#else
		ShaderProgram * texture_2d = 0;
		#endif
		
		texture2d_fx_class.Initialize(1);
		texture2d_fx_class.GetTechnique(0)->Initialize(1);
		texture2d_fx_class.GetTechnique(0)->GetPass(0)->Initialize(texture_2d);

		texture2d_fxinst.Initialize(&texture2d_fx_class, 0);
		texture2d_fxinst.SetTexture(0, 0, registry.Retrieve(registry.GetTexture2DCatalog(), "Textures/grass_2048.jpg"));
		texture2d_fxinst.SetUniformBuffer(0, 0, &model_view_proj);

		terrain_visual.SetVertexBuffer(&mesh_quad.vertex_buffer);
		terrain_visual.SetIndexBuffer(&mesh_quad.index_buffer);
		terrain_visual.SetModelBound(0);
		terrain_visual.SetFX(&texture2d_fxinst);

		renderer.CreateInputLayout(terrain_visual.GetVertexBuffer()->format_hash, texture_2d);
	}

	void InitMVPUniform()
	{
		DescUniformBuffer desc;
		desc.byte_count = 64;
		desc.domain = DOMAIN_VERTEX;
		desc.index = 0;
		renderer.CreateResource(desc, &model_view_proj);
	}

	void DeinitMVPUniform()
	{
		renderer.Delete(&model_view_proj);
	}

	void UpdateMVPUniform()
	{
		//Calculate Matrices.
		Eigen::Matrix4f model = Eigen::Matrix4f::Identity() * 3;
		model(3, 3) = 1.0f;
		Eigen::Matrix4f model_view_proj_matrix;
		scene_data.camera.ViewProjMatrix(model_view_proj_matrix.data());
		model_view_proj_matrix = model * model_view_proj_matrix;

		{
			void * mvp_data = renderer.Map(&model_view_proj);
			memcpy(mvp_data, model_view_proj_matrix.data(), 64);
			renderer.Unmap(&model_view_proj);
		}
	}


	void Init(WindowEvents * window)
	{
		renderer.Initialize(window);
		renderer.ClearColor(0.4f, 0.4f, 1.0f, 1.0f);

		scene_data.Init();

		//const char * in_file = "MeshFiles/biped_rig.dae";
		//const char * in_file = "MeshFiles/astroboy_walk_max.dae";
		const char * in_file = "MeshFiles/boblampclean.md5mesh";
		Assimp::Importer importer;
		const aiScene * ai_scene = importer.ReadFile(in_file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeGraph | aiProcess_GenSmoothNormals);
		if (!ai_scene)
		{
			std::cout << "Assimp Scene Read Failed." << std::endl;
			return;
		}
		AssimpScenePrinter printer;
		printer.PrintAllToFile(ai_scene, "scene.txt");
		AssimpResources assimp_resources;
		AssimpSceneData assimp_scene_data;
		assimp_resources.CreateAssimpSceneData(in_file, ai_scene, assimp_scene_data, 0);
		
		assimp_scene_data.OutputMeshResources("assimpmesh");
		assimp_scene_data.OutputSceneGraphResources("assimpmesh");
		assimp_scene_data.OuptutAnimationResources("assimpmesh");

		int assimp_mesh_count = assimp_scene_data.GetMeshCount();

		has_skeleton = false;
		has_animation = false;
		if (assimp_scene_data.skeletons.size() > 0)
		{
			has_skeleton = true;
			skeleton = assimp_scene_data.skeletons[0];
			assimp_scene_data.skeletons.clear();

			skinned_mesh = new SkinnedMesh(skeleton);
			skinned_mesh->SetDefaultPose();
			skinned_mesh->GeometricStateUpdate();
		}

		if (assimp_scene_data.skeletal_animations.size() > 0)
		{
			has_animation = true;
			anim = assimp_scene_data.skeletal_animations[0];
			assimp_scene_data.skeletal_animations.clear();
		}
		
		if (has_skeleton)
		{
			debug_skel.Init(&renderer, &scene_data.camera);
			debug_skel.InitWithSkinnedMesh(skinned_mesh);
		}

		animation_time = 0;

		registry.SetRenderer(&renderer);
		InitSkinning(assimp_mesh_count);

		mesh_quad.Init(&renderer);
		mesh_box.Init(&renderer);
		InitTerrainPatch();
		InitSkyCube();

		InitMVPUniform();

		show_skeleton = false;

		node.SetWorldBound(BoundCreation::NewOfSameType(skins[0]->GetWorldBound()));
		node.GeometricStateUpdate();

		AABB * box = (AABB *) node.GetWorldBound();
		scene_data.camera.LookAt(
			0, 
			box->GetExtents()[1] * 2, 
			box->GetExtents()[2] * 2, 
			box->GetCenter()[1], box->GetCenter()[1],box->GetCenter()[1], 0, 0, 1);

		scene_data.camera_controller.SetProjector(&scene_data.camera, Eigen::Vector3f(0, 1, 0));
		scene_data.light_source = scene_data.camera;
		scene_data.camera_controller.SetSpeed(100);
	}
	void Deinit()
	{ 
		for (unsigned int i = 0; i < skin_uniforms.size(); ++i)
		{
			renderer.Delete(&skin_uniforms[i]);
		}
		skin_uniforms.clear();

		DeinitMVPUniform();
		matrix_buffer_data.Deinit();

		debug_skel.Deinit();
		if (has_animation)
		{
			delete anim;
		}
		if (has_skeleton)
		{
			delete skinned_mesh;
			delete skeleton;
		}
		mesh_box.Deinit(renderer);
		mesh_quad.Deinit(renderer);
	}

	void Draw()
	{	
		
		renderer.PreRender();
		{
			renderer.ClearAllBuffer();

			{//Update uniform parameters.
				ShaderProgram * current_program = skinning_fxinsts[0].GetTechnique()->GetPass(0)->GetShaderProgram();
				matrix_buffer_data.PreRender(current_program);

				UpdateMVPUniform();
			}

			{//Draw
				for (unsigned int i = 0; i < skins.size(); ++i)
				{
					renderer.DrawVisual(skins[i]);
				}
				renderer.DrawVisual(&terrain_visual);
				renderer.DrawVisual(&cube_visual);
				if (show_skeleton)
				{
					debug_skel.PreDraw(0);
					renderer.Draw();
				}
			}
			
		}
		renderer.PostRender();
	}

	

	void Update(double seconds_elapsed, WindowEvents * active_window)
	{
		if (active_window)
		{
			InputManager * input_manager = &active_window->GetInputManager();
			while (input_manager->HasEvents())
			{
				input_manager->ProcessNext();
				const InputEvent & input_event = input_manager->PeekNext();

				if (input_event.GetType() == InputEvent::KEYBOARD && input_event.data.key.GetType() == KeyEvent::KEY_DOWN)
				{
					switch (input_event.data.key.GetKey())
					{
						case KeyID::KEY_SPACE:
						{
							scene_data.light_source = scene_data.camera;
						}
						break;
						case KeyID::KEY_Z:
						{
							animation_time = seconds_elapsed;
						}
						break;
						case KeyID::KEY_X:
						{
							show_skeleton = !show_skeleton;
						}
						break;
						case KeyID::KEY_EQUAL:
						{
							scene_data.camera_controller.SetSpeed(100);
						}
						break;
						case KeyID::KEY_UNDERSCORE:
						{
							scene_data.camera_controller.SetSpeed(20);
						}
						break;
					}
				}
				else if (input_event.GetType() == InputEvent::KEYBOARD && input_event.data.key.GetType() == KeyEvent::KEY_UP)
				{
					switch (input_event.data.key.GetKey())
					{

						case KeyID::KEY_ESCAPE:
						{
							//Temporary hack to fix problem in windows where after Q down to close window, the up key isn't caught.
							active_window->Close();
						}
						break;
						default:
						break;
					}
				}
				else if (input_event.GetType() == InputEvent::RESIZE)
				{
					renderer.OnViewportUpdate(0, 0, input_event.data.resize.GetWidth(), input_event.data.resize.GetHeight());
					scene_data.OnViewportUpdate(input_event.data.resize.GetWidth(), input_event.data.resize.GetHeight());
				}
				scene_data.UpdateCameraOrientation(active_window, seconds_elapsed);
				input_manager->PopEvent();
			}
			scene_data.UpdateCameraPosition(active_window, seconds_elapsed);
		}
		//mesh_torus.Update(seconds_elapsed);

		if (has_animation)
		{
			if (seconds_elapsed - animation_time > anim->GetDuration())
			{
				//skinned_mesh->SetDefaultPose();
				//skinned_mesh->GeometricStateUpdate();
				animation_time = seconds_elapsed;
			}
			else
			{
				anim->UpdateSkinnedMesh(skinned_mesh, std::max(seconds_elapsed - animation_time, 0.0), 1.0f, true);
				skinned_mesh->GeometricStateUpdate();
			}
		}
		
		if (has_skeleton)
		{
			debug_skel.Update(0);
		}
	}

	
};



void AssimpResources_Tests::Entry()
{
	try
	{
		ApplicationLoop<MeshViewFX>().Run();
	}
	catch (Exception & e)
	{
		e.PrintReason();
	}
}
