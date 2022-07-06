#include <servers/rendering/renderer_scene_renderer.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(RendererSceneRenderer, t) {}

	RendererSceneRenderer::RendererSceneRenderer(RenderingDevice * device, RendererStorage * storage) : m_device{ VALIDATE(device) }, m_storage{ VALIDATE(storage) }
	{
	}

	RendererSceneRenderer::~RendererSceneRenderer()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID RendererSceneRenderer::camera_create()
	{
		return RID();
	}

	void RendererSceneRenderer::camera_set_perspective(RID camera, float fovy_degrees, float z_near, float z_far)
	{
	}

	void RendererSceneRenderer::camera_set_orthogonal(RID camera, float size, float z_near, float z_far)
	{
	}

	void RendererSceneRenderer::camera_set_frustum(RID camera, float size, Vec2 offset, float z_near, float z_far)
	{
	}

	void RendererSceneRenderer::camera_set_transform(RID camera, const Transform & transform)
	{
	}

	void RendererSceneRenderer::camera_set_cull_mask(RID camera, uint32_t layers)
	{
	}

	void RendererSceneRenderer::camera_set_environment(RID camera, RID env)
	{
	}

	void RendererSceneRenderer::camera_set_camera_effects(RID camera, RID fx)
	{
	}

	void RendererSceneRenderer::camera_set_use_vertical_aspect(RID camera, bool enable)
	{
	}

	bool RendererSceneRenderer::is_camera(RID camera) const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
