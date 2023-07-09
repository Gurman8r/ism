#ifndef _ISM_RENDERER_CANVAS_RENDERER_HPP_
#define _ISM_RENDERER_CANVAS_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// canvas renderer
	class ISM_API Renderer2D : public Object
	{
		OBJECT_CLASS(Renderer2D, Object);
		
		friend class DefaultRenderingServer;

		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

	public:
		Renderer2D(RenderingDevice * device, RendererStorage * storage);

		virtual ~Renderer2D() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_CANVAS_RENDERER_HPP_
