#ifndef _ISM_RENDERER_CANVAS_RENDERER_HPP_
#define _ISM_RENDERER_CANVAS_RENDERER_HPP_

#include <servers/rendering/renderer_storage.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// canvas renderer
	class ISM_API RendererCanvasRenderer : public Object
	{
		OBJECT_COMMON(RendererCanvasRenderer, Object);
		
		friend class RenderingServerDefault;

		RenderingDevice * const m_device;
		RendererStorage * const m_storage;

	public:
		using Index2D = typename uint32_t;

		struct Vertex2D
		{
			Vec2 position{};
			Vec2 texcoord{};
			Color color{};
		};

		struct Command2D
		{
			FloatRect rect{};
			RID texture{};
			uint32_t vertex_offset{};
			uint32_t index_offset{};
			uint32_t element_count{};
		};

	public:
		RendererCanvasRenderer(RenderingDevice * device, RendererStorage * storage);

		virtual ~RendererCanvasRenderer() override;

	public:
		void push_rect(FloatRect const & rect);
		void pop_rect();
		
		void push_texture(RID texture);
		void pop_texture();

		FloatRect const & get_rect() const { ASSERT(!m_rect_stack.empty()); return m_rect_stack.back(); }
		Vec2 const & get_rect_min() const { ASSERT(!m_rect_stack.empty()); return m_rect_stack.back().min(); }
		Vec2 const & get_rect_max() const { ASSERT(!m_rect_stack.empty()); return m_rect_stack.back().max(); }
		RID get_texture() const { ASSERT(!m_texture_stack.empty()); return m_texture_stack.back(); }

	public:
		void add_line(Vec2 const & p1, Vec2 const & p2, Color const & color, float_t thickness = 1.f);
		void add_rect(Vec2 const & min, Vec2 const & max, Color const & color, float_t thickness = 1.f);
		void add_rect_filled(Vec2 const & min, Vec2 const & max, Color const & color);
		void add_quad(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Color const & color, float_t thickness = 1.f);
		void add_quad_filled(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Color const & color);
		void add_triangle(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Color const & color, float_t thickness = 1.f);
		void add_triangle_filled(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Color const & color);
		void add_circle(Vec2 const & center, float_t radius, Color const & color, int32_t num_segments, float_t thickness = 1.f);
		void add_circle_filled(Vec2 const & center, float_t radius, Color const & color, int32_t num_segments);
		void add_image(RID texture, Vec2 const & min, Vec2 const & max, Vec2 const & uv0, Vec2 const & uv1, Color const & color = Colors::white);
		void add_image_quad(RID texture, Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Vec2 const & uv0, Vec2 const & uv1, Vec2 const & uv2, Vec2 const & uv3, Color const & color = Colors::white);

	public:
		void prim_reserve(uint32_t index_count, uint32_t vertex_count);
		void prim_unreserve(uint32_t index_count, uint32_t vertex_count);
		void prim_rect(Vec2 const & min, Vec2 const & max, Color const & color = Colors::white);
		void prim_rect_uv(Vec2 const & min, Vec2 const & max, Vec2 const & uv0, Vec2 const & uv1, Color const & color = Colors::white);
		void prim_quad_uv(Vec2 const & p1, Vec2 const & p2, Vec2 const & p3, Vec2 const & p4, Vec2 const & uv0, Vec2 const & uv1, Vec2 const & uv2, Vec2 const & uv3, Color const & color = Colors::white);

	private:
		Command2D m_cmd_head{};
		List<Command2D> m_cmd_buffer{};
		List<FloatRect> m_rect_stack{};
		List<RID> m_texture_stack{};

		List<Vertex2D> m_vertex_buffer{};
		List<Index2D> m_index_buffer{};
		Index2D m_vertex_current_index{};
		Vertex2D * m_vertex_write_ptr{};
		Index2D * m_index_write_ptr{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_RENDERER_CANVAS_RENDERER_HPP_
