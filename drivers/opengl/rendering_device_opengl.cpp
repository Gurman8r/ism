#if OPENGL_ENABLED

#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define GL_RID(rid) ((uint32_t &)(intptr_t &)(rid))

constexpr uint32_t TEXTURE_TYPE[] = {
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_1D_ARRAY,
	GL_TEXTURE_2D_ARRAY,
	GL_TEXTURE_CUBE_MAP_ARRAY,
};

constexpr uint32_t SHADER_STAGE[] = {
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_COMPUTE_SHADER
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
	RD_VertexArray & vertexarray{ m_data.push_back<RD_VertexArray>({}) };

	glGenVertexArrays(1, &GL_RID(vertexarray.handle));
	
	glBindVertexArray(GL_RID(vertexarray.handle));
	
	return (RID)&vertexarray;
}

void RenderingDeviceOpenGL::vertexarray_destroy(RID rid)
{
	RD_VertexArray & vertexarray{ *VALIDATE((RD_VertexArray *)rid) };
	glDeleteVertexArrays(1, &GL_RID(vertexarray.handle));
	m_data.erase<RD_VertexArray>(m_data.index_of<RD_VertexArray>(vertexarray));
}

void RenderingDeviceOpenGL::vertexarray_update(RID rid, VertexLayout const & layout, RID indices, Vector<RID> const & vertices)
{
	RD_VertexArray & vertexarray{ *VALIDATE((RD_VertexArray *)rid) };
	glBindVertexArray(GL_RID(vertexarray.handle));
	
	RD_IndexBuffer & indexbuffer{ *VALIDATE((RD_IndexBuffer *)indices) };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer.handle));
	
	for (RID const vb : vertices)
	{
		RD_VertexBuffer & vertexbuffer{ *VALIDATE((RD_VertexBuffer *)vb) };
		glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer.handle));
	
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
	RD_VertexBuffer & vertexbuffer{ m_data.push_back<RD_VertexBuffer>({}) };
	glGenBuffers(1, &GL_RID(vertexbuffer.handle));
	glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer.handle));
	glBufferData(GL_ARRAY_BUFFER, (uint32_t)data.size(), data.data(), data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	return (RID)&vertexbuffer;
}

void RenderingDeviceOpenGL::vertexbuffer_destroy(RID rid)
{
	RD_VertexBuffer & vertexbuffer{ *VALIDATE((RD_VertexBuffer *)rid) };
	glDeleteBuffers(1, &GL_RID(vertexbuffer.handle));
	m_data.erase<RD_VertexBuffer>(m_data.index_of<RD_VertexBuffer>(vertexbuffer));
}

void RenderingDeviceOpenGL::vertexbuffer_update(RID rid, Vector<byte> const & data, size_t offset)
{
	RD_VertexBuffer & vertexbuffer{ *VALIDATE((RD_VertexBuffer *)rid) };
	glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer.handle));
	glBufferSubData(GL_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)data.size(), data.data());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::indexbuffer_create(Vector<byte> const & data)
{
	RD_IndexBuffer & indexbuffer{ m_data.push_back<RD_IndexBuffer>({}) };
	glGenBuffers(1, &GL_RID(indexbuffer.handle));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer.handle));
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)data.size(), data.data(), data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
	return (RID)&indexbuffer;
}

void RenderingDeviceOpenGL::indexbuffer_destroy(RID rid)
{
	RD_IndexBuffer & indexbuffer{ *VALIDATE((RD_IndexBuffer *)rid) };
	glDeleteBuffers(1, &GL_RID(indexbuffer.handle));
	m_data.erase<RD_IndexBuffer>(m_data.index_of<RD_IndexBuffer>(indexbuffer));
}

void RenderingDeviceOpenGL::indexbuffer_update(RID rid, Vector<byte> const & data, size_t offset)
{
	RD_IndexBuffer & indexbuffer{ *VALIDATE((RD_IndexBuffer *)rid) };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer.handle));
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)data.size(), data.data());
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(TextureFormat const & format, TextureView const & view, Vector<byte> const & data)
{
	RD_Texture & texture{ m_data.push_back<RD_Texture>({}) };
	texture.texture_type = format.texture_type;
	texture.data_format = format.format;
	texture.view = view;
	texture.width_2d = texture.width = format.width;
	texture.height_2d = texture.height = format.height;
	texture.mipmaps = format.mipmaps;

	glGenTextures(1, &GL_RID(texture.handle));
	if (texture.texture_type == TextureType_2D) {
		glBindTexture(GL_TEXTURE_2D, GL_RID(texture.handle));
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
	}

	return (RID)&texture;
}

void RenderingDeviceOpenGL::texture_destroy(RID rid)
{
	RD_Texture & texture{ *VALIDATE((RD_Texture *)rid) };
	glDeleteTextures(1, &GL_RID(texture.handle));
	m_data.erase<RD_Texture>(m_data.index_of<RD_Texture>(texture));
}

void RenderingDeviceOpenGL::texture_update(RID rid, Vector<byte> const & data)
{
	RD_Texture & texture{ *VALIDATE((RD_Texture *)rid) };
	uint32_t type{ TEXTURE_TYPE[texture.texture_type] };
	glBindTexture(type, GL_RID(texture.handle));
	glTexImage2D(type, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.data());
}

void RenderingDeviceOpenGL::texture_set_size_override(RID rid, int32_t width, int32_t height)
{
	RD_Texture & texture{ *VALIDATE((RD_Texture *)rid) };
	texture.width_2d = width;
	texture.height_2d = height;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & attachments)
{
	VERIFY(!attachments.empty());

	RD_FrameBuffer & framebuffer{ m_data.push_back<RD_FrameBuffer>({}) };

	glGenFramebuffers(1, &GL_RID(framebuffer.handle));

	for (size_t i = 0; i < attachments.size(); ++i)
	{
		RD_Texture & color_attachment{ *VALIDATE((RD_Texture *)attachments[i]) };
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + (uint32_t)i,
			GL_TEXTURE_2D,
			GL_RID(color_attachment.handle),
			0);
	}

	RD_Texture & depth_stencil_attachment{ *((RD_Texture *)texture_create({}, {}, {})) }; // <- FIXME
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D,
		GL_RID(depth_stencil_attachment.handle),
		0);

	VERIFY(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	return (RID)&framebuffer;
}

void RenderingDeviceOpenGL::framebuffer_destroy(RID rid)
{
	RD_FrameBuffer & framebuffer{ *VALIDATE((RD_FrameBuffer *)rid) };
	glDeleteFramebuffers(1, &GL_RID(framebuffer.handle));
	m_data.erase<RD_FrameBuffer>(m_data.index_of<RD_FrameBuffer>(framebuffer));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::shader_create(Vector<ShaderStageData> const & stage_data)
{
	VERIFY(!stage_data.empty());

	RD_Shader & shader{ m_data.push_back<RD_Shader>({}) };

	shader.handle = (RID)glCreateProgramObjectARB();

	for (ShaderStageData const & stage : stage_data)
	{
		uint32_t obj{ glCreateShaderObjectARB(SHADER_STAGE[stage.shader_stage]) };

		cstring code{ (cstring)stage.source_code.data() };
		
		glShaderSourceARB(obj, (uint32_t)stage.source_code.size(), &code, nullptr);
		
		glCompileShaderARB(obj);

		int32_t compile_success;
		glGetObjectParameterivARB(obj, GL_OBJECT_COMPILE_STATUS_ARB, &compile_success);
		if (!compile_success) { /* error */ }

		glAttachObjectARB(GL_RID(shader.handle), obj);
	}

	glLinkProgramARB(GL_RID(shader.handle));

	int32_t link_success;
	glGetObjectParameterivARB(GL_RID(shader.handle), GL_OBJECT_LINK_STATUS_ARB, &link_success);
	if (!link_success) { /* error */ }

	return (RID)&shader;
}

void RenderingDeviceOpenGL::shader_destroy(RID rid)
{
	RD_Shader & shader{ *VALIDATE((RD_Shader *)rid) };
	glDeleteObjectARB(GL_RID(shader.handle));
	m_data.erase<RD_Shader>(m_data.index_of<RD_Shader>(shader));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
