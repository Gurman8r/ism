#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(RendererStorage, t) {}

RendererStorage::RendererStorage()
{
}

RendererStorage::~RendererStorage()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::render_target_create()
{
	RenderTarget * rt{ memnew(RenderTarget) };

	if (!rt->texture) {
	}

	rt->color_format = ColorFormat_R8G8B8_UNORM;
	rt->color_format_srgb = ColorFormat_R8G8B8_UNORM;

	rt->color = RD::get_singleton()->texture_create
	({
		TextureType_2D,
		rt->color_format,
		(uint32_t)rt->size[0],
		(uint32_t)rt->size[1],
		1, 1, 1,
		SamplerFilter_Linear,
		SamplerFilter_Linear,
		SamplerRepeatMode_ClampToEdge,
		SamplerRepeatMode_ClampToEdge,
		TextureSamples_1,
		TextureFlags_Sampling | TextureFlags_ColorAttachment | TextureFlags_CanCopyFrom
	});

	rt->framebuffer = RD::get_singleton()->framebuffer_create({ rt->color });

	return (RID)rt;
}

void RendererStorage::render_target_destroy(RID rid)
{
	ASSERT(rid);

	RenderTarget * render_target{ (RenderTarget *)rid };

	memdelete(render_target);
}

void RendererStorage::render_target_resize(RID rid, int32_t width, int32_t height)
{
	ASSERT(rid);

	RenderTarget * render_target{ (RenderTarget *)rid };

	RD::get_singleton()->framebuffer_resize(render_target->framebuffer, width, height);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */