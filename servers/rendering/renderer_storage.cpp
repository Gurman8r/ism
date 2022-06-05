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
	Material * m{ memnew(Material{}) };
	m->shader = nullptr;
	m->uniform_buffer = nullptr;
	m->uniform_set = RENDERING_DEVICE->uniform_set_create({}, nullptr);
	return (RID)m;
}

void RendererStorage::material_destroy(RID material)
{
	Material * const m{ VALIDATE((Material *)material) };
	if (m->uniform_buffer) { RENDERING_DEVICE->buffer_destroy(m->uniform_buffer); }
	if (m->uniform_set) { RENDERING_DEVICE->uniform_set_destroy(m->uniform_set); }
	memdelete(m);
}

void RendererStorage::material_set_shader(RID material, RID shader)
{
	Material * const m{ VALIDATE((Material *)material) };
	if (m->shader == shader) { return; }
	m->shader = shader;
}

void RendererStorage::material_set_param(RID material, String const & key, OBJ const & value)
{
	Material * const m{ VALIDATE((Material *)material) };
}

OBJ RendererStorage::material_get_param(RID material, String const & key)
{
	Material * const m{ VALIDATE((Material *)material) };
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::mesh_create(Vector<RS::SurfaceData> const & surfaces)
{
	Mesh * const m{ memnew(Mesh{}) };

	for (RS::SurfaceData const & surface : surfaces)
	{
		mesh_add_surface((RID)m, surface);
	}

	return (RID)m;
}

void RendererStorage::mesh_destroy(RID mesh)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };

	for (Mesh::Surface * s : m->surfaces)
	{
		ASSERT(s);
		if (s->vertex_array) { RENDERING_DEVICE->vertex_array_destroy(s->vertex_array); }
		if (s->index_array) { RENDERING_DEVICE->index_array_destroy(s->index_array); }
		if (s->uniform_set) { RENDERING_DEVICE->uniform_set_destroy(s->uniform_set); }
		memdelete(s);
	}

	memdelete(m);
}

void RendererStorage::mesh_clear(RID mesh)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };
	m->surfaces.clear();
}

void RendererStorage::mesh_add_surface(RID mesh, RS::SurfaceData const & surface)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };
	Mesh::Surface * const s{ m->surfaces.emplace_back(memnew(Mesh::Surface{})) };
	s->primitive = surface.primitive;

	// vertices
	RID vb{ RENDERING_DEVICE->vertex_buffer_create(surface.vertex_data.size(), surface.vertex_data) };
	RID va{ RENDERING_DEVICE->vertex_array_create(surface.vertex_count, {}, { vb }) };
	s->vertex_array = va;
	s->vertex_count = surface.vertex_count;
	s->vertex_buffer_size = surface.vertex_data.size();

	// indices
	RID ib{ RENDERING_DEVICE->index_buffer_create(surface.index_count, RD::IndexbufferFormat_U32, surface.index_data) };
	RID ia{ RENDERING_DEVICE->index_array_create(ib, 0, surface.index_count) };
	s->index_array = ia;
	s->index_count = surface.index_count;
	s->index_buffer_size = surface.index_data.size();

	// material
	s->material = nullptr;
	s->uniform_set = nullptr;
}

size_t RendererStorage::mesh_get_surface_count(RID mesh)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };
	return m->surfaces.size();
}

RS::Primitive_ RendererStorage::mesh_surface_get_primitive(RID mesh, size_t index)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };
	ASSERT(index < m->surfaces.size());
	return m->surfaces[index]->primitive;
}

RID RendererStorage::mesh_surface_get_vertex_array(RID mesh, size_t index)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };
	ASSERT(index < m->surfaces.size());
	return m->surfaces[index]->vertex_array;
}

RID RendererStorage::mesh_surface_get_index_array(RID mesh, size_t index)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };
	ASSERT(index < m->surfaces.size());
	return m->surfaces[index]->index_array;
}

RID RendererStorage::mesh_surface_get_material(RID mesh, size_t index)
{
	Mesh * const m{ VALIDATE((Mesh *)mesh) };
	ASSERT(index < m->surfaces.size());
	return m->surfaces[index]->material;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::render_target_create()
{
	RenderTarget * const rt{ memnew(RenderTarget) };

	if (!rt->texture) { rt->texture = texture2d_placeholder_create(); }

	rt->color_format = RD::DataFormat_R8G8B8_UNORM;
	rt->color_format_srgb = RD::DataFormat_R8G8B8_UNORM;
	
	rt->color = RENDERING_DEVICE->texture_create(COMPOSE(RD::TextureCreateInfo, t) {
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
	RenderTarget * const rt{ VALIDATE((RenderTarget *)render_target) };
	if (rt->framebuffer) { RENDERING_DEVICE->framebuffer_destroy(rt->framebuffer); }
	if (rt->backbuffer) { RENDERING_DEVICE->framebuffer_destroy(rt->backbuffer); }
	memdelete(rt);
}

void RendererStorage::render_target_set_position(RID render_target, int32_t x, int32_t  y)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)render_target) };
}

void RendererStorage::render_target_set_size(RID render_target, int32_t width, int32_t height)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)render_target) };
	RENDERING_DEVICE->framebuffer_set_size(rt->framebuffer, width, height);
	RENDERING_DEVICE->framebuffer_set_size(rt->backbuffer, width, height);
}

RID RendererStorage::render_target_get_texture(RID render_target)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)render_target) };

	if (!rt->texture) { rt->texture = texture2d_placeholder_create(); }

	return rt->texture;
}

Vec2i RendererStorage::render_target_get_position(RID render_target)
{
	return { 0, 0 };
}

Vec2i RendererStorage::render_target_get_size(RID render_target)
{
	return VALIDATE((RenderTarget *)render_target)->size;
}

void RendererStorage::render_target_request_clear(RID render_target, Color const & value)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)render_target) };
	rt->clear_color = value;
	rt->clear_requested = true;
}

void RendererStorage::render_target_disable_clear_request(RID render_target)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)render_target) };
	rt->clear_requested = false;
}

bool RendererStorage::render_target_is_clear_requested(RID render_target)
{
	return VALIDATE((RenderTarget *)render_target)->clear_requested;
}

Color RendererStorage::render_target_get_clear_request_color(RID render_target)
{
	return VALIDATE((RenderTarget *)render_target)->clear_color;
}

void RendererStorage::render_target_do_clear_request(RID render_target)
{
	RenderTarget * const rt{ VALIDATE((RenderTarget *)render_target) };
	if (!rt->clear_requested) { return; }
	RENDERING_DEVICE->draw_list_begin(rt->framebuffer, rt->clear_color);
	RENDERING_DEVICE->draw_list_end();
	rt->clear_requested = false;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */