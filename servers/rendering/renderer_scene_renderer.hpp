#ifndef _ISM_RENDERER_SCENE_RENDERER_HPP_
#define _ISM_RENDERER_SCENE_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering/shader_language.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene renderer
	class ISM_API RendererSceneRenderer
	{
	private:
		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

		enum {
			SCENE_STATE_UNIFORM_SET,
			RENDER_PASS_UNIFORM_SET,
			TRANSFORMS_UNIFORM_SET,
			MATERIAL_UNIFORM_SET,
		};

		struct SceneState
		{
			struct UBO
			{
				Mat4 projection_matrix{};
				Mat4 inv_projection_matrix{};

				Mat4 camera_matrix{};
				Mat4 inv_camera_matrix{};

				Vec2 viewport_size{};
				Vec2 screen_pixel_size{};

				f32 znear{};
				f32 zfar{};

				// etc...
			}
			ubo{};
		}
		m_scene_state{};

		struct RenderPass
		{
			struct UBO
			{

			}
			ubo{};
		}
		m_render_pass{};

		RID m_shader{};
		RID m_pipeline{};

	public:
		RendererSceneRenderer(RenderingDevice * device, RendererStorage * storage);
		~RendererSceneRenderer();

		void initialize();
		void finalize();

		void update_shader(RID shader);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_SCENE_RENDERER_HPP_
