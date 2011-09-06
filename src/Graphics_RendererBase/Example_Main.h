#ifndef _EXAMPLE_MAIN_H_
#define _EXAMPLE_MAIN_H_

#include "RendererBaseEntries.h"
#include "RendererBase.h"
#include "IncludeAll_Interface.h"
#include "IncludeAll_Desc.h"

#include "ExLib_ProceduralTexture.h"
#include "ExLib_ProceduralMesh.h"
#include "ExLib_Eigen.h"
#include "ExLib_WindowEvents.h"

#include "ApplicationLoop.h"
#include "ExLib_Projector.h"
#include "MouseLookController.h"

#include "Aux_UniformBufferUpdater.h"

#include "Example_TorusMesh.h"
#include "Example_SceneData.h"
#include "Example_RenderStates.h"
#include "Example_EffectFragmentLighting.h"
#include "Example_MeshQuad.h"
#include "Example_Texture2D.h"
#include "Example_TextureCube.h"
#include "Example_MeshBox.h"
#include "Example_EffectCubeMap.h"


class Example_Main
{
public:
	RendererBase renderer;
	Example_SceneData scene_data;
	Example_RenderStates render_state_set;
	Example_TorusMesh mesh_torus;
	Example_EffectFragmentLighting effect_fragmentlighting;
	Example_MeshQuad mesh_quad;
	Example_MeshBox mesh_box;
	Example_EffectCubeMap effect_cubemap;

	void Init(WindowEvents * window)
	{
		renderer.Initialize(window);
		renderer.ClearColor(0.4f, 0.4f, 1.0f, 1.0f);

		scene_data.Init();
		mesh_torus.Init(&renderer);
		mesh_torus.InitWithShader(effect_fragmentlighting.Init(&renderer, &scene_data.camera, &scene_data.light_source));
		mesh_torus.InitWithShader(effect_cubemap.Init(&renderer, &scene_data.camera));

		mesh_quad.Init(&renderer);
		mesh_quad.InitWithShader(&effect_fragmentlighting.shader_program);
		mesh_quad.InitWithShader(&effect_cubemap.shader_program);

		mesh_box.Init(&renderer);
		mesh_box.InitWithShader(&effect_fragmentlighting.shader_program);
		mesh_box.InitWithShader(&effect_cubemap.shader_program);
		
		render_state_set.Init(&renderer);
		render_state_set.Set();
		
	}
	void Deinit()
	{ 
		render_state_set.Deinit();
		effect_cubemap.Deinit();
		effect_fragmentlighting.Deinit();
		mesh_box.Deinit(renderer);
		mesh_quad.Deinit(renderer);
		mesh_torus.Deinit(renderer);
	}

	void Draw()
	{	
		renderer.PreRender();
		{
			renderer.ClearAllBuffer();

			
			mesh_torus.PreRender();
			mesh_torus.PreRenderWithShader(effect_fragmentlighting.PreRender());
			renderer.Draw();

			mesh_quad.PreRender();
			mesh_quad.PreRenderWithShader(&effect_fragmentlighting.shader_program);
			renderer.Draw();
			

			mesh_box.PreRender();
			//mesh_box.PreRenderWithShader(&effect_fragmentlighting.shader_program);
			mesh_box.PreRenderWithShader(effect_cubemap.PreRender());
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
		mesh_torus.Update(seconds_elapsed);
	}

	
};

#endif