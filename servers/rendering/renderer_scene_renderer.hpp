#ifndef _ISM_RENDERER_SCENE_RENDERER_HPP_
#define _ISM_RENDERER_SCENE_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene renderer
	class ISM_API RendererSceneRenderer : public BaseObject
	{
		OBJECT_COMMON(RendererSceneRenderer, BaseObject);

		friend class RenderingServerDefault;

		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

	public:
		RendererSceneRenderer(RenderingDevice * device, RendererStorage * storage);

		virtual ~RendererSceneRenderer() override;

	public:
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

				float_t z_near{};
				float_t z_far{};
				
				float_t time{};

				// etc...
			}
			ubo{};
		};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_SCENE_RENDERER_HPP_
