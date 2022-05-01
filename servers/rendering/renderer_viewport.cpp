#include <servers/rendering/renderer_viewport.hpp>

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
	return nullptr;
}

void RendererViewport::viewport_destroy(RID rid)
{
}

void RendererViewport::draw_viewports()
{
}