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

RID RendererStorage::texture2d_placeholder_create()
{
	return RID();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::material_create()
{
	_Material * m{ memnew(_Material{}) };
	m->uniform_buffer = nullptr;
	m->uniform_set = RENDERING_DEVICE->uniform_set_create({}, nullptr);
	return (RID)m;
}

void RendererStorage::material_destroy(RID material)
{
	_Material * const m{ VALIDATE((_Material *)material) };
	if (m->uniform_buffer) { RENDERING_DEVICE->buffer_destroy(m->uniform_buffer); }
	if (m->uniform_set) { RENDERING_DEVICE->uniform_set_destroy(m->uniform_set); }
	memdelete(m);
}

void RendererStorage::material_set_shader(RID material, RID shader)
{
	_Material * const m{ VALIDATE((_Material *)material) };
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::mesh_create()
{
	_Mesh * m{ memnew(_Mesh{}) };
	return (RID)m;
}

void RendererStorage::mesh_destroy(RID mesh)
{
	_Mesh * const m{ VALIDATE((_Mesh *)mesh) };
	memdelete(m);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::render_target_create()
{
	_RenderTarget * const rt{ memnew(_RenderTarget) };

	if (!rt->texture) { rt->texture = texture2d_placeholder_create(); }

	rt->color_format = RD::DataFormat_R8G8B8_UNORM;
	rt->color_format_srgb = RD::DataFormat_R8G8B8_UNORM;
	
	rt->color = RENDERING_DEVICE->texture_create(COMPOSE(RD::TextureFormat, t) {
		t.color_format = rt->color_format;
		t.color_format_srgb = rt->color_format_srgb;
		t.width = (uint32_t)rt->size[0];
		t.width = (uint32_t)rt->size[1];
		t.usage_flags = RD::TextureFlags_Sampling | RD::TextureFlags_ColorAttachment | RD::TextureFlags_CanCopyFrom;
	});

	rt->framebuffer = RENDERING_DEVICE->framebuffer_create({ rt->color });

	return (RID)rt;
}

void RendererStorage::render_target_destroy(RID render_target)
{
	ASSERT(render_target);

	_RenderTarget * const rt{ (_RenderTarget *)render_target };

	memdelete(rt);
}

void RendererStorage::render_target_bind(RID render_target)
{
	_RenderTarget * const rt{ VALIDATE((_RenderTarget *)render_target) };

	RENDERING_DEVICE->framebuffer_bind(rt->framebuffer);
}

void RendererStorage::render_target_set_position(RID render_target, int32_t x, int32_t  y)
{
	_RenderTarget * const rt{ VALIDATE((_RenderTarget *)render_target) };
}

void RendererStorage::render_target_set_size(RID render_target, int32_t width, int32_t height)
{
	_RenderTarget * const rt{ VALIDATE((_RenderTarget *)render_target) };

	RENDERING_DEVICE->framebuffer_set_size(rt->framebuffer, width, height);
}

RID RendererStorage::render_target_get_texture(RID render_target)
{
	_RenderTarget * const rt{ VALIDATE((_RenderTarget *)render_target) };

	if (!rt->texture) { rt->texture = texture2d_placeholder_create(); }

	return rt->texture;
}

Vec2i RendererStorage::render_target_get_position(RID render_target)
{
	return { 0, 0 };
}

Vec2i RendererStorage::render_target_get_size(RID render_target)
{
	return VALIDATE((_RenderTarget *)render_target)->size;
}

void RendererStorage::render_target_request_clear(RID render_target, Color const & value)
{
	_RenderTarget * const rt{ VALIDATE((_RenderTarget *)render_target) };
	rt->clear_color = value;
	rt->clear_requested = true;
}

void RendererStorage::render_target_disable_clear_request(RID render_target)
{
	_RenderTarget * const rt{ VALIDATE((_RenderTarget *)render_target) };
	rt->clear_requested = false;
}

bool RendererStorage::render_target_is_clear_requested(RID render_target)
{
	return VALIDATE((_RenderTarget *)render_target)->clear_requested;
}

Color RendererStorage::render_target_get_clear_request_color(RID render_target)
{
	return VALIDATE((_RenderTarget *)render_target)->clear_color;
}

void RendererStorage::render_target_do_clear_request(RID render_target)
{
	_RenderTarget * const rt{ VALIDATE((_RenderTarget *)render_target) };
	if (!rt->clear_requested) { return; }
	RENDERING_DEVICE->draw_list_begin(rt->framebuffer, rt->clear_color);
	RENDERING_DEVICE->draw_list_end();
	rt->clear_requested = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */