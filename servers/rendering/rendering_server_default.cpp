#include <servers/rendering/rendering_server_default.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

OBJECT_EMBED(RenderingServerDefault, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingServerDefault::RenderingServerDefault()
	: RenderingServer	{}
	, m_device			{ memnew(RENDERING_DEVICE_DEFAULT()) }
	, m_storage			{ memnew(RendererStorage(m_device)) }
	, m_canvas_renderer	{ memnew(RendererCanvasRenderer(m_device, m_storage)) }
	, m_scene_renderer	{ memnew(RendererSceneRenderer(m_device, m_storage)) }
{
}

RenderingServerDefault::~RenderingServerDefault()
{
	if (m_scene_renderer) { memdelete(m_scene_renderer); m_scene_renderer = nullptr; }
	if (m_canvas_renderer) { memdelete(m_canvas_renderer); m_canvas_renderer = nullptr; }
	if (m_storage) { memdelete(m_storage); m_storage = nullptr; }
	if (m_device) { memdelete(m_device); m_device = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingServerDefault::initialize()
{
}

void RenderingServerDefault::finalize()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingServerDefault::texture_destroy(RID texture)
{
	m_device->texture_destroy(texture);
}

RID RenderingServerDefault::texture_replace(RID old_texture, RID new_texture)
{
	return {};
}

RID RenderingServerDefault::texture2d_create(Ref<Image> const & image)
{
	if (!image) { return nullptr; }
	RD::DataFormat_ color_format{}, color_format_srgb{};
	RD::TextureSwizzle_ swizzle_r{}, swizzle_g{}, swizzle_b{}, swizzle_a{};
	switch (image->get_format())
	{
	case Image::Format_L8: {
		color_format = RD::DataFormat_R8_UNORM;
		swizzle_r = RD::TextureSwizzle_R;
		swizzle_g = RD::TextureSwizzle_R;
		swizzle_b = RD::TextureSwizzle_R;
		swizzle_a = RD::TextureSwizzle_One;
	} break;
	case Image::Format_LA8: {
		color_format = RD::DataFormat_R8G8_UNORM;
		swizzle_r = RD::TextureSwizzle_R;
		swizzle_g = RD::TextureSwizzle_R;
		swizzle_b = RD::TextureSwizzle_R;
		swizzle_a = RD::TextureSwizzle_G;
	} break;
	case Image::Format_R8: {
		color_format = RD::DataFormat_R8_UNORM;
		swizzle_r = RD::TextureSwizzle_R;
		swizzle_g = RD::TextureSwizzle_Zero;
		swizzle_b = RD::TextureSwizzle_Zero;
		swizzle_a = RD::TextureSwizzle_One;
	} break;
	case Image::Format_RG8: {
		color_format = RD::DataFormat_R8G8_UNORM;
		swizzle_r = RD::TextureSwizzle_R;
		swizzle_g = RD::TextureSwizzle_G;
		swizzle_b = RD::TextureSwizzle_Zero;
		swizzle_a = RD::TextureSwizzle_One;
	} break;
	case Image::Format_RGB8: {
		color_format = RD::DataFormat_R8G8B8_UNORM;
		color_format_srgb = RD::DataFormat_R8G8B8_SRGB;
		swizzle_r = RD::TextureSwizzle_R;
		swizzle_g = RD::TextureSwizzle_G;
		swizzle_b = RD::TextureSwizzle_B;
		swizzle_a = RD::TextureSwizzle_One;

	} break;
	case Image::Format_RGBA8: {
		color_format = RD::DataFormat_R8G8B8A8_UNORM;
		color_format_srgb = RD::DataFormat_R8G8B8A8_SRGB;
		swizzle_r = RD::TextureSwizzle_R;
		swizzle_g = RD::TextureSwizzle_G;
		swizzle_b = RD::TextureSwizzle_B;
		swizzle_a = RD::TextureSwizzle_A;
	} break;
	}
	return m_device->texture_create(COMPOSE(RD::TextureCreateInfo, t) {
		t.color_format = color_format;
		t.color_format_srgb = color_format_srgb;
		t.width = (uint32_t)image->get_width();
		t.height = (uint32_t)image->get_height();
		t.depth = (uint32_t)image->get_depth();
		t.min_filter = t.mag_filter = RD::SamplerFilter_Linear;
		t.repeat_s = t.repeat_t = RD::SamplerRepeatMode_ClampToEdge;
		t.swizzle_r = swizzle_r;
		t.swizzle_g = swizzle_g;
		t.swizzle_b = swizzle_b;
		t.swizzle_a = swizzle_a;
	}, image->get_data());
}

RID RenderingServerDefault::texture2d_placeholder_create()
{
	return m_storage->texture2d_placeholder_create();
}

Ref<Image> RenderingServerDefault::texture2d_get_image(RID texture)
{
	if (!texture) { return nullptr; }

	DynamicBuffer data{ m_device->texture_get_data(texture) };

	Ref<Image> image;

	return image;
}

RID RenderingServerDefault::texture3d_placeholder_create()
{
	return m_storage->texture3d_placeholder_create();
}

RID RenderingServerDefault::texturecube_placeholder_create()
{
	return m_storage->texturecube_placeholder_create();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::shader_create()
{
	return RID();
}

RID RenderingServerDefault::shader_placeholder_create()
{
	return m_storage->shader_placeholder_create();
}

void RenderingServerDefault::shader_destroy(RID shader)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::material_create()
{
	return m_storage->material_create();
}

RID RenderingServerDefault::material_placeholder_create()
{
	return m_storage->material_placeholder_create();
}

void RenderingServerDefault::material_destroy(RID material)
{
	m_storage->material_destroy(material);
}

RID RenderingServerDefault::material_get_shader(RID material) const
{
	return m_storage->material_get_shader(material);
}

void RenderingServerDefault::material_set_shader(RID material, RID shader)
{
	m_storage->material_set_shader(material, shader);
}

Variant RenderingServerDefault::material_get_param(RID material, StringName const & key) const
{
	return m_storage->material_get_param(material, key);
}

void RenderingServerDefault::material_set_param(RID material, StringName const & key, Variant const & value)
{
	m_storage->material_set_param(material, key, value);
}

void RenderingServerDefault::material_update(RID material, Map<StringName, Variant> const & params)
{
	m_storage->material_update(material, params);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::mesh_create(Vector<SurfaceData> const & surfaces)
{
	return m_storage->mesh_create(surfaces);
}

RID RenderingServerDefault::mesh_placeholder_create()
{
	return m_storage->mesh_placeholder_create();
}

void RenderingServerDefault::mesh_destroy(RID mesh)
{
	m_storage->mesh_destroy(mesh);
}

void RenderingServerDefault::mesh_clear(RID mesh)
{
	m_storage->mesh_clear(mesh);
}

size_t RenderingServerDefault::mesh_get_surface_count(RID mesh)
{
	return m_storage->mesh_get_surface_count(mesh);
}

void RenderingServerDefault::mesh_add_surface(RID mesh, SurfaceData const & surface)
{
	m_storage->mesh_add_surface(mesh, surface);
}

RS::Primitive_ RenderingServerDefault::mesh_surface_get_primitive(RID mesh, size_t index)
{
	return m_storage->mesh_surface_get_primitive(mesh, index);
}

RID RenderingServerDefault::mesh_surface_get_vertex_array(RID mesh, size_t index)
{
	return m_storage->mesh_surface_get_vertex_array(mesh, index);
}

RID RenderingServerDefault::mesh_surface_get_index_array(RID mesh, size_t index)
{
	return m_storage->mesh_surface_get_index_array(mesh, index);
}

RID RenderingServerDefault::mesh_surface_get_material(RID mesh, size_t index)
{
	return m_storage->mesh_surface_get_material(mesh, index);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::camera_create(Vec3 const & position, Vec4 const & rotation)
{
	return m_storage->camera_create(position, rotation);
}

void RenderingServerDefault::camera_destroy(RID camera)
{
	m_storage->camera_destroy(camera);
}

Vec3 RenderingServerDefault::camera_get_position(RID camera)
{
	return m_storage->camera_get_position(camera);
}

void RenderingServerDefault::camera_set_position(RID camera, Vec3 const & value)
{
	m_storage->camera_set_position(camera, value);
}

Vec4 RenderingServerDefault::camera_get_rotation(RID camera)
{
	return m_storage->camera_get_rotation(camera);
}

void RenderingServerDefault::camera_set_rotation(RID camera, Vec4 const & value)
{
	m_storage->camera_set_rotation(camera, value);
}

Mat4 RenderingServerDefault::camera_get_transform(RID camera)
{
	return m_storage->camera_get_transform(camera);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::viewport_create()
{
	return m_storage->viewport_create();
}

void RenderingServerDefault::viewport_destroy(RID viewport)
{
	m_storage->viewport_destroy(viewport);
}

void RenderingServerDefault::viewport_set_parent_viewport(RID viewport, RID parent_viewport)
{
	m_storage->viewport_set_parent_viewport(viewport, parent_viewport);
}

void RenderingServerDefault::viewport_set_size(RID viewport, int32_t width, int32_t height)
{
	m_storage->viewport_set_size(viewport, width, height);
}

RID RenderingServerDefault::viewport_get_texture(RID viewport) const
{
	return m_storage->viewport_get_texture(viewport);
}

void RenderingServerDefault::viewport_attach_to_screen(RID viewport, IntRect const & rect, WindowID screen)
{
	m_storage->viewport_attach_to_screen(viewport, rect, screen);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */