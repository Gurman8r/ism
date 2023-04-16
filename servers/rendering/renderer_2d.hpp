#ifndef _ISM_RENDERER_CANVAS_RENDERER_HPP_
#define _ISM_RENDERER_CANVAS_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// canvas renderer
	class ISM_API Renderer2D : public Object
	{
		DEFINE_CLASS(Renderer2D, Object);
		
		friend class RenderingServerDefault;

		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

	public:
		Renderer2D(RenderingDevice * device, RendererStorage * storage);

		virtual ~Renderer2D() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_CANVAS_RENDERER_HPP_
