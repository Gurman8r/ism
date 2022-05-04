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
	ColorFormat_ color_format{}, color_format_srgb{};
	TextureSwizzle_ swizzle_r{}, swizzle_g{}, swizzle_b{}, swizzle_a{};
	switch (image->get_format())
	{
	case ImageFormat_L8: {
		color_format = ColorFormat_R8_UNORM;
		swizzle_r = TextureSwizzle_R;
		swizzle_g = TextureSwizzle_R;
		swizzle_b = TextureSwizzle_R;
		swizzle_a = TextureSwizzle_One;
	} break;
	case ImageFormat_LA8: {
		color_format = ColorFormat_R8G8_UNORM;
		swizzle_r = TextureSwizzle_R;
		swizzle_g = TextureSwizzle_R;
		swizzle_b = TextureSwizzle_R;
		swizzle_a = TextureSwizzle_G;
	} break;
	case ImageFormat_R8: {
		color_format = ColorFormat_R8_UNORM;
		swizzle_r = TextureSwizzle_R;
		swizzle_g = TextureSwizzle_Zero;
		swizzle_b = TextureSwizzle_Zero;
		swizzle_a = TextureSwizzle_One;
	} break;
	case ImageFormat_RG8: {
		color_format = ColorFormat_R8G8_UNORM;
		swizzle_r = TextureSwizzle_R;
		swizzle_g = TextureSwizzle_G;
		swizzle_b = TextureSwizzle_Zero;
		swizzle_a = TextureSwizzle_One;
	} break;
	case ImageFormat_RGB8: {
		color_format = ColorFormat_R8G8B8_UNORM;
		color_format_srgb = ColorFormat_R8G8B8_SRGB;
		swizzle_r = TextureSwizzle_R;
		swizzle_g = TextureSwizzle_G;
		swizzle_b = TextureSwizzle_B;
		swizzle_a = TextureSwizzle_One;

	} break;
	case ImageFormat_RGBA8: {
		color_format = ColorFormat_R8G8B8A8_UNORM;
		color_format_srgb = ColorFormat_R8G8B8A8_SRGB;
		swizzle_r = TextureSwizzle_R;
		swizzle_g = TextureSwizzle_G;
		swizzle_b = TextureSwizzle_B;
		swizzle_a = TextureSwizzle_A;
	} break;
	}
	return m_device->texture_create(COMPOSE(RD::TextureFormat, ts) {
		ts.color_format = color_format;
		ts.color_format_srgb = color_format_srgb;
		ts.width = (uint32_t)image->get_width();
		ts.height = (uint32_t)image->get_height();
		ts.min_filter = ts.mag_filter = SamplerFilter_Linear;
		ts.repeat_s = ts.repeat_t = SamplerRepeatMode_ClampToEdge;
		ts.swizzle_r = swizzle_r;
		ts.swizzle_g = swizzle_g;
		ts.swizzle_b = swizzle_b;
		ts.swizzle_a = swizzle_a;
	}, image->get_data());
}

RID RenderingServerDefault::texture2d_placeholder_create()
{
	return nullptr;
}

Ref<Image> RenderingServerDefault::texture2d_get_image(RID texture)
{
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::shader_create()
{
	return RID();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingServerDefault::material_create()
{
	return RID();
}

void RenderingServerDefault::material_set_shader(RID material, RID shader)
{
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