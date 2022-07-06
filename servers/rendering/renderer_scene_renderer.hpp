#ifndef _ISM_RENDERER_SCENE_RENDERER_HPP_
#define _ISM_RENDERER_SCENE_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// scene renderer
	class ISM_API RendererSceneRenderer : public Object
	{
		OBJECT_COMMON(RendererSceneRenderer, Object);

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

	public:
		RID camera_create();
		void camera_set_perspective(RID camera, float fovy_degrees, float z_near, float z_far);
		void camera_set_orthogonal(RID camera, float size, float z_near, float z_far);
		void camera_set_frustum(RID camera, float size, Vec2 offset, float z_near, float z_far);
		void camera_set_transform(RID camera, const Transform & transform);
		void camera_set_cull_mask(RID camera, uint32_t layers);
		void camera_set_environment(RID camera, RID env);
		void camera_set_camera_effects(RID camera, RID fx);
		void camera_set_use_vertical_aspect(RID camera, bool enable);
		bool is_camera(RID camera) const;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_SCENE_RENDERER_HPP_
