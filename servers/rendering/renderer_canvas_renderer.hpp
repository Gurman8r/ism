#ifndef _ISM_RENDERER_CANVAS_RENDERER_HPP_
#define _ISM_RENDERER_CANVAS_RENDERER_HPP_

#include <servers/rendering_server.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// canvas renderer
	class ISM_API RendererCanvasRenderer : public Object
	{
		OBJECT_COMMON(RendererCanvasRenderer, Object);

		static RendererCanvasRenderer * singleton;

		friend class RenderingServerDefault;

	protected:
		explicit RendererCanvasRenderer();

	public:
		virtual ~RendererCanvasRenderer();

		FORCE_INLINE static RendererCanvasRenderer * get_singleton() noexcept { return singleton; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define CANVAS_RENDERER (ism::RendererCanvasRenderer::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_CANVAS_RENDERER_HPP_
