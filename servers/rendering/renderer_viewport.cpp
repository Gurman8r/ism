#include <servers/rendering/renderer_viewport.hpp>
#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

MEMBER_IMPL(RendererViewport::singleton) {};

OBJECT_EMBED(RendererViewport, t) {}

RendererViewport::RendererViewport()
{
	singleton = this;
}

RendererViewport::~RendererViewport()
{
}

RID RendererViewport::viewport_create()
{
	Viewport * const vp{ memnew(Viewport) };
	vp->self = (RID)vp;
	vp->parent = nullptr;
	vp->size = { 1280, 720 };
	vp->camera = nullptr;
	vp->render_target = RENDERER_STORAGE->render_target_create();
	vp->render_target_texture = nullptr;
	return (RID)vp;
}

void RendererViewport::viewport_destroy(RID viewport)
{
	Viewport * const vp{ VALIDATE((Viewport *)viewport) };
	RENDERER_STORAGE->render_target_destroy(vp->render_target);
	memdelete(vp);
}

void RendererViewport::viewport_set_parent_viewport(RID viewport, RID parent_viewport)
{
	Viewport * const vp{ VALIDATE((Viewport *)viewport) };
	vp->parent = parent_viewport;
}

void RendererViewport::viewport_set_size(RID viewport, int32_t width, int32_t height)
{
	Viewport * const vp{ VALIDATE((Viewport *)viewport) };
	vp->size = { width, height };
}

RID RendererViewport::viewport_get_texture(RID viewport) const
{
	return VALIDATE((Viewport *)viewport)->render_target_texture;
}

void RendererViewport::viewport_attach_to_screen(RID viewport, IntRect const & rect, WindowID screen)
{
}

void RendererViewport::draw_viewports()
{
}