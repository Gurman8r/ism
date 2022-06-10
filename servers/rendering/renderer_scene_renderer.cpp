#include <servers/rendering/renderer_scene_renderer.hpp>

using namespace ism;

OBJECT_EMBED(RendererSceneRenderer, t) {}

RendererSceneRenderer::RendererSceneRenderer(RenderingDevice * device, RendererStorage * storage) : m_device{ VALIDATE(device) }, m_storage{ VALIDATE(storage) }
{
}

RendererSceneRenderer::~RendererSceneRenderer()
{
}
