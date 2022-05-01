#ifndef _ISM_RENDERER_STORAGE_HPP_
#define _ISM_RENDERER_STORAGE_HPP_

#include <servers/rendering/rendering_device.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API RendererStorage : public Object
	{
		OBJECT_COMMON(RendererStorage, Object);

	public:
		RendererStorage();

		virtual ~RendererStorage();

	public:
		/* RENDER TARGET */
		struct RenderTarget final
		{
			Vec2i size{};

			RID framebuffer{}, backbuffer{};

			RID color{};

			RID texture{};

			RID framebuffer_uniforms{}, backbuffer_uniforms{};

			bool clear_requested{};

			Color clear_color{};
		};

		RID render_target_create();
		void render_target_destroy(RID rid);
		void render_target_resize(RID rid, int32_t width, int32_t height);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_STORAGE_HPP_
