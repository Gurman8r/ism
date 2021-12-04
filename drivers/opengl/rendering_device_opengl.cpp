#if OPENGL_ENABLED

#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define GL_RID(rid) ((uint32_t &)(intptr_t &)rid)

constexpr uint32_t TEXTURE_TYPE[] = {
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_1D_ARRAY,
	GL_TEXTURE_2D_ARRAY,
	GL_TEXTURE_CUBE_MAP_ARRAY,
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

RID RenderingDeviceOpenGL::vertexarray_create()
{
	RID rid; glGenVertexArrays(1, &GL_RID(rid));
	
	glBindVertexArray(GL_RID(rid));
	
	return rid;
}

void RenderingDeviceOpenGL::vertexarray_destroy(RID rid)
{
	glDeleteVertexArrays(1, &GL_RID(rid));
}

void RenderingDeviceOpenGL::vertexarray_update(RID rid, VertexLayout const & layout, RID indices, Vector<RID> const & vertices)
{
	glBindVertexArray(GL_RID(rid));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indices));
	
	for (RID const vb : vertices)
	{
		glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vb));
	
		for (size_t i = 0, imax = layout.elements.size(); i < imax; ++i)
		{
			VertexLayout::Element const & e{ layout.elements[i] };

			if (e.type == DataType_I32)
			{
				glVertexAttribIPointer(
					(uint32_t)i,
					e.count,
					GL_INT,
					layout.stride,
					(void const *)(intptr_t)e.offset);
			}
			else
			{
				VERIFY(e.type == DataType_F32 || e.type == DataType_Bool);

				glVertexAttribPointer(
					(uint32_t)i,
					e.count,
					e.type == DataType_F32 ? GL_FLOAT : GL_BOOL,
					e.normalized,
					layout.stride,
					(void const *)(intptr_t)e.offset);
			}
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertexbuffer_create(Vector<byte> const & data)
{
	RID rid; glGenBuffers(1, &GL_RID(rid));

	glBindBuffer(GL_ARRAY_BUFFER, GL_RID(rid));

	glBufferData(
		GL_ARRAY_BUFFER,
		(uint32_t)data.size(),
		data.data(),
		data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	return rid;
}

void RenderingDeviceOpenGL::vertexbuffer_destroy(RID rid)
{
	glDeleteBuffers(1, &GL_RID(rid));
}

void RenderingDeviceOpenGL::vertexbuffer_update(RID rid, Vector<byte> const & data, size_t offset)
{
	glBindBuffer(GL_ARRAY_BUFFER, GL_RID(rid));

	glBufferSubData(
		GL_ARRAY_BUFFER,
		(uint32_t)offset,
		(uint32_t)data.size(),
		data.data());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::indexbuffer_create(Vector<byte> const & data)
{
	RID rid; glGenBuffers(1, &GL_RID(rid));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(rid));

	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		(uint32_t)data.size(),
		data.data(),
		data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);

	return rid;
}

void RenderingDeviceOpenGL::indexbuffer_destroy(RID rid)
{
	glDeleteBuffers(1, &GL_RID(rid));
}

void RenderingDeviceOpenGL::indexbuffer_update(RID rid, Vector<byte> const & data, size_t offset)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(rid));

	glBufferSubData(
		GL_ELEMENT_ARRAY_BUFFER,
		(uint32_t)offset,
		(uint32_t)data.size(),
		data.data());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & attachments)
{
	VERIFY(!attachments.empty());

	RID rid; glGenFramebuffers(1, &GL_RID(rid));

	for (size_t i = 0; i < attachments.size(); ++i)
	{
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + (uint32_t)i,
			GL_TEXTURE_2D,
			GL_RID(attachments[i]),
			0);
	}

	RID depth_stencil{ texture_create({}, {}, {}) };
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D,
		GL_RID(depth_stencil),
		0);

	VERIFY(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	return rid;
}

void RenderingDeviceOpenGL::framebuffer_destroy(RID rid)
{
	glDeleteFramebuffers(1, &GL_RID(rid));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(TextureFormat const & format, TextureView const & view, Vector<byte> const & data)
{
	RID rid; glGenTextures(1, &GL_RID(rid));
	
	if (format.texture_type == TextureType_2D)
	{
		glBindTexture(GL_TEXTURE_2D, GL_RID(rid));
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RGBA,
			format.width,
			format.height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data.data());
	}

	m_texture.push_back(rid, format, view);

	return rid;
}

void RenderingDeviceOpenGL::texture_update(RID rid, Vector<byte> const & data)
{
	size_t const it{ m_texture.index_of<TEX_RID>(rid) };
	if (it == m_texture.npos) { return; }
	m_texture.expand<TEX_FORMAT, TEX_VIEW>(it, [rid, &data](TextureFormat & format, TextureView & view)
	{
		uint32_t const texture_type{ TEXTURE_TYPE[format.texture_type] };
		glBindTexture(texture_type, GL_RID(rid));
		glTexImage2D(
			texture_type,
			0,
			GL_RGBA,
			format.width,
			format.height,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			data.data());
	});
}

void RenderingDeviceOpenGL::texture_destroy(RID rid)
{
	if (size_t const i{ m_texture.index_of<TEX_RID>(rid) }; i != m_texture.npos)
	{
		glDeleteTextures(1, &GL_RID(rid));

		m_texture.erase(i);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::shader_create(Vector<ShaderStageData> const & stages)
{
	RID rid{ (RID)glCreateProgramObjectARB() };

	return rid;
}

void RenderingDeviceOpenGL::shader_destroy(RID rid)
{
	glDeleteObjectARB(GL_RID(rid));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
