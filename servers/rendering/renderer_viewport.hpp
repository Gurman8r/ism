#ifndef _ISM_RENDERER_VIEWPORT_HPP_
#define _ISM_RENDERER_VIEWPORT_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// renderer viewport
	class ISM_API RendererViewport : public Object
	{
		OBJECT_COMMON(RendererViewport, Object);

		static RendererViewport * singleton;

		friend class RenderingServerDefault;

	protected:
		explicit RendererViewport();

	public:
		virtual ~RendererViewport();

		FORCE_INLINE static RendererViewport * get_singleton() noexcept { return singleton; }

	public:
		struct Viewport
		{
			RID self{}, parent{};

			Vec2i size{};

			RID camera{};

			RID render_target{}, render_target_texture{};
		};

		RID viewport_create();

		void viewport_destroy(RID viewport);
		
		void viewport_set_parent_viewport(RID viewport, RID parent_viewport);

		void viewport_set_size(RID viewport, int32_t width, int32_t height);

		RID viewport_get_texture(RID viewport) const;
		
		void viewport_attach_to_screen(RID viewport, IntRect const & rect, WindowID screen);
		
		void draw_viewports();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define RENDERER_VIEWPORT (ism::RendererViewport::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

}

#endif // !_ISM_RENDERER_VIEWPORT_HPP_
