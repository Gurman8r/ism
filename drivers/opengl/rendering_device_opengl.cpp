#if OPENGL_ENABLED

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef OPENGL_LOADER_CUSTOM
#if __has_include(OPENGL_LOADER_CUSTOM)
#	include OPENGL_LOADER_CUSTOM
#endif

#elif defined(OPENGL_LOADER_ES2)
#include <GLES2/gl2.h>
#define OPENGL_INIT()

#elif defined(OPENGL_LOADER_ES3)
#if defined(os_apple) && (TARGET_OS_IOS || TARGET_OS_TV)
#	include <OpenGLES/ES3/gl.h>
#else
#	include <GLES3/gl3.h>
#endif
#define OPENGL_INIT()

#elif defined(OPENGL_LOADER_GLEW)
#include <GL/glew.h>
#define OPENGL_INIT() do { \
	glewExperimental = true; \
	ASSERT(glewInit() == GLEW_OK); \
} while (0)

#elif defined(OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>
#define OPENGL_INIT() do { \
	ASSERT(gl3wInit()); \
} while(0)

#elif defined(OPENGL_LOADER_GLAD)
#include <glad/glad.h>
#define OPENGL_INIT() do { \
	ASSERT(gladLoadGL()); \
} while(0)

#else
#	error "Can't find OpenGL"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define glSetEnabled(m_cap, m_bool) ((m_bool) ? glEnable(m_cap) : glDisable(m_cap))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void glCheckError(cstring expr, cstring file, uint32_t line)
{
	uint32_t const code{ glGetError() };
	cstring name{}, desc{};
	switch (code)
	{
	default: return;
	case GL_INVALID_ENUM: {
		name = "GL_INVALID_ENUM";
		desc = "An unacceptable value has been specified for an enumerated argument.";
	} break;
	case GL_INVALID_VALUE: {
		name = "GL_INVALID_VALUE";
		desc = "A numeric argument is out of range.";
	} break;
	case GL_INVALID_OPERATION: {
		name = "GL_INVALID_OPERATION";
		desc = "The specified operation is not allowed in the current state.";
	} break;
	case GL_STACK_OVERFLOW: {
		name = "GL_STACK_OVERFLOW";
		desc = "This command would cause a stack overflow.";
	} break;
	case GL_STACK_UNDERFLOW: {
		name = "GL_STACK_UNDERFLOW";
		desc = "This command would cause a stack underflow.";
	} break;
	case GL_OUT_OF_MEMORY: {
		name = "GL_OUT_OF_MEMORY";
		desc = "There is not enough memory left to execute the command.";
	} break;
	case GL_INVALID_FRAMEBUFFER_OPERATION: {
		name = "GL_INVALID_FRAMEBUFFER_OPERATION";
		desc = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
	} break;
	}

	SYSTEM->printerr(
		"\nAn internal OpenGL call failed in \"%s\" (%u) \n"
		"Code: %u\n"
		"Expression: %s\n"
		"Description: %s\n"
		, file, line, code, expr, desc);
}

#if ISM_IS_DEBUG
#	define glCheck(expr) \
	do { expr; glCheckError(TOSTR(expr), __FILE__, __LINE__); } while (0)
#else
#	define glCheck(expr) expr
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void get_image_info(RD::DataFormat_ const data_format, Image::Format_ * image_format, uint32_t * internal_format, uint32_t * format, uint32_t * type) noexcept
{
	Image::Format_ _image_format{};
	uint32_t _internal_format{}, _format{}, _type{};
	switch (data_format)
	{
	case RD::DataFormat_R8_UNORM: {
		_image_format = Image::Format_R8;
		_internal_format = GL_RGBA;
		_format = GL_RED;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_R8G8_UNORM: {
		_image_format = Image::Format_RG8;
		_internal_format = GL_RGBA;
		_format = GL_RG;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_R8G8B8_UNORM: {
		_image_format = Image::Format_RGB8;
		_internal_format = GL_RGBA;
		_format = GL_RGB;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_R8G8B8A8_UNORM: {
		_image_format = Image::Format_RGBA8;
		_internal_format = GL_RGBA;
		_format = GL_RGBA;
		_type = GL_UNSIGNED_BYTE;
	} break;
	case RD::DataFormat_D24_UNORM_S8_UINT: {
		_image_format = Image::Format_L8;
		_internal_format = GL_DEPTH24_STENCIL8;
		_format = GL_DEPTH_STENCIL;
		_type = GL_UNSIGNED_INT_24_8;
	} break;
	}
	if (image_format) { *image_format = _image_format; }
	if (internal_format) { *internal_format = _internal_format; }
	if (format) { *format = _format; }
	if (type) { *type = _type; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MAKE_ENUM_MAPPING(TO_GL, CompareOperator_, uint32_t,
	GL_NEVER,
	GL_LESS,
	GL_EQUAL,
	GL_LEQUAL,
	GL_GREATER,
	GL_NOTEQUAL,
	GL_GEQUAL,
	GL_ALWAYS);

MAKE_ENUM_MAPPING(TO_GL, LogicOperation_, uint32_t,
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

ALIAS(BufferType_) RD::BufferType_;
MAKE_ENUM_MAPPING(TO_GL, BufferType_, uint32_t,
	GL_ARRAY_BUFFER,
	GL_ELEMENT_ARRAY_BUFFER,
	GL_UNIFORM_BUFFER);

ALIAS(IndexbufferFormat_) RD::IndexbufferFormat_;
MAKE_ENUM_MAPPING(TO_GL, IndexbufferFormat_, uint32_t,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT);

ALIAS(TextureType_) RD::TextureType_;
MAKE_ENUM_MAPPING(TO_GL, TextureType_, uint32_t,
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

ALIAS(TextureSwizzle_) RD::TextureSwizzle_;
MAKE_ENUM_MAPPING(TO_GL, TextureSwizzle_, int32_t,
	0, // TextureSwizzle_Identity
	GL_ZERO,
	GL_ONE,
	GL_RED,
	GL_GREEN,
	GL_BLUE,
	GL_ALPHA);

ALIAS(SamplerFilter_) RD::SamplerFilter_;
MAKE_ENUM_MAPPING(TO_GL, SamplerFilter_, uint32_t,
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR);

ALIAS(SamplerRepeatMode_) RD::SamplerRepeatMode_;
MAKE_ENUM_MAPPING(TO_GL, SamplerRepeatMode_, uint32_t,
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRROR_CLAMP_TO_EDGE);

ALIAS(ShaderStage_) RD::ShaderStage_;
MAKE_ENUM_MAPPING(TO_GL, ShaderStage_, uint32_t,
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_COMPUTE_SHADER);

ALIAS(RenderPrimitive_) RD::RenderPrimitive_;
MAKE_ENUM_MAPPING(TO_GL, RenderPrimitive_, uint32_t,
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

ALIAS(PolygonCullMode_) RD::PolygonCullMode_;
MAKE_ENUM_MAPPING(TO_GL, PolygonCullMode_, uint32_t,
	GL_FRONT_AND_BACK,
	GL_FRONT,
	GL_BACK);

ALIAS(PolygonFrontFace_) RD::PolygonFrontFace_;
MAKE_ENUM_MAPPING(TO_GL, PolygonFrontFace_, uint32_t,
	GL_CW,
	GL_CCW);

ALIAS(StencilOperation_) RD::StencilOperation_;
MAKE_ENUM_MAPPING(TO_GL, StencilOperation_, uint32_t,
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_DECR,
	GL_INVERT,
	GL_INCR_WRAP,
	GL_DECR_WRAP);

ALIAS(BlendFactor_) RD::BlendFactor_;
MAKE_ENUM_MAPPING(TO_GL, BlendFactor_, uint32_t,
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

ALIAS(BlendOperation_) RD::BlendOperation_;
MAKE_ENUM_MAPPING(TO_GL, BlendOperation_, uint32_t,
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(RenderingDeviceOpenGL, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RD{}
{
	OPENGL_INIT();

	glCheck(glEnable(GL_ALPHA_TEST));
	glCheck(glAlphaFunc(GL_GREATER, 0.0001f));
	glCheck(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
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

RID RenderingDeviceOpenGL::buffer_create(BufferType_ buffer_type, size_t size_in_bytes, DynamicBuffer const & data)
{
	ASSERT(0 < size_in_bytes);

	BufferBase * b;
	switch (buffer_type)
	{
	default: {
		CRASH("INVALID BUFFER TYPE");
	} break;
	case RD::BufferType_VertexBuffer: {
		b = memnew(VertexBuffer{});
		b->usage = data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	} break;
	case RD::BufferType_IndexBuffer: {
		b = memnew(IndexBuffer{});
		b->usage = data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;
	} break;
	case RD::BufferType_UniformBuffer: {
		b = memnew(UniformBuffer{});
		b->usage = GL_STATIC_DRAW;
	} break;
	}

	b->buffer_type = TO_GL(buffer_type);
	b->size = (uint32_t)size_in_bytes;
	b->data = data;

	glCheck(glGenBuffers(1, &b->handle));
	glCheck(glBindBuffer(b->buffer_type, b->handle));
	glCheck(glBufferData(b->buffer_type, b->size, b->data.data(), b->usage));
	glCheck(glBindBuffer(b->buffer_type, NULL));

	return (RID)b;
}

void RenderingDeviceOpenGL::buffer_destroy(RID buffer)
{
	BufferBase * const b{ VALIDATE((BufferBase *)buffer) };
	if (b->handle) { glCheck(glDeleteBuffers(1, &b->handle)); }
	switch (b->buffer_type) {
	default: { CRASH("INVALID BUFFER TYPE"); } break;
	case GL_ARRAY_BUFFER: { memdelete((VertexBuffer *)b); } break;
	case GL_ELEMENT_ARRAY_BUFFER: { memdelete((IndexBuffer *)b); } break;
	case GL_UNIFORM_BUFFER: { memdelete((UniformBuffer *)b); } break;
	}
}

void RenderingDeviceOpenGL::buffer_update(RID buffer, size_t offset, void const * data, size_t size_in_bytes)
{
	BufferBase * const b{ VALIDATE((BufferBase *)buffer) };
	b->data.write(offset, data, size_in_bytes);
	if (b->size < offset + size_in_bytes) { b->size = offset + size_in_bytes; }
	glCheck(glBindBuffer(b->buffer_type, b->handle));
	glCheck(glBufferSubData(b->buffer_type, (uint32_t)offset, b->size, b->data.data()));
	glCheck(glBindBuffer(b->buffer_type, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertex_buffer_create(size_t size_in_bytes, DynamicBuffer const & buffer)
{
	ASSERT(0 < size_in_bytes);
	VertexBuffer * const vb{ VALIDATE((VertexBuffer *)buffer_create(BufferType_VertexBuffer, size_in_bytes, buffer)) };
	return (RID)vb;
}

RID RenderingDeviceOpenGL::vertex_array_create(size_t vertex_count, VertexLayout const & layout, Vector<RID> const & buffers)
{
	ASSERT(0 < vertex_count);
	VertexArray * const va{ memnew(VertexArray{}) };
	va->vertex_count = vertex_count;
	va->layout = layout;
	va->buffers = buffers;

	glCheck(glGenVertexArrays(1, &va->handle));
	glCheck(glBindVertexArray(va->handle));

	for (RID const vertex_buffer : va->buffers)
	{
		VertexBuffer * const vb{ VALIDATE((VertexBuffer *)vertex_buffer) };

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb->handle));

		for (uint32_t i = 0, imax = (uint32_t)va->layout.attributes.size(); i < imax; ++i)
		{
			VertexLayoutAttribute const & fmt{ va->layout.attributes[i] };

			if (fmt.type == DataType_I32)
			{
				glCheck(glVertexAttribIPointer(i, fmt.count, GL_INT, va->layout.stride, (void *)(intptr_t)fmt.offset));
			}
			else
			{
				ASSERT(fmt.type == DataType_F32 || fmt.type == DataType_Bool);

				int32_t const fmt_type{ fmt.type == DataType_F32 ? GL_FLOAT : GL_BOOL };

				glCheck(glVertexAttribPointer(i, fmt.count, fmt_type, fmt.normalized, va->layout.stride, (void *)(intptr_t)fmt.offset));
			}

			glCheck(glEnableVertexAttribArray((uint32_t)i));
		}
	}

	return (RID)va;
}

void RenderingDeviceOpenGL::vertex_array_destroy(RID vertex_array)
{
	VertexArray * const va{ VALIDATE((VertexArray *)vertex_array) };
	for (RID const vertex_buffer : va->buffers) { if (vertex_buffer) { buffer_destroy(vertex_buffer); } }
	if (va->handle) { glCheck(glDeleteVertexArrays(1, &va->handle)); }
	memdelete(va);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::index_buffer_create(size_t index_count, IndexbufferFormat_ index_type, DynamicBuffer const & data)
{
	ASSERT(0 < index_count);
	size_t const size_in_bytes{ index_count * RD::get_index_buffer_format_size(index_type) };
	IndexBuffer * const ib{ VALIDATE((IndexBuffer *)buffer_create(BufferType_IndexBuffer, size_in_bytes, data)) };
	ib->index_count = (uint32_t)index_count;
	ib->index_type = TO_GL(index_type);
	return (RID)ib;
}

RID RenderingDeviceOpenGL::index_array_create(RID index_buffer, size_t index_offset, size_t index_count)
{
	IndexBuffer * const ib{ VALIDATE((IndexBuffer *)index_buffer) };
	IndexArray * const ia{ memnew(IndexArray{}) };
	ia->index_buffer = index_buffer;
	ia->index_offset = (uint32_t)index_offset;
	ia->index_count = (uint32_t)index_count;
	ia->index_type = ib->index_type;
	return (RID)ia;
}

void RenderingDeviceOpenGL::index_array_destroy(RID index_array)
{
	IndexArray * const ia{ VALIDATE((IndexArray *)index_array) };
	if (ia->index_buffer) { buffer_destroy(ia->index_buffer); }
	memdelete(ia);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(TextureCreateInfo const & spec, DynamicBuffer const & data)
{
	Texture * const t{ memnew(Texture{}) };
	t->texture_type = TO_GL(spec.texture_type);
	t->color_format = spec.color_format;
	t->color_format_srgb = spec.color_format_srgb;
	t->width_2d = t->width = spec.width;
	t->height_2d = t->height = spec.height;
	t->depth = spec.depth;
	t->layers = spec.layers;
	t->mipmaps = spec.mipmaps;
	t->samples = spec.samples;
	t->repeat_s = TO_GL(spec.repeat_s);
	t->repeat_t = TO_GL(spec.repeat_t);
	t->min_filter = TO_GL((SamplerFilter_)(spec.min_filter + (2 * (0 < spec.mipmaps))));
	t->mag_filter = TO_GL(spec.mag_filter);
	t->usage_flags = spec.usage_flags;
	t->swizzle_mask = {
		(spec.swizzle_r == TextureSwizzle_Identity) ? GL_RED : TO_GL(spec.swizzle_r),
		(spec.swizzle_g == TextureSwizzle_Identity) ? GL_GREEN : TO_GL(spec.swizzle_g),
		(spec.swizzle_b == TextureSwizzle_Identity) ? GL_BLUE : TO_GL(spec.swizzle_b),
		(spec.swizzle_a == TextureSwizzle_Identity) ? GL_ALPHA : TO_GL(spec.swizzle_a) };
	get_image_info(t->color_format, &t->image_format, 0, 0, 0);
	_texture_update((RID)t, data.data());
	return (RID)t;
}

void RenderingDeviceOpenGL::texture_destroy(RID texture)
{
	Texture * const t{ VALIDATE((Texture *)texture) };
	if (t->handle) { glCheck(glDeleteTextures(1, &t->handle)); }
	memdelete(t);
}

void RenderingDeviceOpenGL::texture_update(RID texture, DynamicBuffer const & data)
{
	Texture * const t{ VALIDATE((Texture *)texture) };
	if (t->handle) { glCheck(glDeleteTextures(1, &t->handle)); }
	_texture_update((RID)t, data.data());
}

void RenderingDeviceOpenGL::_texture_update(RID texture, void const * data)
{
	Texture * const t{ VALIDATE((Texture *)texture) };

	uint32_t _internal_format, _format, _type;
	get_image_info(t->color_format, nullptr, &_internal_format, &_format, &_type);

	glCheck(glGenTextures(1, &t->handle));
	glCheck(glBindTexture(t->texture_type, t->handle));

	switch (t->texture_type) {
	default: {
		CRASH("INVALID TEXTURE TYPE");
	} break;
	case GL_TEXTURE_2D: {
		glCheck(glTexImage2D(GL_TEXTURE_2D, 0, _internal_format, t->width, t->height, 0, _format, _type, data));
	} break;
	case GL_TEXTURE_3D: {
		// TODO: load texture3d...
	} break;
	case GL_TEXTURE_CUBE_MAP: {
		// TODO: load cubemap...
	} break;
	}

	if (0 < t->mipmaps) { glCheck(glGenerateMipmap(t->texture_type)); }
	glCheck(glTexParameteri(t->texture_type, GL_TEXTURE_WRAP_S, t->repeat_s));
	glCheck(glTexParameteri(t->texture_type, GL_TEXTURE_WRAP_T, t->repeat_t));
	glCheck(glTexParameteri(t->texture_type, GL_TEXTURE_MIN_FILTER, t->min_filter));
	glCheck(glTexParameteri(t->texture_type, GL_TEXTURE_MAG_FILTER, t->mag_filter));
	glCheck(glTexParameteriv(t->texture_type, GL_TEXTURE_SWIZZLE_RGBA, t->swizzle_mask));
}

void * RenderingDeviceOpenGL::texture_get_handle(RID texture)
{
	return (void *)(intptr_t)(VALIDATE((Texture *)texture)->handle);
}

DynamicBuffer RenderingDeviceOpenGL::texture_get_data(RID texture)
{
	Texture * const t{ VALIDATE((Texture *)texture) };

	uint32_t _internal_format, _format;
	get_image_info(t->color_format, nullptr, &_internal_format, nullptr, &_format);

	DynamicBuffer data{};
	data.resize((size_t)(t->width * t->height * t->depth));
	glCheck(glBindTexture(t->texture_type, t->handle));
	glCheck(glGetTexImage(t->texture_type, 0, _internal_format, _format, data));
	return data;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & texture_attachments)
{
	Framebuffer * fb{ memnew(Framebuffer{}) };
	fb->texture_attachments = texture_attachments;

	glCheck(glGenFramebuffers(1, &fb->handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));

	uint32_t color_attachment_index{};
	for (size_t i = 0; i < fb->texture_attachments.size(); ++i)
	{
		Texture * const t{ VALIDATE((Texture *)fb->texture_attachments[i]) };
		if (i == 0) { fb->width = t->width; }
		if (i == 0) { fb->height = t->height; }

		glCheck(glBindTexture(t->texture_type, t->handle));

		if (FLAG_READ(t->usage_flags, TextureFlags_ColorAttachment)) {
			ASSERT(!FLAG_READ(t->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), t->texture_type, t->handle, 0));
		}
		else {
			ASSERT(FLAG_READ(t->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, t->texture_type, t->handle, 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	return (RID)fb;
}

void RenderingDeviceOpenGL::framebuffer_destroy(RID framebuffer)
{
	Framebuffer * const fb{ VALIDATE((Framebuffer *)framebuffer) };
	for (RID texture : fb->texture_attachments) { texture_destroy(texture); }
	if (fb->handle) { glCheck(glDeleteFramebuffers(1, &fb->handle)); }
	memdelete(fb);
}

void RenderingDeviceOpenGL::framebuffer_set_size(RID framebuffer, int32_t width, int32_t height)
{
	Framebuffer * const fb{ VALIDATE((Framebuffer *)framebuffer) };
	if (fb->width == width && fb->height == height) { return; }
	fb->width = width;
	fb->height = height;

	if (fb->handle) { glCheck(glDeleteFramebuffers(1, &fb->handle)); }
	glCheck(glGenFramebuffers(1, &fb->handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));

	uint32_t color_attachment_index{};
	for (size_t i = 0; i < fb->texture_attachments.size(); ++i)
	{
		Texture * t{ VALIDATE((Texture *)fb->texture_attachments[i]) };
		t->width = fb->width;
		t->height = fb->height;

		texture_update((RID)t);

		if (FLAG_READ(t->usage_flags, TextureFlags_ColorAttachment)) {
			ASSERT(!FLAG_READ(t->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), t->texture_type, t->handle, 0));
		}
		else {
			ASSERT(FLAG_READ(t->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, t->texture_type, t->handle, 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::sampler_create(SamplerCreateInfo const & sampler_state)
{
	Sampler * ss{ memnew(Sampler{}) };
	ss->mag_filter = TO_GL(sampler_state.mag_filter);
	ss->min_filter = TO_GL(sampler_state.min_filter);
	ss->mip_filter = TO_GL(sampler_state.mip_filter);
	ss->repeat_s = TO_GL(sampler_state.repeat_s);
	ss->repeat_t = TO_GL(sampler_state.repeat_t);
	ss->repeat_r = TO_GL(sampler_state.repeat_r);
	ss->use_anisotropy = sampler_state.use_anisotropy;
	ss->anisotropy_max = sampler_state.anisotropy_max;
	ss->enable_compare = sampler_state.enable_compare;
	ss->compare_op = TO_GL(sampler_state.compare_op);
	ss->lod_bias = sampler_state.lod_bias;
	ss->min_lod = sampler_state.min_lod;
	ss->max_lod = sampler_state.max_lod;
	ss->border_color = sampler_state.border_color;
	ss->unnormalized_uvw = sampler_state.unnormalized_uvw;

	glCheck(glGenSamplers(1, &ss->handle));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_WRAP_S, ss->repeat_s));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_WRAP_T, ss->repeat_t));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_WRAP_R, ss->repeat_r));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_MAG_FILTER, ss->mag_filter));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_MIN_FILTER, ss->min_filter));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_COMPARE_MODE, ss->enable_compare ? GL_COMPARE_REF_TO_TEXTURE : GL_NONE));
	glCheck(glSamplerParameteri(ss->handle, GL_TEXTURE_COMPARE_FUNC, ss->compare_op));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_LOD_BIAS, ss->lod_bias));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_MAX_ANISOTROPY, ss->anisotropy_max));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_MIN_LOD, ss->min_lod));
	glCheck(glSamplerParameterf(ss->handle, GL_TEXTURE_MAX_LOD, ss->max_lod));

	switch (ss->border_color)
	{
	case SamplerBorderColor_Float_Transparent_Black: {
		constexpr Vec4f c{ 0.f, 0.f, 0.f, 0.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Int_Transparent_Black: {
		constexpr Vec4i c{ 0, 0, 0, 0 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Float_Opaque_Black: {
		constexpr Vec4f c{ 0.f, 0.f, 0.f, 0.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Int_Opaque_Black: {
		constexpr Vec4i c{ 0, 0, 0, 1 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Float_Opaque_White: {
		constexpr Vec4f c{ 1.f, 1.f, 1.f, 1.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	case SamplerBorderColor_Int_Opaque_White: {
		constexpr Vec4i c{ 1, 1, 1, 1 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, c));
	} break;
	}

	return (RID)ss;
}

void RenderingDeviceOpenGL::sampler_destroy(RID sampler)
{
	Sampler * const ss{ VALIDATE((Sampler *)sampler) };
	if (ss->handle) { glCheck(glDeleteSamplers(1, &ss->handle)); }
	memdelete(ss);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::shader_create(ShaderCreateInfo const & shader_info)
{
	Shader * s{ memnew(Shader{}) };
	glCheck(s->handle = glCreateProgramObjectARB());
	for (ShaderStageData const & stage : shader_info.stage_data) {
		if (!stage.code.empty()) {
			s->stage_data.push_back(stage);
		}
	}

	// compile stages
	for (ShaderStageData const & stage : s->stage_data)
	{
		cstring code{ VALIDATE(stage.code.c_str()) };

		uint32_t obj;
		glCheck(obj = glCreateShaderObjectARB(TO_GL(stage.shader_stage)));
		glCheck(glShaderSourceARB(obj, 1, &code, nullptr));
		glCheck(glCompileShaderARB(obj));

		int32_t compile_status;
		glCheck(glGetObjectParameterivARB(obj, GL_OBJECT_COMPILE_STATUS_ARB, &compile_status));
		if (!compile_status) {
			char log_str[1024]{};
			int32_t log_len{};
			glCheck(glGetInfoLogARB(obj, sizeof(log_str), &log_len, log_str));
			glCheck(glDeleteObjectARB(obj));
			glCheck(glDeleteProgramsARB(1, &s->handle));
			memdelete(s);
			SYSTEM->printerr(String{ log_str, (size_t)log_len });
			return nullptr;
		}

		glCheck(glAttachObjectARB(s->handle, obj));
		glCheck(glDeleteObjectARB(obj));
	}

	// link stages
	glCheck(glLinkProgramARB(s->handle));
	int32_t link_status;
	glCheck(glGetObjectParameterivARB(s->handle, GL_OBJECT_LINK_STATUS_ARB, &link_status));
	if (!link_status) {
		char log_str[1024]{};
		int32_t log_len{};
		glCheck(glGetInfoLogARB(s->handle, sizeof(log_str), &log_len, log_str));
		glCheck(glDeleteProgramsARB(1, &s->handle));
		memdelete(s);
		SYSTEM->printerr(String{ log_str, (size_t)log_len });
		return nullptr;
	}

	return (RID)s;
}

void RenderingDeviceOpenGL::shader_destroy(RID shader)
{
	Shader * s{ VALIDATE((Shader *)shader) };
	if (s->handle) { glCheck(glDeleteProgramsARB(1, &s->handle)); }
	memdelete(s);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::uniform_buffer_create(size_t size_in_bytes, DynamicBuffer const & data)
{
	ASSERT(0 < size_in_bytes);
	UniformBuffer * ub{ VALIDATE((UniformBuffer *)buffer_create(BufferType_UniformBuffer, size_in_bytes, data)) };
	return (RID)ub;
}

RID RenderingDeviceOpenGL::uniform_set_create(Vector<Uniform> const & uniforms, RID shader)
{
	UniformSet * set{ memnew(UniformSet{}) };
	set->shader = shader;
	for (size_t i = 0; i < uniforms.size(); ++i)
	{
		UniformDescriptor u{ uniforms[i].uniform_type };
		switch (u.uniform_type)
		{
		case UniformType_Sampler: {
		} break;
		case UniformType_SamplerWithTexture: {
		} break;
		case UniformType_Texture: {
			u.binding = uniforms[i].binding;
			u.length = uniforms[i].ids.size();
			for (RID const texture : uniforms[i].ids) {
				u.textures.push_back(texture);
			}
		} break;
		case UniformType_Image: {
		} break;
		case UniformType_TextureBuffer: {
		} break;
		case UniformType_SamplerWithTextureBuffer: {
		} break;
		case UniformType_ImageBuffer: {
		} break;
		case UniformType_UniformBuffer: {
			ASSERT(1 == uniforms[i].ids.size());
			u.binding = uniforms[i].binding;
			u.length = 1;
			u.buffers.push_back(uniforms[i].ids[0]);
		} break;
		case UniformType_StorageBuffer: {
		} break;
		case UniformType_InputAttachment: {
		} break;
		}
		set->uniforms.push_back(std::move(u));
	}
	return (RID)set;
}

void RenderingDeviceOpenGL::uniform_set_destroy(RID uniform_set)
{
	UniformSet * const set{ VALIDATE((UniformSet *)uniform_set) };
	memdelete(set);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::pipeline_create(RID shader, RenderPrimitive_ primitive, RasterizationState const & rasterization_state, MultisampleState const & multisample_state, DepthStencilState const & depth_stencil_state, ColorBlendState const & color_blend_state)
{
	RenderPipeline * rp{ memnew(RenderPipeline{}) };
	rp->shader = shader;
	rp->primitive = TO_GL(primitive);
	rp->rasterization_state = rasterization_state;
	rp->multisample_state = multisample_state;
	rp->depth_stencil_state = depth_stencil_state;
	rp->color_blend_state = color_blend_state;
	return (RID)rp;
}

void RenderingDeviceOpenGL::pipeline_destroy(RID pipeline)
{
	RenderPipeline * const rp{ VALIDATE((RenderPipeline *)pipeline) };
	memdelete(rp);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::draw_list_begin_for_screen(WindowID window, Color const & clear_color)
{
	ASSERT(window);

	DrawList * dl{ &m_lists.emplace_back(DrawList{}) };

	Vec2i const size{ DISPLAY_SERVER->window_get_size(window) };
	glCheck(glViewport(0, 0, size[0], size[1]));
	glCheck(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));
	glCheck(glClear(GL_COLOR_BUFFER_BIT));

	return (RID)dl;
}

RID RenderingDeviceOpenGL::draw_list_begin(RID framebuffer, Color const & clear_color, float_t clear_depth, int32_t clear_stencil)
{
	DrawList * dl{ &m_lists.emplace_back(DrawList{}) };

	Framebuffer * const fb{ VALIDATE((Framebuffer *)framebuffer) };
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));
	glCheck(glViewport(0, 0, fb->width, fb->height));
	glCheck(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));
	glCheck(glClearDepth(clear_depth));
	glCheck(glClearStencil(clear_stencil));
	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	return (RID)dl;
}

void RenderingDeviceOpenGL::draw_list_bind_pipeline(RID draw_list, RID pipeline)
{
	DrawList * const dl{ VALIDATE((DrawList *)draw_list) };
	if (dl->state.pipeline == pipeline) { return; }
	dl->state.pipeline = pipeline;
	
	RenderPipeline * const rp{ VALIDATE((RenderPipeline *)pipeline) };
	if (dl->state.pipeline_shader != rp->shader) {
		dl->state.pipeline_shader = rp->shader;
		// shader changed...
	}

	// rasterization state
	glCheck(glSetEnabled(GL_DEPTH_CLAMP, rp->rasterization_state.enable_depth_clamp));
	glCheck(glSetEnabled(GL_RASTERIZER_DISCARD, rp->rasterization_state.discard_primitives));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(TO_GL(rp->rasterization_state.cull_mode)));
	glCheck(glFrontFace(TO_GL(rp->rasterization_state.front_face)));
	glCheck(glPixelTransferf(GL_DEPTH_BIAS, rp->rasterization_state.depth_bias_constant_factor));
	glCheck(glLineWidth(rp->rasterization_state.line_width));

	// multisample state
	glCheck(glSetEnabled(GL_SAMPLE_SHADING, rp->multisample_state.enable_sample_shading));
	glCheck(glMinSampleShading(rp->multisample_state.min_sample_shading));
	for (size_t i = 0; i < rp->multisample_state.sample_mask.size(); ++i) {
		glCheck(glSampleMaski((uint32_t)i, rp->multisample_state.sample_mask[i]));
	}
	glCheck(glSetEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE, rp->multisample_state.enable_alpha_to_coverage));
	glCheck(glSetEnabled(GL_SAMPLE_ALPHA_TO_ONE, rp->multisample_state.enable_alpha_to_one));

	// depth stencil state
	glCheck(glSetEnabled(GL_DEPTH_TEST, rp->depth_stencil_state.enable_depth_test));
	glCheck(glDepthFunc(TO_GL(rp->depth_stencil_state.depth_compare_operator)));
	glCheck(glDepthRange(rp->depth_stencil_state.depth_range_min, rp->depth_stencil_state.depth_range_max));
	glCheck(glSetEnabled(GL_STENCIL_TEST, rp->depth_stencil_state.enable_stencil));
	glCheck(glStencilFuncSeparate(GL_FRONT, TO_GL(rp->depth_stencil_state.front_op.compare), rp->depth_stencil_state.front_op.reference, rp->depth_stencil_state.front_op.compare_mask));
	glCheck(glStencilMaskSeparate(GL_FRONT, rp->depth_stencil_state.front_op.write_mask));
	glCheck(glStencilFuncSeparate(GL_BACK, TO_GL(rp->depth_stencil_state.back_op.compare), rp->depth_stencil_state.back_op.reference, rp->depth_stencil_state.back_op.compare_mask));
	glCheck(glStencilMaskSeparate(GL_BACK, rp->depth_stencil_state.back_op.write_mask));

	// color blend state
	glCheck(glSetEnabled(GL_LOGIC_OP, rp->color_blend_state.enable_logic_op));
	glCheck(glLogicOp(TO_GL(rp->color_blend_state.logic_op)));
	for (auto & attachment : rp->color_blend_state.attachments)
	{
		glCheck(glSetEnabled(GL_BLEND, attachment.enable_blend));
		glCheck(glBlendColor(
			rp->color_blend_state.blend_constant[0],
			rp->color_blend_state.blend_constant[1],
			rp->color_blend_state.blend_constant[2],
			rp->color_blend_state.blend_constant[3]));
		glCheck(glBlendFuncSeparate(
			TO_GL(attachment.src_color_blend_factor),
			TO_GL(attachment.dst_color_blend_factor),
			TO_GL(attachment.src_alpha_blend_factor),
			TO_GL(attachment.dst_alpha_blend_factor)));
		glCheck(glBlendEquationSeparate(
			TO_GL(attachment.color_blend_op),
			TO_GL(attachment.alpha_blend_op)));
	}
}

void RenderingDeviceOpenGL::draw_list_bind_uniform_set(RID draw_list, RID uniform_set, size_t index)
{
	DrawList * const dl{ VALIDATE((DrawList *)draw_list) };
	UniformSet * const set{ VALIDATE((UniformSet *)uniform_set) };
	ASSERT(index < ARRAY_SIZE(dl->state.sets));
	if (dl->state.set_count <= index) { dl->state.set_count = (uint32_t)index + 1; }
	dl->state.sets[index].uniform_set = uniform_set;
	dl->state.sets[index].bound = false;
}

void RenderingDeviceOpenGL::draw_list_bind_vertex_array(RID draw_list, RID vertex_array)
{
	DrawList * const dl{ VALIDATE((DrawList *)draw_list) };
	if (dl->state.vertex_array == vertex_array) { return; }
	dl->state.vertex_array = vertex_array;
	VertexArray * const va{ VALIDATE((VertexArray *)dl->state.vertex_array) };
	glCheck(glBindVertexArray(va->handle));
}

void RenderingDeviceOpenGL::draw_list_bind_index_array(RID draw_list, RID index_array)
{
	DrawList * const dl{ VALIDATE((DrawList *)draw_list) };
	if (dl->state.index_array == index_array) { return; }
	dl->state.index_array = index_array;
	if (dl->state.index_array) {
		IndexArray * const ia{ VALIDATE((IndexArray *)dl->state.index_array) };
		IndexBuffer * const ib{ VALIDATE((IndexBuffer *)ia->index_buffer) };
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->handle));
	}
	else {
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
	}
}

void RenderingDeviceOpenGL::draw_list_draw(RID draw_list, bool use_indices, size_t instances, size_t procedural_vertices)
{
	DrawList * const dl{ VALIDATE((DrawList *)draw_list) };

	RenderPipeline * const rp{ VALIDATE((RenderPipeline *)dl->state.pipeline) };

	Shader * const ps{ VALIDATE((Shader *)dl->state.pipeline_shader) };

	glCheck(glUseProgramObjectARB(ps->handle));

	for (uint32_t i = 0; i < dl->state.set_count; ++i)
	{
		if (!dl->state.sets[i].bound)
		{
			UniformSet * const set{ VALIDATE((UniformSet *)dl->state.sets[i].uniform_set) };
			for (UniformDescriptor const & u : set->uniforms)
			{
				switch (u.uniform_type)
				{
				case UniformType_Sampler: {
				} break;
				case UniformType_SamplerWithTexture: {
				} break;
				case UniformType_Texture: {
					ASSERT(0 < u.textures.size());
					if (1 == u.textures.size()) {
						Texture * const t{ VALIDATE((Texture *)u.textures.front()) };
						glCheck(glBindTextureUnit(u.binding, t->handle));
						glCheck(glUniform1iARB(u.binding, u.binding));
					}
				} break;
				case UniformType_Image: {
				} break;
				case UniformType_TextureBuffer: {
				} break;
				case UniformType_SamplerWithTextureBuffer: {
				} break;
				case UniformType_ImageBuffer: {
				} break;
				case UniformType_UniformBuffer: {
					ASSERT(1 == u.buffers.size());
					UniformBuffer * const ub{ VALIDATE((UniformBuffer *)u.buffers[0]) };
					glCheck(glBindBufferRange(GL_UNIFORM_BUFFER, u.binding, ub->handle, 0, ub->size));
				} break;
				case UniformType_StorageBuffer: {
				} break;
				case UniformType_InputAttachment: {
				} break;
				}
			}
			dl->state.sets[i].bound = true;
		}
	}

	VertexArray * const va{ VALIDATE((VertexArray *)dl->state.vertex_array) };
	if (use_indices)
	{
		IndexArray * const ia{ VALIDATE((IndexArray *)dl->state.index_array) };
		for (RID const vb : va->buffers)
		{
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, VALIDATE((VertexBuffer *)vb)->handle));
			glCheck(glDrawElementsInstanced(rp->primitive, ia->index_count, ia->index_type, nullptr, (uint32_t)instances));
		}
	}
	else
	{
		for (RID const vb : va->buffers)
		{
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, VALIDATE((VertexBuffer *)vb)->handle));
			glCheck(glDrawArraysInstanced(rp->primitive, 0, va->vertex_count, (uint32_t)instances));
		}
	}
}

void RenderingDeviceOpenGL::draw_list_end()
{
	ASSERT(!m_lists.empty());
	SCOPE_EXIT(&) { m_lists.pop_back(); };

	glCheck(glBindVertexArray(NULL));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, NULL));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
	glCheck(glUseProgramObjectARB(NULL));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
