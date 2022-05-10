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
	, m_storage			{ memnew(RendererStorage) }
	, m_viewport		{ memnew(RendererViewport) }
	, m_canvas_renderer	{ memnew(RendererCanvasRenderer) }
	, m_scene_renderer	{ memnew(RendererSceneRenderer) }
{
}

RenderingServerDefault::~RenderingServerDefault()
{
	if (m_scene_renderer) { memdelete(m_scene_renderer); m_scene_renderer = nullptr; }
	if (m_canvas_renderer) { memdelete(m_canvas_renderer); m_canvas_renderer = nullptr; }
	if (m_viewport) { memdelete(m_viewport); m_viewport = nullptr; }
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
	return nullptr;
}

Ref<Image> RenderingServerDefault::texture2d_get_image(RID texture)
{
	if (!texture) { return nullptr; }

	DynamicBuffer data{ m_device->texture_get_data(texture) };

	Ref<Image> image;

	return image;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::shader_create()
{
	return RID();
}

void RenderingServerDefault::shader_destroy(RID shader)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::material_create()
{
	return m_storage->material_create();
}

void RenderingServerDefault::material_destroy(RID material)
{
	m_storage->material_destroy(material);
}

void RenderingServerDefault::material_set_shader(RID material, RID shader)
{
	m_storage->material_set_shader(material, shader);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::mesh_create()
{
	return RID();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::camera_create()
{
	return RID();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::viewport_create()
{
	return m_viewport->viewport_create();
}

void RenderingServerDefault::viewport_destroy(RID viewport)
{
	VALIDATE(m_viewport)->viewport_destroy(viewport);
}

void RenderingServerDefault::viewport_set_parent_viewport(RID viewport, RID parent_viewport)
{
}

void RenderingServerDefault::viewport_set_size(RID viewport, int32_t width, int32_t height)
{
}

RID RenderingServerDefault::viewport_get_texture(RID viewport) const
{
	return RID();
}

void RenderingServerDefault::viewport_attach_to_screen(RID viewport, IntRect const & rect, WindowID screen)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */