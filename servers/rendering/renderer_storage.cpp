#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(RendererStorage::singleton) {};

OBJECT_EMBED(RendererStorage, t) {}

RendererStorage::RendererStorage()
{
	singleton = this;
}

RendererStorage::~RendererStorage()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::material_create()
{
	return RID();
}

void RendererStorage::material_set_shader(RID material, RID shader)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::mesh_create()
{
	return RID();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::render_target_create()
{
	RenderTarget * const rt{ memnew(RenderTarget) };

	if (!rt->texture) {
		// create placeholder
	}

	rt->color_format = ColorFormat_R8G8B8_UNORM;
	rt->color_format_srgb = ColorFormat_R8G8B8_UNORM;
	
	rt->color = RENDERING_DEVICE->texture_create(COMPOSE(RD::TextureFormat, ts) {
		ts.color_format = rt->color_format;
		ts.color_format_srgb = rt->color_format_srgb;
		ts.width = (uint32_t)rt->size[0];
		ts.width = (uint32_t)rt->size[1];
		ts.usage_flags = TextureFlags_Sampling | TextureFlags_ColorAttachment | TextureFlags_CanCopyFrom;
	});

	rt->framebuffer = RENDERING_DEVICE->framebuffer_create({ rt->color });

	return (RID)rt;
}

void RendererStorage::render_target_destroy(RID rid)
{
	ASSERT(rid);

	RenderTarget * const rt{ (RenderTarget *)rid };

	memdelete(rt);
}

void RendererStorage::render_target_bind(RID rid)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)rid) };

	RENDERING_DEVICE->framebuffer_bind(rt->framebuffer);

	if (rt->clear_requested) { RENDERING_DEVICE->clear(rt->clear_color); }
}

void RendererStorage::render_target_set_position(RID rid, int32_t x, int32_t  y)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)rid) };
}

void RendererStorage::render_target_set_size(RID rid, int32_t width, int32_t height)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)rid) };

	RENDERING_DEVICE->framebuffer_set_size(rt->framebuffer, width, height);
}

RID RendererStorage::render_target_get_texture(RID rid)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)rid) };

	if (!rt->texture) {
		rt->texture = nullptr;
	}

	return rt->texture;
}

Vec2i RendererStorage::render_target_get_position(RID rid)
{
	return { 0, 0 };
}

Vec2i RendererStorage::render_target_get_size(RID rid)
{
	return VALIDATE((RenderTarget *)rid)->size;
}

void RendererStorage::render_target_request_clear(RID rid, Color const & value)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)rid) };
	rt->clear_color = value;
	rt->clear_requested = true;
}

void RendererStorage::render_target_disable_clear_request(RID rid)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)rid) };
	rt->clear_requested = false;
}

bool RendererStorage::render_target_is_clear_requested(RID rid)
{
	return VALIDATE((RenderTarget *)rid)->clear_requested;
}

Color RendererStorage::render_target_get_clear_request_color(RID rid)
{
	return VALIDATE((RenderTarget *)rid)->clear_color;
}

void RendererStorage::render_target_do_clear_request(RID rid)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)rid) };
	if (!rt->clear_requested) { return; }
	RENDERING_DEVICE->drawlist_begin(rt->framebuffer, rt->clear_color);
	RENDERING_DEVICE->drawlist_end();
	rt->clear_requested = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */