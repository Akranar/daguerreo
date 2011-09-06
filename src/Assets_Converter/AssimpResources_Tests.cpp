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

class MeshView
{
public:
	typedef RendererFX<RendererBase> Renderer;
	Renderer renderer;
	Example_SceneData scene_data;
	Example_RenderStates render_state_set;
	//Example_TorusMesh mesh_torus;
	Example_EffectFragmentTexture effect_fragmentlighting;
	Example_EffectSkinning effect_skinning;
	Example_MeshQuad mesh_quad;
	Example_MeshBox mesh_box;
	Example_EffectCubeMapFile effect_cubemap;
	Example_AssimpMesh mesh_assimp;
	Skeleton * skeleton;
	SkinnedMesh * skinned_mesh;
	SkeletalAnimation * anim;
	double animation_time;

	DebugDisplaySkeleton debug_skel;

	bool has_skeleton;
	bool has_animation;

	void Init(WindowEvents * window)
	{
		renderer.Initialize(window);
		renderer.ClearColor(0.4f, 0.4f, 1.0f, 1.0f);

		scene_data.Init();
		//mesh_torus.Init(&renderer);
		//mesh_torus.InitWithShader();
		//mesh_torus.InitWithShader();

		mesh_quad.Init(&renderer);
		mesh_quad.InitWithShader(effect_fragmentlighting.Init(&renderer, &scene_data.camera, &scene_data.light_source));
		mesh_quad.InitWithShader(effect_cubemap.Init(&renderer, &scene_data.camera));

		mesh_box.Init(&renderer);
		mesh_box.InitWithShader(&effect_fragmentlighting.shader_program);
		mesh_box.InitWithShader(&effect_cubemap.shader_program);


		//const char * in_file = "MeshFiles/biped_rig.dae";
		//const char * in_file = "MeshFiles/astroboy_walk_max.dae";
		const char * in_file = "MeshFiles/boblampclean.md5mesh";
		Assimp::Importer importer;
		const aiScene * ai_scene = importer.ReadFile(in_file, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_OptimizeGraph | aiProcess_GenNormals);
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
		
		assimp_scene_data.OutputMeshResources("seymour");
		assimp_scene_data.OutputSceneGraphResources("seymour");
		assimp_scene_data.OuptutAnimationResources("seymour");

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
		
		//render_state_set.Init(&renderer);
		//render_state_set.Set();

		
		mesh_assimp.Init(&renderer, &assimp_scene_data);
		mesh_assimp.InitWithShader(effect_skinning.Init(&renderer, &scene_data.camera, &scene_data.light_source, skinned_mesh), &assimp_scene_data);

		if (has_skeleton)
		{
			debug_skel.Init(&renderer, &scene_data.camera);
			debug_skel.InitWithSkinnedMesh(skinned_mesh);
		}

		animation_time = 0;
	}
	void Deinit()
	{ 
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
		effect_skinning.Deinit();
		mesh_assimp.Deinit(renderer);
		//render_state_set.Deinit();
		effect_cubemap.Deinit();
		effect_fragmentlighting.Deinit();
		mesh_box.Deinit(renderer);
		mesh_quad.Deinit(renderer);
		//mesh_torus.Deinit(renderer);
	}

	void Draw()
	{	
		
		renderer.PreRender();
		{
			renderer.ClearAllBuffer();

			ShaderProgram * skin_program = effect_skinning.PreRender();
			for (unsigned int i = 0; i < mesh_assimp.GetMeshCount(); ++i)
			{
				mesh_assimp.PreRender(i);
				mesh_assimp.PreRenderWithShader(i, skin_program);
				//mesh_assimp.PreRenderWithShader(i, &effect_fragmentlighting.shader_program);
				renderer.Draw();
			}
			
			


			mesh_quad.PreRender();
			mesh_quad.PreRenderWithShader(effect_fragmentlighting.PreRender());
			//render_state_set.Set();
			renderer.Draw();
			
			

			mesh_box.PreRender();
			//mesh_box.PreRenderWithShader(&effect_fragmentlighting.shader_program);
			mesh_box.PreRenderWithShader(effect_cubemap.PreRender());
			//render_state_set.Set();
			renderer.Draw();
			
			debug_skel.PreDraw(0);
			renderer.Draw();
			
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
					scene_data.camera.SetPerspective(45.0f, float(input_event.data.resize.GetWidth()) /  float(input_event.data.resize.GetHeight()), 1.0f, 5000);
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

/*
void AssimpResources_Tests::Entry()
{
	try
	{
		ApplicationLoop<MeshView>().Run();
	}
	catch (Exception & e)
	{
		e.PrintReason();
	}
}
*/