#include <servers/rendering/rendering_server_default.hpp>

#if OPENGL_ENABLED
#include <drivers/opengl/rendering_device_opengl.hpp>
#define RENDERING_DEVICE_DEFAULT RenderingDeviceOpenGL
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

OBJECT_EMBED(RenderingServerDefault, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingServerDefault::RenderingServerDefault() : RenderingServer{}
{
	m_device = memnew(RENDERING_DEVICE_DEFAULT());
}

RenderingServerDefault::~RenderingServerDefault()
{
	m_device = nullptr;
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

	return RD->texture_create({
		TextureType_2D,
		color_format,
		(uint32_t)image->get_width(),
		(uint32_t)image->get_height(),
		1, // depth
		1, // layers
		1, // mipmaps
		SamplerFilter_Linear,
		SamplerFilter_Linear,
		SamplerRepeatMode_ClampToEdge,
		SamplerRepeatMode_ClampToEdge,
		TextureSamples_1,
		TextureFlags_Default,
		color_format_srgb,
		swizzle_r,
		swizzle_g,
		swizzle_b,
		swizzle_a,
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

RID RenderingServerDefault::framebuffer_create(FramebufferSpecification const & spec)
{
	ASSERT(0 < spec.width && 0 < spec.height);

	Vector<RID> textures; textures.reserve(spec.attachments.size());

	for (FramebufferTextureType_ const type : spec.attachments)
	{
		constexpr ColorFormat_ color_format[3] = { ColorFormat_R8G8B8_UNORM, ColorFormat_R8_UNORM, ColorFormat_D24_UNORM_S8_UINT, };

		textures.push_back(RD->texture_create
		({
			TextureType_2D,
			color_format[type],
			spec.width,
			spec.height,
			1, 1, 0,
			SamplerFilter_Linear,
			SamplerFilter_Linear,
			SamplerRepeatMode_ClampToEdge,
			SamplerRepeatMode_ClampToEdge,
			TextureSamples_1,
			TextureFlags_Default
			| (type != FramebufferTextureType_DepthStencil ? TextureFlags_ColorAttachment : TextureFlags_DepthStencilAttachment)
		}));
	}

	return RD->framebuffer_create(std::move(textures));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingServerDefault::begin_scene()
{
}

void RenderingServerDefault::end_scene()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */