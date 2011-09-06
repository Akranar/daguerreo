#include "../RendererBaseEntries.h"
#include "../ApplicationLoop.h"
#include "../RendererBase.h"
#include "../IncludeAll_Interface.h"
#include "../MouseLookControllerDefines.h"
#include "../MouseLookController.h"
#include "../ExLib_Projector.h"

#include <Core_Exception.h>
#include <d3d9.h>
#include <d3dx9.h>

#include "RendererBasePimpl.h"
#include "../Example_TorusMesh.h"
#include "SampleTexture.h"


class SceneData
{
public:
	Projector camera;
	Projector light_source;
};

class ShaderProgramPimpl
{
public:
	IDirect3DVertexShader9 * vertex_shader;
	IDirect3DPixelShader9 * pixel_shader;
	ID3DXConstantTable * vertex_constant_table;
	ID3DXConstantTable * pixel_constant_table;
};


class D3D9App
{
public:
	RendererBase renderer;
	Example_TorusMesh mesh_torus;
	SampleTexture texture_static;
	ShaderProgram program;
	MOUSELOOK_CONTROLLER_VAR_DECL;
	UPDATE_CAMERA_DECL;

	D3DXHANDLE mvp;

	BlendState blend_state;
	RasterizerState rast_state;
	DepthStencilState ds_state;
	SamplerState sampler_state;

	void InitStates()
	{
		DescBlendState blend_desc;
		DescRasterizerState rast_desc;
		DescDepthStencilState ds_desc;
		DescSamplerState samp_desc;

		blend_desc.SetDefaults();
		rast_desc.SetDefaults();
		ds_desc.SetDefaults();
		samp_desc.SetDefaults();

		renderer.CreateState(blend_desc, &blend_state);
		renderer.CreateState(rast_desc, &rast_state);
		renderer.CreateState(ds_desc, &ds_state);
		renderer.CreateState(samp_desc, &sampler_state);
	};

	void SetStates()
	{
		renderer.SetState(&blend_state);
		renderer.SetState(&rast_state);
		renderer.SetState(&ds_state);
		renderer.SetState(&sampler_state, 0);
	}

	void DeinitStates()
	{
		renderer.DestroyState(&blend_state);
		renderer.DestroyState(&rast_state);
		renderer.DestroyState(&ds_state);
		renderer.DestroyState(&sampler_state);
	}

	void Init(WindowEvents * window)
	{
		renderer.Initialize(window);
		renderer.ClearColor(0.4f, 0.4f, 1.0f, 1.0f);
		
		texture_static.Init(renderer);

		MOUSELOOK_INIT;

		DescShaderProgram shader_desc;
		shader_desc.ReadFiles("HLSL9/Texture2D.hlslv", "HLSL9/Texture2D.hlslf");
		//shader_desc.ReadFiles("HLSL9/Lighting.hlslv", "HLSL9/Lighting.hlslf");
		renderer.CreateResource(shader_desc, &program);

		mesh_torus.Init(&renderer);
		mesh_torus.InitWithShader(&program);

		mvp = ((ShaderProgramPimpl *) program.PRIVATE)->vertex_constant_table->GetConstantByName(NULL, "$_ModelViewProjMatrix");

		ShaderProgramPimpl * shader_pimpl = ((ShaderProgramPimpl *) program.PRIVATE);

		D3DXCONSTANTTABLE_DESC desc;
		shader_pimpl->vertex_constant_table->GetDesc(&desc);


		mvp = shader_pimpl->vertex_constant_table->GetConstant(NULL, 0);

		D3DXCONSTANT_DESC mvp_desc;
		UINT const_desc_count = 1;
		shader_pimpl->vertex_constant_table->GetConstantDesc(mvp, &mvp_desc, &const_desc_count);


		InitStates();
		SetStates();

	}
	void Deinit()
	{
		DeinitStates();
		renderer.Delete(&program);
		texture_static.Deinit(renderer);
		mesh_torus.Deinit(renderer);
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
					//OpenGLCore::SetViewport(input_event.data.resize.GetWidth(), input_event.data.resize.GetHeight());
					//glViewport(0, 0, input_event.data.resize.GetWidth(), input_event.data.resize.GetHeight());
					scene_data.camera.SetPerspective(45.0f, float(input_event.data.resize.GetWidth()) /  float(input_event.data.resize.GetHeight()), 1.0f, 5000);
				}
				UpdateCamera(input_event, active_window, seconds_elapsed);
				input_manager->PopEvent();
			}
			const KeyState & key_state = input_manager->GetKeyState();

			
			camera_controller.MovementUpdate(key_state.GetState(KeyID::KEY_W),
									key_state.GetState(KeyID::KEY_A),
									key_state.GetState(KeyID::KEY_D),
									key_state.GetState(KeyID::KEY_S),
									key_state.GetState(KeyID::KEY_E),
									key_state.GetState(KeyID::KEY_C),
									camera_controller.GetDistance(),
									seconds_elapsed
									);
		}

		mesh_torus.Update(seconds_elapsed);
	}
	void Draw()
	{	
		renderer.ClearAllBuffer();

		//Transforms
		Eigen::Matrix4f model = Eigen::Matrix4f::Identity() * 3;
		model(3, 3) = 1.0f;
		Eigen::Matrix4f model_view_proj;
		scene_data.camera.ViewProjMatrix(model_view_proj.data());
		model_view_proj = model * model_view_proj;
		
		//void * data;
		//cbuffer->Map(D3D10_MAP_WRITE_DISCARD, 0, &data);
		//memcpy(data, model_view_proj.data(), 64);
		//cbuffer->Unmap();
		D3DXMATRIX matrix;
		int index = 0;
		/*
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < 4; ++j)
			{
				matrix.m[i][j] = model_view_proj.data()[index];
				index += 1;
			}
		}
		*/
		memcpy(matrix.m, model_view_proj.data(), 64);

		ShaderProgramPimpl * shader_pimpl = ((ShaderProgramPimpl *) program.PRIVATE);
		HRESULT result = shader_pimpl->vertex_constant_table->SetMatrix(renderer.pimpl->d3d_device, mvp, &matrix);
		

		renderer.BindShaderProgram(&program);
		mesh_torus.PreRender();
		mesh_torus.PreRenderWithShader(&program);
		texture_static.PreRender(renderer);

		renderer.pimpl->d3d_device->BeginScene();

		result = renderer.pimpl->d3d_device->DrawIndexedPrimitive(
			  D3DPT_TRIANGLELIST,
			  0,
			  0,
			  mesh_torus.vertex_buffer.byte_count / mesh_torus.vertex_buffer.byte_stride,
			  0,
			  mesh_torus.index_buffer.byte_count / 12
			);

		renderer.pimpl->d3d_device->EndScene();

		texture_static.PostRender(renderer);
		mesh_torus.PostRender(renderer);
		renderer.BindShaderProgram(0);
	}
};


void RendererBaseEntries::Entry()
{
	ApplicationLoop<D3D9App>().Run();
}