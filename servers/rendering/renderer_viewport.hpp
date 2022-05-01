#ifndef _ISM_RENDERER_VIEWPORT_HPP_
#define _ISM_RENDERER_VIEWPORT_HPP_

#include <servers/rendering/render_target.hpp>

namespace ism
{
	class ISM_API RendererViewport : public Object
	{
		OBJECT_COMMON(RendererViewport, Object);

	public:
		RendererViewport();

		virtual ~RendererViewport();

	public:
		struct Viewport final
		{
			RID self{}, parent{};

			Vec2i size{};

			RID camera{};

			RID render_target{}, render_target_texture{}, render_buffers{};
		};

		RID viewport_create();
		
		void viewport_destroy(RID rid);
		
		void viewport_resize(RID rid, int32_t width, int32_t height);
		
		RID viewport_get_texture(RID rid) const;
		
		void viewport_attach_to_screen(RID rid, IntRect const & rect, WindowID screen);
		
		void viewport_set_clear_color(RID rid, Color const & value);
		
		void draw_viewports();
	};
}

#endif // !_ISM_RENDERER_VIEWPORT_HPP_
