#ifndef _ISM_RENDERER_VIEWPORT_HPP_
#define _ISM_RENDERER_VIEWPORT_HPP_

#include <servers/rendering/framebuffer.hpp>

namespace ism
{
	class ISM_API RendererViewport : public Object
	{
		OBJECT_COMMON(RendererViewport, Object);

		struct Viewport
		{
		};

		Vector<Viewport *> m_active_viewports{};

	public:
		RendererViewport();

		virtual ~RendererViewport();

	public:
		RID viewport_create();

		void viewport_destroy(RID rid);

		void viewport_resize(RID rid, int32_t width, int32_t height);

		RID viewport_get_texture(RID rid) const;

		void draw_viewports();
	};
}

#endif // !_ISM_RENDERER_VIEWPORT_HPP_
