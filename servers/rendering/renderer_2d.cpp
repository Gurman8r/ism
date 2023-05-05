#include <servers/rendering/renderer_2d.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Renderer2D, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Renderer2D::Renderer2D(RenderingDevice * device, RendererStorage * storage) : m_device{ VALIDATE(device) }, m_storage{ VALIDATE(storage) }
	{
	}

	Renderer2D::~Renderer2D()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}