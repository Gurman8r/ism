#include <servers/rendering/renderer_canvas_renderer.hpp>

using namespace ism;

MEMBER_IMPL(RendererCanvasRenderer::singleton) {};

OBJECT_EMBED(RendererCanvasRenderer, t) {}

RendererCanvasRenderer::RendererCanvasRenderer()
{
	singleton = this;
}

RendererCanvasRenderer::~RendererCanvasRenderer()
{
}