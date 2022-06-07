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

		friend class RenderingServerDefault;

	protected:
		explicit RendererCanvasRenderer();

	public:
		virtual ~RendererCanvasRenderer();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_CANVAS_RENDERER_HPP_
