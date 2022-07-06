#include <servers/rendering/renderer_3d.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(Renderer3D, t) {}

	Renderer3D::Renderer3D(RenderingDevice * device, RendererStorage * storage) : m_device{ VALIDATE(device) }, m_storage{ VALIDATE(storage) }
	{
	}

	Renderer3D::~Renderer3D()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	RID Renderer3D::camera_create()
	{
		return RID();
	}

	void Renderer3D::camera_set_perspective(RID camera, float fovy_degrees, float z_near, float z_far)
	{
	}

	void Renderer3D::camera_set_orthogonal(RID camera, float size, float z_near, float z_far)
	{
	}

	void Renderer3D::camera_set_frustum(RID camera, float size, Vec2 offset, float z_near, float z_far)
	{
	}

	void Renderer3D::camera_set_transform(RID camera, const Transform & transform)
	{
	}

	void Renderer3D::camera_set_cull_mask(RID camera, uint32_t layers)
	{
	}

	void Renderer3D::camera_set_environment(RID camera, RID env)
	{
	}

	void Renderer3D::camera_set_camera_effects(RID camera, RID fx)
	{
	}

	void Renderer3D::camera_set_use_vertical_aspect(RID camera, bool enable)
	{
	}

	bool Renderer3D::is_camera(RID camera) const
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
