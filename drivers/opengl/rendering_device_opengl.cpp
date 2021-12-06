#if OPENGL_ENABLED

#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

// convert RID to opengl handle
#define GL_RID(rid) ((uint32_t &)(intptr_t &)(rid))

// map enum to array of constant values
#define MAKE_ENUM_MAPPING(m_from, m_to, ...)									\
	static constexpr m_to _MAP_##m_from##_TO_##m_to##_[] = { ##__VA_ARGS__ };	\
	NODISCARD static constexpr m_to MAP(m_from i) noexcept						\
	{																			\
		return _MAP_##m_from##_TO_##m_to##_[(size_t)i];							\
	}																			\

MAKE_ENUM_MAPPING(TextureType_, uint32_t,
	GL_TEXTURE_1D,
	GL_TEXTURE_2D,
	GL_TEXTURE_3D,
	GL_TEXTURE_CUBE_MAP,
	GL_TEXTURE_1D_ARRAY,
	GL_TEXTURE_2D_ARRAY,
	GL_TEXTURE_CUBE_MAP_ARRAY,
	GL_PROXY_TEXTURE_2D,
	GL_PROXY_TEXTURE_1D_ARRAY,
	GL_PROXY_TEXTURE_CUBE_MAP);

MAKE_ENUM_MAPPING(TextureSwizzle_, int32_t,
	0, // TextureSwizzle_Identity
	GL_ZERO,
	GL_ONE,
	GL_RED,
	GL_GREEN,
	GL_BLUE,
	GL_ALPHA);

MAKE_ENUM_MAPPING(SamplerFilter_, uint32_t,
	GL_NEAREST,
	GL_LINEAR);

MAKE_ENUM_MAPPING(SamplerRepeatMode_, uint32_t,
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRROR_CLAMP_TO_EDGE);

MAKE_ENUM_MAPPING(ShaderStage_, uint32_t,
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_COMPUTE_SHADER);

MAKE_ENUM_MAPPING(RenderPrimitive_, uint32_t,
	GL_POINTS,
	GL_LINES,
	GL_LINES_ADJACENCY,
	GL_LINE_STRIP,
	GL_LINE_STRIP_ADJACENCY,
	GL_TRIANGLES,
	GL_TRIANGLES_ADJACENCY,
	GL_TRIANGLE_STRIP,
	GL_TRIANGLE_STRIP_ADJACENCY,
	0, // RenderPrimitive_TriangleStripsWithRestartIndex
	GL_PATCHES);

MAKE_ENUM_MAPPING(PolygonCullMode_, uint32_t,
	GL_FRONT_AND_BACK,
	GL_FRONT,
	GL_BACK);

MAKE_ENUM_MAPPING(PolygonFrontFace_, uint32_t,
	GL_CW,
	GL_CCW);

MAKE_ENUM_MAPPING(StencilOperation_, uint32_t,
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_DECR,
	GL_INVERT,
	GL_INCR_WRAP,
	GL_DECR_WRAP);

MAKE_ENUM_MAPPING(BlendFactor_, uint32_t,
	GL_ZERO,
	GL_ONE,
	GL_SRC_COLOR,
	GL_ONE_MINUS_SRC_COLOR,
	GL_DST_COLOR,
	GL_ONE_MINUS_DST_COLOR,
	GL_SRC_ALPHA,
	GL_ONE_MINUS_SRC_ALPHA,
	GL_DST_ALPHA,
	GL_ONE_MINUS_DST_ALPHA,
	GL_CONSTANT_COLOR,
	GL_ONE_MINUS_CONSTANT_COLOR,
	GL_CONSTANT_ALPHA,
	GL_ONE_MINUS_CONSTANT_ALPHA,
	GL_SRC1_ALPHA,
	GL_ONE_MINUS_SRC1_ALPHA);

MAKE_ENUM_MAPPING(BlendOperation_, uint32_t,
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX);

MAKE_ENUM_MAPPING(CompareOperator_, uint32_t,
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS);

MAKE_ENUM_MAPPING(LogicOperation_, uint32_t,
	GL_CLEAR,
	GL_AND,
	GL_AND_REVERSE,
	GL_COPY,
	GL_AND_INVERTED,
	GL_NOOP,
	GL_XOR,
	GL_OR,
	GL_NOR,
	GL_EQUIV,
	GL_INVERT,
	GL_OR_REVERSE,
	GL_NAND,
	GL_SET);

MAKE_ENUM_MAPPING(ColorChannel_, uint32_t,
	GL_LUMINANCE,
	GL_LUMINANCE_ALPHA,
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

EMBED_CLASS(RenderingDeviceOpenGL, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{}
{
	ASSERT(OPENGL_INIT());
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
	glViewport(rect[0], rect[1], rect[2], rect[3]);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertexarray_create()
{
	RD_VertexArray & vertexarray{ m_data.push_back<RD_VertexArray>({}) };

	glGenVertexArrays(1, &GL_RID(vertexarray.handle));
	
	glBindVertexArray(GL_RID(vertexarray.handle));
	
	return vertexarray;
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
				ASSERT(e.type == DataType_F32 || e.type == DataType_Bool);

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
	return vertexbuffer;
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
	return indexbuffer;
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
	texture.data_format_srgb = view.format_override;
	texture.view = view;
	texture.width_2d = texture.width = format.width;
	texture.height_2d = texture.height = format.height;
	texture.depth = format.depth;
	texture.layers = format.layers;
	texture.mipmaps = format.mipmaps;

	uint32_t color_composition{}, pixel_data_format{}, pixel_data_type{};
	switch (texture.data_format)
	{
	case DataFormat_R8_UNORM: {
		texture.image_format = ImageFormat_R8;
		color_composition = GL_RGBA;
		pixel_data_format = GL_R;
		pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	case DataFormat_R8G8_UNORM: {
		texture.image_format = ImageFormat_RG8;
		color_composition = GL_RGBA;
		pixel_data_format = GL_RG;
		pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	case DataFormat_R8G8B8_UNORM: {
		texture.image_format = ImageFormat_RGB8;
		color_composition = GL_RGBA;
		pixel_data_format = GL_RGB;
		pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	case DataFormat_R8G8B8A8_UNORM: {
		texture.image_format = ImageFormat_RGBA8;
		color_composition = GL_RGBA;
		pixel_data_format = GL_RGBA;
		pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	}

	uint32_t const sampler_type{ MAP(texture.texture_type) };
	glGenTextures(1, &GL_RID(texture.handle));
	glBindTexture(sampler_type, GL_RID(texture.handle));
	
	if (texture.texture_type == TextureType_2D) {
		glTexImage2D(sampler_type, 0, color_composition, texture.width, texture.height, 0, pixel_data_format, pixel_data_type, data.data());
	}

	if (0 < texture.mipmaps) {
		glGenerateMipmap(sampler_type);
	}

	// TODO: I think this should go into a sampler object or something

	bool const repeated{};
	int32_t const wrap_mode{ repeated ? GL_REPEAT : GL_CLAMP_TO_EDGE };
	glTexParameteri(sampler_type, GL_TEXTURE_WRAP_S, wrap_mode);
	glTexParameteri(sampler_type, GL_TEXTURE_WRAP_T, wrap_mode);

	bool const smooth{};
	int32_t const filter_mode{
		(0 < texture.mipmaps)
		? (smooth ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR)
		: (smooth ? GL_LINEAR : GL_NEAREST) };
	glTexParameteri(sampler_type, GL_TEXTURE_MAG_FILTER, filter_mode);
	glTexParameteri(sampler_type, GL_TEXTURE_MIN_FILTER, filter_mode);

	int32_t const swizzle_mask[4]{
		(view.swizzle_r == TextureSwizzle_Identity) ? GL_RED : MAP(view.swizzle_r),
		(view.swizzle_g == TextureSwizzle_Identity) ? GL_GREEN : MAP(view.swizzle_g),
		(view.swizzle_b == TextureSwizzle_Identity) ? GL_BLUE : MAP(view.swizzle_b),
		(view.swizzle_a == TextureSwizzle_Identity) ? GL_ALPHA : MAP(view.swizzle_a) };
	glTexParameteriv(sampler_type, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask);

	return texture;
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
	uint32_t type{ MAP(texture.texture_type) };
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

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & texture_attachments)
{
	RD_FrameBuffer & framebuffer{ m_data.push_back<RD_FrameBuffer>({}) };
	framebuffer.textures = texture_attachments;

	glGenFramebuffers(1, &GL_RID(framebuffer.handle));

	for (size_t i = 0; i < texture_attachments.size(); ++i)
	{
		RD_Texture & color_attachment{ *VALIDATE((RD_Texture *)texture_attachments[i]) };
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + (uint32_t)i,
			GL_TEXTURE_2D,
			GL_RID(color_attachment.handle),
			0);
	}

	RD_Texture & depth_stencil{ m_data.push_back<RD_Texture>({}) };
	depth_stencil.texture_type = TextureType_2D;
	depth_stencil.data_format = {};
	depth_stencil.view = {};
	depth_stencil.width_2d = depth_stencil.width = framebuffer.size[0];
	depth_stencil.height_2d = depth_stencil.height = framebuffer.size[1];
	depth_stencil.mipmaps = 1;
	depth_stencil.usage_flags = TextureFlags_DepthStencilAttachment;
	glGenTextures(1, &GL_RID(depth_stencil.handle));
	glBindTexture(GL_TEXTURE_2D, GL_RID(depth_stencil.handle));
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_DEPTH24_STENCIL8,
		depth_stencil.width,
		depth_stencil.height,
		0,
		GL_DEPTH_STENCIL,
		GL_UNSIGNED_INT_24_8,
		nullptr);
	framebuffer.textures.push_back(depth_stencil);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D,
		GL_RID(depth_stencil.handle),
		0);

	glGenerateMipmap(GL_TEXTURE_2D);

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

	return framebuffer;
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
	ASSERT(!stage_data.empty());

	RD_Shader & shader{ m_data.push_back<RD_Shader>({}) };

	shader.handle = (RID)glCreateProgramObjectARB();

	for (ShaderStageData const & stage : stage_data)
	{
		uint32_t obj{ glCreateShaderObjectARB(MAP(stage.shader_stage)) };

		cstring source_code{ (cstring)stage.source_code.data() };
		
		glShaderSourceARB(obj, (uint32_t)stage.source_code.size(), &source_code, nullptr);
		
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

	return shader;
}

void RenderingDeviceOpenGL::shader_destroy(RID rid)
{
	RD_Shader & shader{ *VALIDATE((RD_Shader *)rid) };
	glDeleteObjectARB(GL_RID(shader.handle));
	m_data.erase<RD_Shader>(m_data.index_of<RD_Shader>(shader));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
