#include <servers/rendering/renderer_canvas_renderer.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RendererCanvasRenderer::RendererCanvasRenderer(RenderingDevice * device, RendererStorage * storage)
		: m_device{ VALIDATE(device) }
		, m_storage{ VALIDATE(storage) }
	{
	}

	RendererCanvasRenderer::~RendererCanvasRenderer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}