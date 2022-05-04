#ifndef _ISM_RENDERER_STORAGE_HPP_
#define _ISM_RENDERER_STORAGE_HPP_

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// renderer storage
	class ISM_API RendererStorage : public Object
	{
		OBJECT_COMMON(RendererStorage, Object);

		static RendererStorage * singleton;

	public:
		explicit RendererStorage();

		virtual ~RendererStorage();

		FORCE_INLINE static RendererStorage * get_singleton() noexcept { return singleton; }

	public:
		/* MATERIAL */
		struct Material final
		{

		};

		RID material_create();

		void material_set_shader(RID material, RID shader);

	public:
		/* MESH */
		struct Mesh final
		{

		};

		RID mesh_create();

	public:
		/* RENDER TARGET */
		struct RenderTarget final
		{
			Vec2i size{};

			RID framebuffer{}, backbuffer{};

			RID color{};

			ColorFormat_ color_format{}, color_format_srgb{};

			RID texture{};

			RID framebuffer_uniforms{}, backbuffer_uniforms{};

			bool clear_requested{};

			Color clear_color{};
		};

		RID render_target_create();
		void render_target_destroy(RID rid);
		void render_target_bind(RID rid);

		void render_target_set_position(RID rid, int32_t x, int32_t  y);
		void render_target_set_size(RID rid, int32_t width, int32_t height);

		RID render_target_get_texture(RID rid);
		Vec2i render_target_get_position(RID rid);
		Vec2i render_target_get_size(RID rid);

		void render_target_request_clear(RID rid, Color const & value);
		void render_target_disable_clear_request(RID rid);
		bool render_target_is_clear_requested(RID rid);
		Color render_target_get_clear_request_color(RID rid);
		void render_target_do_clear_request(RID rid);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define RENDERER_STORAGE (ism::RendererStorage::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_STORAGE_HPP_
