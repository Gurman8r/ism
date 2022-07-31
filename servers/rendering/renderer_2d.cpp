#include <servers/rendering/renderer_2d.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(Renderer2D, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Renderer2D::Renderer2D(RenderingDevice * device, RendererStorage * storage) : m_device{ VALIDATE(device) }, m_storage{ VALIDATE(storage) }
	{
	}

	Renderer2D::~Renderer2D()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Renderer2D::push_rect(FloatRect const & rect)
	{
		m_rect_stack.push_back(rect);
		m_cmd_head.rect = rect;
	}

	void Renderer2D::pop_rect()
	{
		ASSERT(!m_rect_stack.empty());
		m_rect_stack.pop_back();
	}

	void Renderer2D::push_texture(RID texture)
	{
		m_texture_stack.push_back(texture);
		m_cmd_head.texture = texture;
	}

	void Renderer2D::pop_texture()
	{
		ASSERT(!m_texture_stack.empty());
		m_texture_stack.pop_back();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Renderer2D::add_line(Vec2 const & p1, Vec2 const & p2, Color const & color, f32 thickness)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_rect(Vec2 const & min, Vec2 const & max, Color const & color, f32 thickness)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_rect_filled(Vec2 const & min, Vec2 const & max, Color const & color)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_quad(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Color const & color, f32 thickness)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_quad_filled(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Color const & color)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_triangle(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Color const & color, f32 thickness)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_triangle_filled(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Color const & color)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_circle(Vec2 const & center, f32 radius, Color const & color, i32 num_segments, f32 thickness)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_circle_filled(Vec2 const & center, f32 radius, Color const & color, i32 num_segments)
	{
		if (color[3] == 0.f) { return; }
	}

	void Renderer2D::add_image(RID texture, Vec2 const & min, Vec2 const & max, Vec2 const & uv0, Vec2 const & uv1, Color const & color)
	{
		if (color[3] == 0.f) { return; }
		bool const push_texture_id{ texture != m_cmd_head.texture };
		if (push_texture_id) { push_texture(texture); }
		prim_reserve(6, 4);
		prim_rect_uv(min, max, uv0, uv1, color);
		if (push_texture_id) { pop_texture(); }
	}

	void Renderer2D::add_image_quad(RID texture, Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Vec2 const & uv0, Vec2 const & uv1, Vec2 const & uv2, Vec2 const & uv3, Color const & color)
	{
		if (color[3] == 0.f) { return; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Renderer2D::prim_reserve(u32 index_count, u32 vertex_count)
	{
		Command2D & cmd{ m_cmd_buffer.emplace_back(Command2D{}) };
		cmd.element_count += index_count;

		u32 const old_vertex_buffer_size{ (u32)m_vertex_buffer.size() };
		m_vertex_buffer.resize((size_t)(old_vertex_buffer_size + vertex_count));
		m_vertex_write_ptr = m_vertex_buffer.data() + old_vertex_buffer_size;

		u32 const old_index_buffer_size{ (u32)m_index_buffer.size() };
		m_index_buffer.resize((size_t)(old_index_buffer_size + index_count));
		m_index_write_ptr = m_index_buffer.data() + old_index_buffer_size;
	}

	void Renderer2D::prim_unreserve(u32 index_count, u32 vertex_count)
	{
		Command2D & cmd{ m_cmd_buffer.back() };
		cmd.element_count -= index_count;

		u32 const old_vertex_buffer_size{ (u32)m_vertex_buffer.size() };
		m_vertex_buffer.resize((size_t)(old_vertex_buffer_size - vertex_count));

		u32 const old_index_buffer_size{ (u32)m_index_buffer.size() };
		m_index_buffer.resize((size_t)(old_index_buffer_size - index_count));
	}

	void Renderer2D::prim_rect(Vec2 const & min, Vec2 const & max, Color const & color)
	{
		Vec2 const c{ max[0], min[1] }, d{ min[0], max[1] }, texcoord{ 0, 0 };
		Index2D const index{ (Index2D)m_vertex_current_index };
		m_index_write_ptr[0] = index; m_index_write_ptr[1] = (Index2D)(index + 1); m_index_write_ptr[2] = (Index2D)(index + 2);
		m_index_write_ptr[3] = index; m_index_write_ptr[4] = (Index2D)(index + 2); m_index_write_ptr[5] = (Index2D)(index + 3);
		m_vertex_write_ptr[0].position = min; m_vertex_write_ptr[0].texcoord = texcoord; m_vertex_write_ptr[0].color = color;
		m_vertex_write_ptr[1].position = c; m_vertex_write_ptr[1].texcoord = texcoord; m_vertex_write_ptr[1].color = color;
		m_vertex_write_ptr[2].position = max; m_vertex_write_ptr[2].texcoord = texcoord; m_vertex_write_ptr[2].color = color;
		m_vertex_write_ptr[3].position = d; m_vertex_write_ptr[3].texcoord = texcoord; m_vertex_write_ptr[3].color = color;
		m_vertex_write_ptr += 4;
		m_vertex_current_index += 4;
		m_index_write_ptr += 6;
	}

	void Renderer2D::prim_rect_uv(Vec2 const & min, Vec2 const & max, Vec2 const & uv0, Vec2 const & uv1, Color const & color)
	{
		Vec2 const c{ max[0], min[1] }, d{ min[0], max[1] }, uv2{ uv1[0], uv0[1] }, uv3{ uv0[0], uv1[1] };
		Index2D const index{ (Index2D)m_vertex_current_index };
		m_index_write_ptr[0] = index; m_index_write_ptr[1] = (Index2D)(index + 1); m_index_write_ptr[2] = (Index2D)(index + 2);
		m_index_write_ptr[3] = index; m_index_write_ptr[4] = (Index2D)(index + 2); m_index_write_ptr[5] = (Index2D)(index + 3);
		m_vertex_write_ptr[0].position = min; m_vertex_write_ptr[0].texcoord = uv0; m_vertex_write_ptr[0].color = color;
		m_vertex_write_ptr[1].position = c; m_vertex_write_ptr[1].texcoord = uv2; m_vertex_write_ptr[1].color = color;
		m_vertex_write_ptr[2].position = max; m_vertex_write_ptr[2].texcoord = uv1; m_vertex_write_ptr[2].color = color;
		m_vertex_write_ptr[3].position = d; m_vertex_write_ptr[3].texcoord = uv3; m_vertex_write_ptr[3].color = color;
		m_vertex_write_ptr += 4;
		m_vertex_current_index += 4;
		m_index_write_ptr += 6;
	}

	void Renderer2D::prim_quad_uv(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Vec2 const & uv0, Vec2 const & uv1, Vec2 const & uv2, Vec2 const & uv3, Color const & color)
	{
		Index2D const index{ (Index2D)m_vertex_current_index };
		m_index_write_ptr[0] = index; m_index_write_ptr[1] = (Index2D)(index + 1); m_index_write_ptr[2] = (Index2D)(index + 2);
		m_index_write_ptr[3] = index; m_index_write_ptr[4] = (Index2D)(index + 2); m_index_write_ptr[5] = (Index2D)(index + 3);
		m_vertex_write_ptr[0].position = p1; m_vertex_write_ptr[0].texcoord = uv0; m_vertex_write_ptr[0].color = color;
		m_vertex_write_ptr[1].position = p2; m_vertex_write_ptr[1].texcoord = uv1; m_vertex_write_ptr[1].color = color;
		m_vertex_write_ptr[2].position = p3; m_vertex_write_ptr[2].texcoord = uv2; m_vertex_write_ptr[2].color = color;
		m_vertex_write_ptr[3].position = p4; m_vertex_write_ptr[3].texcoord = uv3; m_vertex_write_ptr[3].color = color;
		m_vertex_write_ptr += 4;
		m_vertex_current_index += 4;
		m_index_write_ptr += 6;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}