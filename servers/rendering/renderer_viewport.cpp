#include <servers/rendering/renderer_viewport.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

OBJECT_EMBED(RendererViewport, t) {}

RendererViewport::RendererViewport()
{
}

RendererViewport::~RendererViewport()
{
}

RID RendererViewport::viewport_create()
{
	Viewport * viewport{ memnew(Viewport) };
	viewport->self = (RID)viewport;
	viewport->parent = nullptr;
	viewport->render_target = nullptr;
	return (RID)viewport;
}

void RendererViewport::viewport_destroy(RID rid)
{
	ASSERT(rid);
	Viewport * viewport{ (Viewport *)rid };
	memdelete(viewport);
}

void RendererViewport::viewport_resize(RID rid, int32_t width, int32_t height)
{
}

RID RendererViewport::viewport_get_texture(RID rid) const
{
	return nullptr;
}

void RendererViewport::viewport_attach_to_screen(RID rid, IntRect const & rect, WindowID screen)
{
}

void RendererViewport::viewport_set_clear_color(RID rid, Color const & value)
{
	ASSERT(rid);

	Viewport * viewport{ (Viewport *)rid };
}

void RendererViewport::draw_viewports()
{
	for (Viewport * viewport : m_active_viewports)
	{
	}
}