#if OPENGL_ENABLED

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef OPENGL_LOADER_CUSTOM
#	if __has_include(OPENGL_LOADER_CUSTOM)
#		include OPENGL_LOADER_CUSTOM
#	endif

#elif defined(OPENGL_LOADER_ES2)
#	include <GLES2/gl2.h>
#	define OPENGL_INIT() (false)

#elif defined(OPENGL_LOADER_ES3)
#	if defined(os_apple) && (TARGET_OS_IOS || TARGET_OS_TV)
#		include <OpenGLES/ES3/gl.h>
#	else
#		include <GLES3/gl3.h>
#	endif
#	define OPENGL_INIT() (false)

#elif defined(OPENGL_LOADER_GLEW)
#	include <GL/glew.h>
#	define OPENGL_INIT() ((glewExperimental = true) && (glewInit() == GLEW_OK))

#elif defined(OPENGL_LOADER_GL3W)
#	include <GL/gl3w.h>
#	define OPENGL_INIT() (gl3wInit())

#elif defined(OPENGL_LOADER_GLAD)
#	include <glad/glad.h>
#	define OPENGL_INIT() (gladLoadGL())

#else
#	error "Can't find OpenGL"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <drivers/opengl/rendering_device_opengl.hpp>

using namespace ism;

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

	OS->printerr(
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

void get_color_format_info(
	ColorFormat_ const format,
	ImageFormat_ * image_format,
	uint32_t * color_composition,
	uint32_t * pixel_data_format,
	uint32_t * pixel_data_type
) noexcept
{
	ImageFormat_ _image_format{};
	uint32_t _color_composition{}, _pixel_data_format{}, _pixel_data_type{};
	SCOPE_EXIT(&)
	{
		if (color_composition) { *color_composition = _color_composition; }
		if (image_format) { *image_format = _image_format; }
		if (pixel_data_format) { *pixel_data_format = _pixel_data_format; }
		if (pixel_data_type) { *pixel_data_type = _pixel_data_type; }
	};
	switch (format)
	{
	case ColorFormat_R8_UNORM: {
		_image_format = ImageFormat_R8;
		_color_composition = GL_RGBA;
		_pixel_data_format = GL_R;
		_pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	case ColorFormat_R8G8_UNORM: {
		_image_format = ImageFormat_RG8;
		_color_composition = GL_RGBA;
		_pixel_data_format = GL_RG;
		_pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	case ColorFormat_R8G8B8_UNORM: {
		_image_format = ImageFormat_RGB8;
		_color_composition = GL_RGBA;
		_pixel_data_format = GL_RGB;
		_pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	case ColorFormat_R8G8B8A8_UNORM: {
		_image_format = ImageFormat_RGBA8;
		_color_composition = GL_RGBA;
		_pixel_data_format = GL_RGBA;
		_pixel_data_type = GL_UNSIGNED_BYTE;
	} break;
	case ColorFormat_D24_UNORM_S8_UINT: {
		_image_format = ImageFormat_L8;
		_color_composition = GL_DEPTH24_STENCIL8;
		_pixel_data_format = GL_DEPTH_STENCIL;
		_pixel_data_type = GL_UNSIGNED_INT_24_8;
	} break;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

MAKE_ENUM_MAPPING(TO_GL, TextureSwizzle_, int32_t,
	0, // TextureSwizzle_Identity
	GL_ZERO,
	GL_ONE,
	GL_RED,
	GL_GREEN,
	GL_BLUE,
	GL_ALPHA);

MAKE_ENUM_MAPPING(TO_GL, SamplerFilter_, uint32_t,
	GL_NEAREST,
	GL_LINEAR,
	GL_NEAREST_MIPMAP_LINEAR,
	GL_LINEAR_MIPMAP_LINEAR);

MAKE_ENUM_MAPPING(TO_GL, SamplerRepeatMode_, uint32_t,
	GL_REPEAT,
	GL_MIRRORED_REPEAT,
	GL_CLAMP_TO_EDGE,
	GL_CLAMP_TO_BORDER,
	GL_MIRROR_CLAMP_TO_EDGE);

MAKE_ENUM_MAPPING(TO_GL, ShaderStage_, uint32_t,
	GL_VERTEX_SHADER,
	GL_FRAGMENT_SHADER,
	GL_GEOMETRY_SHADER,
	GL_TESS_CONTROL_SHADER,
	GL_TESS_EVALUATION_SHADER,
	GL_COMPUTE_SHADER);

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

MAKE_ENUM_MAPPING(TO_GL, PolygonCullMode_, uint32_t,
	GL_FRONT_AND_BACK,
	GL_FRONT,
	GL_BACK);

MAKE_ENUM_MAPPING(TO_GL, PolygonFrontFace_, uint32_t,
	GL_CW,
	GL_CCW);

MAKE_ENUM_MAPPING(TO_GL, StencilOperation_, uint32_t,
	GL_KEEP,
	GL_ZERO,
	GL_REPLACE,
	GL_INCR,
	GL_DECR,
	GL_INVERT,
	GL_INCR_WRAP,
	GL_DECR_WRAP);

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

MAKE_ENUM_MAPPING(TO_GL, BlendOperation_, uint32_t,
	GL_FUNC_ADD,
	GL_FUNC_SUBTRACT,
	GL_FUNC_REVERSE_SUBTRACT,
	GL_MIN,
	GL_MAX);

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

MAKE_ENUM_MAPPING(TO_GL, ColorChannel_, uint32_t,
	GL_LUMINANCE,
	GL_LUMINANCE_ALPHA,
	GL_RED,
	GL_RG,
	GL_RGB,
	GL_RGBA);

MAKE_ENUM_MAPPING(TO_GL, IndexbufferFormat_, uint32_t,
	GL_UNSIGNED_BYTE,
	GL_UNSIGNED_SHORT,
	GL_UNSIGNED_INT);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(RenderingDeviceOpenGL, t)
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

RID RenderingDeviceOpenGL::vertexbuffer_create(uint32_t size_bytes, Buffer const & data)
{
	ASSERT(0 < size_bytes);
	_VertexBuffer * vb{ memnew(_VertexBuffer{}) };
	vb->data = data;
	glCheck(glGenBuffers(1, &vb->handle));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb->handle));
	if (!data.empty()) {
		glCheck(glBufferData(GL_ARRAY_BUFFER, (uint32_t)vb->data.size(), vb->data.data(), GL_STATIC_DRAW));
	}
	else {
		glCheck(glBufferData(GL_ARRAY_BUFFER, size_bytes, nullptr, GL_DYNAMIC_DRAW));
	}
	return (RID)vb;
}

void RenderingDeviceOpenGL::vertexbuffer_destroy(RID vertexbuffer)
{
	_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
	glCheck(glDeleteBuffers(1, &vb->handle));
	memdelete(vb);
}

void RenderingDeviceOpenGL::vertexbuffer_set_data(RID vertexbuffer, Buffer const & data, size_t offset)
{
	_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
	vb->data = data;
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb->handle));
	glCheck(glBufferSubData(GL_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)vb->data.size(), vb->data.data()));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertexarray_create(uint32_t vertex_count, VertexFormat const & format, Vector<RID> const & buffers)
{
	ASSERT(0 < vertex_count);
	_VertexArray * va{ memnew(_VertexArray{}) };
	va->vertex_count = vertex_count;
	va->format = format;
	va->buffers = buffers;

	glCheck(glGenVertexArrays(1, &va->handle));
	glCheck(glBindVertexArray(va->handle));
	
	for (RID const vertexbuffer : buffers)
	{
		_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb->handle));

		for (size_t i = 0, imax = va->format.elements.size(); i < imax; ++i)
		{
			VertexFormat::Element const & fmt{ va->format.elements[i] };

			ASSERT(fmt.type == DataType_I32
				|| fmt.type == DataType_F32
				|| fmt.type == DataType_Bool);

			if (fmt.type == DataType_I32)
			{
				glCheck(glVertexAttribIPointer(
					(uint32_t)i,
					fmt.count,
					GL_INT,
					va->format.stride,
					(void const *)(intptr_t)fmt.offset));
			}
			else
			{
				glCheck(glVertexAttribPointer(
					(uint32_t)i,
					fmt.count,
					fmt.type == DataType_F32 ? GL_FLOAT : GL_BOOL,
					fmt.normalized,
					va->format.stride,
					(void const *)(intptr_t)fmt.offset));
			}

			glCheck(glEnableVertexAttribArray((uint32_t)i));
		}
	}

	return (RID)va;
}

void RenderingDeviceOpenGL::vertexarray_destroy(RID vertexarray)
{
	_VertexArray * const va{ VALIDATE((_VertexArray *)vertexarray) };
	for (RID const vertexbuffer : va->buffers) { vertexbuffer_destroy(vertexbuffer); }
	glCheck(glDeleteVertexArrays(1, &va->handle));
	memdelete(va);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::indexbuffer_create(uint32_t index_count, IndexbufferFormat_ index_type, Buffer const & data)
{
	ASSERT(0 < index_count);

	_IndexBuffer * const ib{ memnew(_IndexBuffer{}) };
	ib->index_count = index_count;
	ib->index_type = TO_GL(index_type);
	ib->data = data;

	glCheck(glGenBuffers(1, &ib->handle));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->handle));

	if (!ib->data.empty()) {
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)ib->data.size(), ib->data.data(), GL_STATIC_DRAW));
	}
	else {
		glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count, nullptr, GL_DYNAMIC_DRAW));
	}

	return (RID)ib;
}

void RenderingDeviceOpenGL::indexbuffer_destroy(RID indexbuffer)
{
	_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)indexbuffer) };
	glCheck(glDeleteBuffers(1, &ib->handle));
	memdelete(ib);
}

void RenderingDeviceOpenGL::indexbuffer_set_data(RID indexbuffer, Buffer const & data, size_t offset)
{
	_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)indexbuffer) };
	ib->data = data;
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->handle));
	glCheck(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)ib->data.size(), ib->data.data()));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(RD::TextureFormat const & format, Buffer const & data)
{
	_Texture * tex{ memnew(_Texture{}) };
	tex->texture_type = TO_GL(format.texture_type);
	tex->color_format = format.color_format;
	tex->color_format_srgb = format.color_format_srgb;
	tex->width_2d = tex->width = format.width;
	tex->height_2d = tex->height = format.height;
	tex->depth = format.depth;
	tex->layers = format.layers;
	tex->mipmaps = format.mipmaps;
	tex->samples = format.samples;
	tex->repeat_s = TO_GL(format.repeat_s);
	tex->repeat_t = TO_GL(format.repeat_t);
	tex->min_filter = TO_GL((SamplerFilter_)(format.min_filter + (2 * (0 < format.mipmaps))));
	tex->mag_filter = TO_GL(format.mag_filter);
	tex->usage_flags = format.usage_flags;
	tex->swizzle_mask = {
		(format.swizzle_r == TextureSwizzle_Identity) ? GL_RED : TO_GL(format.swizzle_r),
		(format.swizzle_g == TextureSwizzle_Identity) ? GL_GREEN : TO_GL(format.swizzle_g),
		(format.swizzle_b == TextureSwizzle_Identity) ? GL_BLUE : TO_GL(format.swizzle_b),
		(format.swizzle_a == TextureSwizzle_Identity) ? GL_ALPHA : TO_GL(format.swizzle_a) };

	get_color_format_info(tex->color_format, &tex->image_format, 0, 0, 0);

	_texture_update((RID)tex, data.data());

	return (RID)tex;
}

void RenderingDeviceOpenGL::texture_destroy(RID texture)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };
	glCheck(glDeleteTextures(1, &tex->handle));
	memdelete(tex);
}

void RenderingDeviceOpenGL::texture_bind(RID texture, size_t slot)
{
	glCheck(glBindTextureUnit((uint32_t)slot, texture ? ((_Texture *)texture)->handle : NULL));
}

void RenderingDeviceOpenGL::texture_update(RID texture, Buffer const & data)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };
	if (tex->handle) { glCheck(glDeleteTextures(1, &tex->handle)); }
	_texture_update((RID)tex, data.data());
}

Buffer RenderingDeviceOpenGL::texture_get_data(RID texture)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };

	uint32_t color_composition{}, pixel_data_type{};
	get_color_format_info(tex->color_format, 0, &color_composition, 0, &pixel_data_type);

	size_t channel_count{};
	switch (tex->image_format)
	{
	case ImageFormat_R8: { channel_count = 1; } break;
	case ImageFormat_RG8: { channel_count = 2; } break;
	case ImageFormat_RGB8: { channel_count = 3; } break;
	case ImageFormat_RGBA8: { channel_count = 4; } break;
	}

	Buffer data;
	data.resize((size_t)(tex->width * tex->height * channel_count));
	glCheck(glBindTexture(tex->texture_type, tex->handle));
	glCheck(glGetTexImage(tex->texture_type, 0, color_composition, pixel_data_type, data.data()));
	return data;
}

void * RenderingDeviceOpenGL::texture_get_handle(RID texture)
{
	return (void *)(intptr_t)(VALIDATE((_Texture *)texture)->handle);
}

void RenderingDeviceOpenGL::_texture_update(RID texture, void const * data)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };

	uint32_t color_composition{}, pixel_data_format{}, pixel_data_type{};
	get_color_format_info(tex->color_format, 0, &color_composition, &pixel_data_format, &pixel_data_type);

	glCheck(glGenTextures(1, &tex->handle));
	glCheck(glBindTexture(tex->texture_type, tex->handle));

	if (tex->texture_type == GL_TEXTURE_2D) {
		glCheck(glTexImage2D(tex->texture_type, 0, color_composition, tex->width, tex->height, 0, pixel_data_format, pixel_data_type, data));
	}

	if (0 < tex->mipmaps) {
		glCheck(glGenerateMipmap(tex->texture_type));
	}
	
	glCheck(glTexParameteri(tex->texture_type, GL_TEXTURE_WRAP_S, tex->repeat_s));
	glCheck(glTexParameteri(tex->texture_type, GL_TEXTURE_WRAP_T, tex->repeat_t));
	glCheck(glTexParameteri(tex->texture_type, GL_TEXTURE_MIN_FILTER, tex->min_filter));
	glCheck(glTexParameteri(tex->texture_type, GL_TEXTURE_MAG_FILTER, tex->mag_filter));
	glCheck(glTexParameteriv(tex->texture_type, GL_TEXTURE_SWIZZLE_RGBA, tex->swizzle_mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & texture_attachments)
{
	_Framebuffer * fb{ memnew(_Framebuffer{}) };
	fb->texture_attachments = texture_attachments;

	glCheck(glGenFramebuffers(1, &fb->handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));

	uint32_t color_attachment_index{};
	for (size_t i = 0; i < fb->texture_attachments.size(); ++i)
	{
		_Texture * tex{ VALIDATE((_Texture *)fb->texture_attachments[i]) };
		if (i == 0) { fb->width = tex->width; }
		if (i == 0) { fb->height = tex->height; }

		if (is_depth_stencil_color_format(tex->color_format)) {
			FLAG_SET(tex->usage_flags, TextureFlags_DepthStencilAttachment);
		}
		else {
			FLAG_SET(tex->usage_flags, TextureFlags_ColorAttachment);
		}

		texture_bind((RID)tex);

		if (FLAG_READ(tex->usage_flags, TextureFlags_ColorAttachment))
		{
			ASSERT(!FLAG_READ(tex->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), tex->texture_type, tex->handle, 0));
		}
		else
		{
			ASSERT(FLAG_READ(tex->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, tex->texture_type, tex->handle, 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	return (RID)fb;
}

void RenderingDeviceOpenGL::framebuffer_destroy(RID framebuffer)
{
	_Framebuffer * const fb{ VALIDATE((_Framebuffer *)framebuffer) };
	for (RID texture : fb->texture_attachments) { texture_destroy(texture); }
	glCheck(glDeleteFramebuffers(1, &fb->handle));
	memdelete(fb);
}

void RenderingDeviceOpenGL::framebuffer_bind(RID framebuffer)
{
	if (framebuffer) {
		_Framebuffer * const fb{ (_Framebuffer *)framebuffer };
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));
		glCheck(glViewport(0, 0, fb->width, fb->height));
	}
	else {
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	}
}

void RenderingDeviceOpenGL::framebuffer_set_size(RID framebuffer, int32_t width, int32_t height)
{
	_Framebuffer * const fb{ VALIDATE((_Framebuffer *)framebuffer) };
	if (fb->width == width && fb->height == height) { return; }
	fb->width = width;
	fb->height = height;

	if (fb->handle) { glCheck(glDeleteFramebuffers(1, &fb->handle)); }
	glCheck(glGenFramebuffers(1, &fb->handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));

	uint32_t color_attachment_index{};
	for (size_t i = 0; i < fb->texture_attachments.size(); ++i)
	{
		_Texture * tex{ VALIDATE((_Texture *)fb->texture_attachments[i]) };
		tex->width = fb->width;
		tex->height = fb->height;

		texture_update((RID)tex);

		if (FLAG_READ(tex->usage_flags, TextureFlags_ColorAttachment))
		{
			ASSERT(!FLAG_READ(tex->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), tex->texture_type, tex->handle, 0));
		}
		else
		{
			ASSERT(FLAG_READ(tex->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, tex->texture_type, tex->handle, 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::sampler_create(RD::SamplerState const & sampler_state)
{
	_SamplerState * ss{ memnew(_SamplerState{}) };
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
		constexpr Vec4f color{ 0.f, 0.f, 0.f, 0.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, color));
	} break;
	case SamplerBorderColor_Int_Transparent_Black: {
		constexpr Vec4i color{ 0, 0, 0, 0 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, color));
	} break;
	case SamplerBorderColor_Float_Opaque_Black: {
		constexpr Vec4f color{ 0.f, 0.f, 0.f, 0.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, color));
	} break;
	case SamplerBorderColor_Int_Opaque_Black: {
		constexpr Vec4i color{ 0, 0, 0, 1 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, color));
	} break;
	case SamplerBorderColor_Float_Opaque_White: {
		constexpr Vec4f color{ 1.f, 1.f, 1.f, 1.f };
		glCheck(glSamplerParameterfv(ss->handle, GL_TEXTURE_BORDER_COLOR, color));
	} break;
	case SamplerBorderColor_Int_Opaque_White: {
		constexpr Vec4i color{ 1, 1, 1, 1 };
		glCheck(glSamplerParameteriv(ss->handle, GL_TEXTURE_BORDER_COLOR, color));
	} break;
	}

	return (RID)ss;
}

void RenderingDeviceOpenGL::sampler_destroy(RID sampler)
{
	_SamplerState * const ss{ VALIDATE((_SamplerState *)sampler) };
	glCheck(glDeleteSamplers(1, &ss->handle));
	memdelete(ss);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static uint32_t _compile_shader(ShaderStage_ shader_stage, cstring source_code)
{
	ASSERT(source_code && *source_code);
	uint32_t obj;
	glCheck(obj = glCreateShaderObjectARB(TO_GL(shader_stage)));
	glCheck(glShaderSourceARB(obj, 1, &source_code, nullptr));
	glCheck(glCompileShaderARB(obj));
	return obj;
}

RID RenderingDeviceOpenGL::shader_create(Vector<RD::ShaderStageData> const & stage_data)
{
	static char log_str[1024]{};
	ASSERT(!stage_data.empty());

	_Shader * s{ memnew(_Shader{}) };
	
	glCheck(s->handle = glCreateProgramObjectARB());

	for (ShaderStageData const & stage : stage_data) {
		if (!stage.source.empty()) {
			s->stage_data.push_back(stage);
		}
	}

	for (ShaderStageData & stage : s->stage_data)
	{
		uint32_t obj{ _compile_shader(stage.shader_stage, stage.source.c_str()) };

		int32_t compile_status;
		glCheck(glGetObjectParameterivARB(obj, GL_OBJECT_COMPILE_STATUS_ARB, &compile_status));
		if (!compile_status) {
			glCheck(glGetInfoLogARB(obj, sizeof(log_str), 0, nullptr));
			print_error(log_str);
			glCheck(glDeleteObjectARB(obj));
			shader_destroy((RID)s);
			return RID{};
		}

		glCheck(glAttachObjectARB(s->handle, obj));
		glCheck(glDeleteObjectARB(obj));
	}

	glCheck(glLinkProgramARB(s->handle));

	int32_t link_status;
	glCheck(glGetObjectParameterivARB(s->handle, GL_OBJECT_LINK_STATUS_ARB, &link_status));
	if (!link_status) {
		glCheck(glGetInfoLogARB(s->handle, sizeof(log_str), 0, nullptr));
		print_error(log_str);
		shader_destroy((RID)s);
		return RID{};
	}

	return (RID)s;
}

void RenderingDeviceOpenGL::shader_destroy(RID shader)
{
	_Shader * s{ VALIDATE((_Shader *)shader) };
	glCheck(glDeleteProgramsARB(1, &s->handle));
	memdelete(s);
}

void RenderingDeviceOpenGL::shader_bind(RID shader)
{
	glCheck(glUseProgramObjectARB(shader ? ((_Shader *)shader)->handle : NULL));
}

struct RenderingDeviceOpenGL::UniformBinder final
{
	int32_t location{ -1 };

	uint32_t self{}, last{};

	operator bool() const noexcept { return -1 < location; }

	UniformBinder(_Shader & s, cstring name)
	{
		if (!name || !*name || !(self = s.handle)) { return; }

		glCheck(last = glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

		if (last != self) { glCheck(glUseProgramObjectARB(self)); }

		hash_t const id{ hash(name, std::strlen(name)) };
		
		location = s.bindings.find_or_add_fn(id, [&]()
		{
			int32_t result;
			glCheck(result = glGetUniformLocationARB(self, name));
			return result;
		});
	}

	~UniformBinder()
	{
		if (self && (self != last)) { glCheck(glUseProgramObjectARB(last)); }
	}
};

int32_t RenderingDeviceOpenGL::shader_uniform_location(RID shader, cstring name)
{
	return UniformBinder{ *VALIDATE((_Shader *)shader), name }.location;
}

void RenderingDeviceOpenGL::shader_uniform1i(RID shader, cstring name, int32_t const value)
{
	if (UniformBinder u{ *VALIDATE((_Shader *)shader), name })
	{
		glCheck(glUniform1iARB(u.location, value));
	}
}

void RenderingDeviceOpenGL::shader_uniform1f(RID shader, cstring name, float_t const value)
{
	if (UniformBinder u{ *VALIDATE((_Shader *)shader), name })
	{
		glCheck(glUniform1fARB(u.location, value));
	}
}

void RenderingDeviceOpenGL::shader_uniform2f(RID shader, cstring name, float_t const * value)
{
	if (UniformBinder u{ *VALIDATE((_Shader *)shader), name })
	{
		glCheck(glUniform2fARB(u.location, value[0], value[1]));
	}
}

void RenderingDeviceOpenGL::shader_uniform3f(RID shader, cstring name, float_t const * value)
{
	if (UniformBinder u{ *VALIDATE((_Shader *)shader), name })
	{
		glCheck(glUniform3fARB(u.location, value[0], value[1], value[2]));
	}
}

void RenderingDeviceOpenGL::shader_uniform4f(RID shader, cstring name, float_t const * value)
{
	if (UniformBinder u{ *VALIDATE((_Shader *)shader), name })
	{
		glCheck(glUniform4fARB(u.location, value[0], value[1], value[2], value[3]));
	}
}

void RenderingDeviceOpenGL::shader_uniform16f(RID shader, cstring name, float_t const * value, bool transpose)
{
	if (UniformBinder u{ *VALIDATE((_Shader *)shader), name })
	{
		glCheck(glUniformMatrix4fvARB(u.location, 1, transpose, value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::uniform_buffer_create(Buffer const & data)
{
	return RID();
}

RID RenderingDeviceOpenGL::uniform_set_create(Vector<Uniform> const & uniforms, RID shader)
{
	return RID();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::pipeline_create(RID shader, RenderPrimitive_ primitive, RD::RasterizationState const & rasterization_state, RD::MultisampleState const & multisample_state, RD::DepthStencilState const & depthstencil_state, RD::ColorBlendState const & colorblend_state)
{
	_Pipeline * rp{ memnew(_Pipeline) };
	rp->shader = shader;
	rp->primitive = TO_GL(primitive);
	rp->rasterization_state = rasterization_state;
	rp->multisample_state = multisample_state;
	rp->depthstencil_state = depthstencil_state;
	rp->colorblend_state = colorblend_state;

	glCheck(glEnable(GL_ALPHA_TEST));
	glCheck(glAlphaFunc(GL_GREATER, 0.001f));

	// rasterization state
	glCheck(glSetEnabled(GL_DEPTH_CLAMP, rasterization_state.enable_depth_clamp));
	glCheck(glSetEnabled(GL_RASTERIZER_DISCARD, rasterization_state.discard_primitives));
	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(TO_GL(rasterization_state.cull_mode)));
	glCheck(glFrontFace(TO_GL(rasterization_state.front_face)));
	glCheck(glPixelTransferf(GL_DEPTH_BIAS, rasterization_state.depth_bias_constant_factor));
	glCheck(glLineWidth(rasterization_state.line_width));

	// multisample state
	glCheck(glSetEnabled(GL_SAMPLE_SHADING, multisample_state.enable_sample_shading));
	glCheck(glMinSampleShading(multisample_state.min_sample_shading));
	for (size_t i = 0; i < multisample_state.sample_mask.size(); ++i) {
		glCheck(glSampleMaski((uint32_t)i, multisample_state.sample_mask[i]));
	}
	glCheck(glSetEnabled(GL_SAMPLE_ALPHA_TO_COVERAGE, multisample_state.enable_alpha_to_coverage));
	glCheck(glSetEnabled(GL_SAMPLE_ALPHA_TO_ONE, multisample_state.enable_alpha_to_one));

	// depth stencil state
	glCheck(glSetEnabled(GL_DEPTH_TEST, depthstencil_state.enable_depth_test));
	glCheck(glDepthFunc(TO_GL(depthstencil_state.depth_compare_operator)));
	glCheck(glDepthRange(depthstencil_state.depth_range_min, depthstencil_state.depth_range_max));
	glCheck(glSetEnabled(GL_STENCIL_TEST, depthstencil_state.enable_stencil));
	glCheck(glStencilFuncSeparate(GL_FRONT, TO_GL(depthstencil_state.front_op.compare), depthstencil_state.front_op.reference, depthstencil_state.front_op.compare_mask));
	glCheck(glStencilMaskSeparate(GL_FRONT, depthstencil_state.front_op.write_mask));
	glCheck(glStencilFuncSeparate(GL_BACK, TO_GL(depthstencil_state.back_op.compare), depthstencil_state.back_op.reference, depthstencil_state.back_op.compare_mask));
	glCheck(glStencilMaskSeparate(GL_BACK, depthstencil_state.back_op.write_mask));

	// color blend state
	glCheck(glSetEnabled(GL_LOGIC_OP, colorblend_state.enable_logic_op));
	glCheck(glLogicOp(TO_GL(colorblend_state.logic_op)));
	for (auto & attachment : colorblend_state.attachments)
	{
		glCheck(glSetEnabled(GL_BLEND, attachment.enable_blend));
		glCheck(glBlendColor(
			colorblend_state.blend_constant[0],
			colorblend_state.blend_constant[1],
			colorblend_state.blend_constant[2],
			colorblend_state.blend_constant[3]));
		glCheck(glBlendFuncSeparate(
			TO_GL(attachment.src_color_blend_factor),
			TO_GL(attachment.dst_color_blend_factor),
			TO_GL(attachment.src_alpha_blend_factor),
			TO_GL(attachment.dst_alpha_blend_factor)));
		glCheck(glBlendEquationSeparate(
			TO_GL(attachment.color_blend_op),
			TO_GL(attachment.alpha_blend_op)));
	}
	
	return (RID)rp;
}

void RenderingDeviceOpenGL::pipeline_destroy(RID pipeline)
{
	_Pipeline * const rp{ VALIDATE((_Pipeline *)pipeline) };
	memdelete(rp);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::drawlist_begin_for_screen(WindowID window, Color const & clear_color)
{
	ASSERT(window);
	ASSERT(!m_drawlist);
	m_drawlist = memnew(_DrawList{});

	Vec2i const size{ DISPLAY_SERVER->window_get_size(window) };
	glCheck(glViewport(0, 0, size[0], size[1]));
	glCheck(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));
	glCheck(glClear(GL_COLOR_BUFFER_BIT));

	return (RID)m_drawlist;
}

RID RenderingDeviceOpenGL::drawlist_begin(RID framebuffer, Color const & clear_color, float_t clear_depth, int32_t clear_stencil)
{
	ASSERT(!m_drawlist);
	m_drawlist = memnew(_DrawList{});

	_Framebuffer * const fb{ VALIDATE((_Framebuffer *)framebuffer) };
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, fb->handle));
	glCheck(glViewport(0, 0, fb->width, fb->height));
	glCheck(glClearColor(clear_color[0], clear_color[1], clear_color[2], clear_color[3]));
	glCheck(glClearDepth(clear_depth));
	glCheck(glClearStencil(clear_stencil));
	glCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));

	return (RID)m_drawlist;
}

void RenderingDeviceOpenGL::drawlist_bind_pipeline(RID list, RID pipeline)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
	if (dl->state.pipeline == pipeline) { return; }
	dl->state.pipeline = pipeline;
	_Pipeline * const rp{ VALIDATE((_Pipeline *)pipeline) };
	dl->state.pipeline_shader = rp->shader;
}

void RenderingDeviceOpenGL::drawlist_bind_uniforms(RID list, RID uniforms)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
}

void RenderingDeviceOpenGL::drawlist_bind_vertices(RID list, RID vertexarray)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
	if (dl->state.vertexarray == vertexarray) { return; }
	dl->state.vertexarray = vertexarray;
	_VertexArray * const va{ VALIDATE((_VertexArray *)vertexarray) };
	glCheck(glBindVertexArray(va->handle));
}

void RenderingDeviceOpenGL::drawlist_bind_indices(RID list, RID indexbuffer)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
	if (dl->state.indexbuffer == indexbuffer) { return; }
	dl->state.indexbuffer = indexbuffer;
	_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)indexbuffer) };
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->handle));
}

void RenderingDeviceOpenGL::drawlist_draw(RID list, bool use_indices, uint32_t instances, uint32_t procedural_vertices)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
	_Pipeline * const rp{ VALIDATE((_Pipeline *)dl->state.pipeline) };
	_Shader * const ps{ VALIDATE((_Shader *)dl->state.pipeline_shader) };
	
	glCheck(glUseProgramObjectARB(ps->handle));
	
	// TODO: bind uniforms here

	_VertexArray * const va{ VALIDATE((_VertexArray *)dl->state.vertexarray) };
	if (use_indices)
	{
		_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)dl->state.indexbuffer) };
		for (RID const vertexbuffer : va->buffers) {
			_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb->handle));
			glCheck(glDrawElements(rp->primitive, ib->index_count, ib->index_type, nullptr));
		}
	}
	else
	{
		for (RID const vertexbuffer : va->buffers) {
			_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, vb->handle));
			glCheck(glDrawArrays(rp->primitive, 0, va->vertex_count));
		}
	}
}

void RenderingDeviceOpenGL::drawlist_end()
{
	glCheck(glBindVertexArray(0));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	
	if (m_drawlist) { memdelete(m_drawlist); m_drawlist = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
