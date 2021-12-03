#if OPENGL_ENABLED

#include <drivers/opengl/rendering_device_opengl.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

EMBED_CLASS(RenderingDeviceOpenGL, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{}
{
	VERIFY(OPENGL_INIT());
}

RenderingDeviceOpenGL::~RenderingDeviceOpenGL()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingDeviceOpenGL::initialize()
{
}

void RenderingDeviceOpenGL::finalize()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingDeviceOpenGL::clear(Color const & color, bool depth_stencil)
{
	glClearColor(color[0], color[1], color[2], color[3]);

	uint32_t mask{ GL_COLOR_BUFFER_BIT };

	if LIKELY(depth_stencil) { mask |= (GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

	glClear(mask);
}

void RenderingDeviceOpenGL::set_viewport(IntRect const & rect)
{
	glViewport((int32_t)rect[0], (int32_t)rect[1], (int32_t)rect[2], (int32_t)rect[3]);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertexarray_create(RenderPrimitive_ primitive)
{
	RID rid{};
	glGenVertexArrays(1, &(uint32_t &)(intptr_t &)rid);
	glBindVertexArray((uint32_t)(intptr_t)rid);
	m_vertexarray.push_back(rid, primitive, VertexLayout{}, RID{}, Vector<RID>{});
	return rid;
}

void RenderingDeviceOpenGL::vertexarray_destroy(RID rid)
{
	if (auto i{ m_vertexarray.index_of<VA_RID>(rid) }; i != m_vertexarray.npos)
	{
		glDeleteVertexArrays(1, &(uint32_t &)(intptr_t &)rid);

		m_vertexarray.erase(i);
	}
}

void RenderingDeviceOpenGL::vertexarray_update(RID rid, VertexLayout const & layout, RID indices, Vector<RID> const & vertices)
{
	m_vertexarray.expand_all(m_vertexarray.index_of<VA_RID>(rid), [&
	](RID, RenderPrimitive_, VertexLayout & _layout, RID _indices, Vector<RID> & _vertices)
	{
		_layout = layout;
		_indices = indices;
		_vertices = vertices;

		glBindVertexArray((uint32_t)(intptr_t)rid);

		for (size_t i = 0, imax = _layout.get_elements().size(); i < imax; ++i)
		{
			VertexLayout::Element const & e{ _layout.get_elements()[i] };

			uint32_t const type{ std::invoke([&]() -> uint32_t {
				switch (e.get_base_type()) {
				case hash_v<bool>: return GL_BOOL;
				case hash_v<int32_t>: return GL_INT;
				case hash_v<float_t>: return GL_FLOAT;
				}
				return GL_NONE;
			}) };

			if (type == GL_INT)
			{
				glVertexAttribIPointer(
					(uint32_t)i,
					e.get_component_count(),
					type,
					_layout.get_stride(),
					(void const *)(intptr_t)e.offset);
			}
			else
			{
				glVertexAttribPointer(
					(uint32_t)i,
					e.get_component_count(),
					type,
					e.normalized,
					_layout.get_stride(),
					(void const *)(intptr_t)e.offset);
			}
		}
	});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertexbuffer_create(BufferUsage_ usage, Vector<byte> const & data)
{
	RID rid{};
	glGenBuffers(1, &(uint32_t &)(intptr_t &)rid);
	glBindBuffer(GL_ARRAY_BUFFER, (uint32_t)(intptr_t)rid);
	m_buffer.push_back(rid, usage, data);

	Vector<byte> & _data{ m_buffer.back<BUFFER_DATA>() };

	glBufferData(
		GL_ARRAY_BUFFER,
		(uint32_t)_data.size(),
		(void const *)_data.data(),
		std::invoke([usage]() {
			switch (usage) {
			case BufferUsage_Stream: return GL_STREAM_DRAW;
			case BufferUsage_Static: return GL_STATIC_DRAW;
			case BufferUsage_Dynamic: return GL_DYNAMIC_DRAW;
			}
			return 0;
		}));

	return rid;
}

void RenderingDeviceOpenGL::vertexbuffer_destroy(RID rid)
{
	if (auto i{ m_buffer.index_of<BUFFER_RID>(rid) }; i != m_buffer.npos)
	{
		glDeleteBuffers(1, &(uint32_t &)(intptr_t &)rid);

		m_buffer.erase(i);
	}
}

void RenderingDeviceOpenGL::vertexbuffer_update(RID rid, Vector<byte> const & data, size_t offset)
{
	Vector<byte> & _data{ m_buffer.map_unchecked<BUFFER_RID, BUFFER_DATA>(rid) };
	_data = data;
	glBindBuffer(GL_ARRAY_BUFFER, (uint32_t)(intptr_t)rid);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		(uint32_t)offset,
		(uint32_t)_data.size(),
		(void const *)_data.data());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::indexbuffer_create(BufferUsage_ usage, Vector<byte> const & data)
{
	RID rid{};
	glGenBuffers(1, &(uint32_t &)(intptr_t &)rid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)(intptr_t)rid);
	m_buffer.push_back(rid, usage, data);

	Vector<byte> & _data{ m_buffer.back<BUFFER_DATA>() };

	glBufferData(
		GL_ARRAY_BUFFER,
		(uint32_t)_data.size(),
		(void const *)_data.data(),
		std::invoke([usage]() {
		switch (usage) {
		case BufferUsage_Stream: return GL_STREAM_DRAW;
		case BufferUsage_Static: return GL_STATIC_DRAW;
		case BufferUsage_Dynamic: return GL_DYNAMIC_DRAW;
		}
		return 0;
	}));

	return rid;
}

void RenderingDeviceOpenGL::indexbuffer_destroy(RID rid)
{
	if (auto i{ m_buffer.index_of<BUFFER_RID>(rid) }; i != m_buffer.npos)
	{
		glDeleteBuffers(1, &(uint32_t &)(intptr_t &)rid);

		m_buffer.erase(i);
	}
}

void RenderingDeviceOpenGL::indexbuffer_update(RID rid, Vector<byte> const & data, size_t offset)
{
	Vector<byte> & _data{ m_buffer.map_unchecked<BUFFER_RID, BUFFER_DATA>(rid) };
	_data = data;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)(intptr_t)rid);
	glBufferSubData(
		GL_ARRAY_BUFFER,
		(uint32_t)offset,
		(uint32_t)_data.size(),
		(void const *)_data.data());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & attachments)
{
	RID rid{};
	return rid;
}

void RenderingDeviceOpenGL::framebuffer_destroy(RID rid)
{

}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(TextureFormat const & format, TextureView const & view, Vector<byte> const & data)
{
	RID rid{};
	return rid;
}

void RenderingDeviceOpenGL::texture_destroy(RID rid)
{
	if (auto i{ m_texture.index_of<TEX_RID>(rid) }; i != m_texture.npos)
	{
		m_texture.erase(i);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::shader_create(Vector<ShaderStageData> const & stages)
{
	RID rid{ (RID)glCreateProgramObjectARB() };

	m_shader.push_back(rid, String{});

	String & code{ m_shader.back<SHADER_CODE>() };

	code = "foobar";

	return rid;
}

void RenderingDeviceOpenGL::shader_destroy(RID rid)
{
	if (auto i{ m_shader.index_of<SHADER_RID>(rid) }; i != m_shader.npos)
	{
		glDeleteObjectARB((uint32_t)rid);

		m_shader.erase(i);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
