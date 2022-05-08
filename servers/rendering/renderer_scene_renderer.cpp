#include <servers/rendering/renderer_scene_renderer.hpp>

using namespace ism;

MEMBER_IMPL(RendererSceneRenderer::singleton) {};

OBJECT_EMBED(RendererSceneRenderer, t) {}

RendererSceneRenderer::RendererSceneRenderer()
{
	singleton = this;
}

RendererSceneRenderer::~RendererSceneRenderer()
{
}

void RendererSceneRenderer::render_scene()
{
}
