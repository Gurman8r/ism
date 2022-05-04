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

// convert RID to opengl handle
#define GL_RID(rid) ((uint32_t &)(intptr_t &)(rid))

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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(RenderingDeviceOpenGL, t)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingDeviceOpenGL::RenderingDeviceOpenGL() : RenderingDevice{}
{
	ASSERT(OPENGL_INIT());

	glCheck(glEnable(GL_ALPHA_TEST));
	glCheck(glAlphaFunc(GL_GREATER, 0.001f));

	glCheck(glEnable(GL_BLEND));
	glCheck(glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	glCheck(glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD));

	glCheck(glEnable(GL_CULL_FACE));
	glCheck(glCullFace(GL_BACK));
	glCheck(glFrontFace(GL_CCW));

	glCheck(glEnable(GL_DEPTH_TEST));
	glCheck(glDepthFunc(GL_LESS));
	glCheck(glDepthRangef(0.f, 1.f));

	glCheck(glEnable(GL_STENCIL_TEST));
	glCheck(glStencilFuncSeparate(GL_FRONT, GL_ALWAYS, 0, 0xFFFFFFFF));
	glCheck(glStencilFuncSeparate(GL_BACK, GL_ALWAYS, 0, 0xFFFFFFFF));
	
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

RID RenderingDeviceOpenGL::vertexarray_create(VertexFormat const & layout, Vector<RID> const & buffers)
{
	_VertexArray * va{ memnew(_VertexArray{}) };
	glCheck(glGenVertexArrays(1, &GL_RID(va->handle)));
	glCheck(glBindVertexArray(GL_RID(va->handle)));
	va->buffers = buffers;
	va->layout = layout;

	for (RID const vertices : buffers)
	{
		_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertices) };

		glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vb->handle)));

		for (size_t i = 0, imax = va->layout.elements.size(); i < imax; ++i)
		{
			VertexFormat::Element const & fmt{ va->layout.elements[i] };

			ASSERT(fmt.type == DataType_I32 || fmt.type == DataType_F32 || fmt.type == DataType_Bool);

			if (fmt.type == DataType_I32)
			{
				glCheck(glVertexAttribIPointer(
					(uint32_t)i,
					fmt.count,
					GL_INT,
					va->layout.stride,
					(void const *)(intptr_t)fmt.offset));
			}
			else
			{
				glCheck(glVertexAttribPointer(
					(uint32_t)i,
					fmt.count,
					fmt.type == DataType_F32 ? GL_FLOAT : GL_BOOL,
					fmt.normalized,
					va->layout.stride,
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
	glCheck(glDeleteVertexArrays(1, &GL_RID(va->handle)));
	memdelete(va);
}

void RenderingDeviceOpenGL::vertexarray_bind(RID vertexarray)
{
	glCheck(glBindVertexArray(vertexarray ? GL_RID(((_VertexArray *)vertexarray)->handle) : NULL));
}

RID * RenderingDeviceOpenGL::vertexarray_get_buffers(RID vertexarray)
{
	return VALIDATE((_VertexArray *)vertexarray)->buffers.data();
}

size_t RenderingDeviceOpenGL::vertexarray_get_buffer_count(RID vertexarray)
{
	return VALIDATE((_VertexArray *)vertexarray)->buffers.size();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertexbuffer_create(Buffer const & data)
{
	_VertexBuffer * vb{ memnew(_VertexBuffer{}) };
	vb->data = data;
	glCheck(glGenBuffers(1, &GL_RID(vb->handle)));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vb->handle)));
	glCheck(glBufferData(GL_ARRAY_BUFFER, (uint32_t)vb->data.size(), vb->data.data(), vb->data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
	return (RID)vb;
}

void RenderingDeviceOpenGL::vertexbuffer_destroy(RID vertexbuffer)
{
	_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
	glCheck(glDeleteBuffers(1, &GL_RID(vb->handle)));
	memdelete(vb);
}

void RenderingDeviceOpenGL::vertexbuffer_bind(RID vertexbuffer)
{
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer ? GL_RID(VALIDATE((_VertexBuffer *)vertexbuffer)->handle) : NULL));
}

void RenderingDeviceOpenGL::vertexbuffer_update(RID vertexbuffer, Buffer const & data, size_t offset)
{
	_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
	vb->data = data;
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vb->handle)));
	glCheck(glBufferSubData(GL_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)vb->data.size(), vb->data.data()));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::indexbuffer_create(Buffer const & data)
{
	_IndexBuffer * const ib{ memnew(_IndexBuffer{}) };
	ib->data = data;
	glCheck(glGenBuffers(1, &GL_RID(ib->handle)));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(ib->handle)));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)ib->data.size(), ib->data.data(), ib->data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
	return (RID)ib;
}

void RenderingDeviceOpenGL::indexbuffer_destroy(RID indexbuffer)
{
	_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)indexbuffer) };
	glCheck(glDeleteBuffers(1, &GL_RID(ib->handle)));
	memdelete(ib);
}

void RenderingDeviceOpenGL::indexbuffer_bind(RID indexbuffer)
{
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer ? GL_RID(VALIDATE((_IndexBuffer *)indexbuffer)->handle) : NULL));
}

void RenderingDeviceOpenGL::indexbuffer_update(RID indexbuffer, Buffer const & data, size_t offset)
{
	_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)indexbuffer) };
	ib->data = data;
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(ib->handle)));
	glCheck(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)ib->data.size(), ib->data.data()));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(RD::TextureFormat const & format, Buffer const & data)
{
	_Texture * tex{ memnew(_Texture{}) };
	tex->texture_type = format.texture_type;
	tex->color_format = format.color_format;
	tex->width_2d = tex->width = format.width;
	tex->height_2d = tex->height = format.height;
	tex->depth = format.depth;
	tex->layers = format.layers;
	tex->mipmaps = format.mipmaps;
	tex->samples = format.samples;
	tex->repeat_s = format.repeat_s;
	tex->repeat_t = format.repeat_t;
	tex->min_filter = format.min_filter;
	tex->mag_filter = format.mag_filter;
	tex->usage_flags = format.usage_flags;
	tex->color_format_srgb = format.color_format_srgb;
	tex->swizzle_r = format.swizzle_r;
	tex->swizzle_g = format.swizzle_g;
	tex->swizzle_b = format.swizzle_b;
	tex->swizzle_a = format.swizzle_a;

	get_color_format_info(tex->color_format, &tex->image_format, 0, 0, 0);

	_texture_update((RID)tex, data.data());

	return (RID)tex;
}

void RenderingDeviceOpenGL::texture_destroy(RID texture)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };
	glCheck(glDeleteTextures(1, &GL_RID(tex->handle)));
	memdelete(tex);
}

void RenderingDeviceOpenGL::texture_bind(RID texture, size_t slot)
{
	glCheck(glBindTextureUnit((uint32_t)slot, texture ? GL_RID(((_Texture *)texture)->handle) : NULL));
}

void RenderingDeviceOpenGL::texture_update(RID texture, Buffer const & data)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };
	if (tex->handle) { glCheck(glDeleteTextures(1, &GL_RID(tex->handle))); }
	_texture_update((RID)tex, data.data());
}

Buffer RenderingDeviceOpenGL::texture_get_data(RID texture)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };

	uint32_t const sampler_type{ TO_GL(tex->texture_type) };

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
	glCheck(glBindTexture(sampler_type, GL_RID(tex->handle)));
	glCheck(glGetTexImage(sampler_type, 0, color_composition, pixel_data_type, data.data()));
	return data;
}

RID RenderingDeviceOpenGL::texture_get_handle(RID texture)
{
	return VALIDATE((_Texture *)texture)->handle;
}

void RenderingDeviceOpenGL::_texture_update(RID texture, void const * data)
{
	_Texture * const tex{ VALIDATE((_Texture *)texture) };

	uint32_t color_composition{}, pixel_data_format{}, pixel_data_type{};
	get_color_format_info(tex->color_format, 0, &color_composition, &pixel_data_format, &pixel_data_type);

	uint32_t & handle{ GL_RID(tex->handle) };
	uint32_t const sampler_type{ TO_GL(tex->texture_type) };
	glCheck(glGenTextures(1, &handle));
	glCheck(glBindTexture(sampler_type, handle));

	if (tex->texture_type == TextureType_2D) {
		glCheck(glTexImage2D(sampler_type, 0, color_composition, tex->width, tex->height, 0, pixel_data_format, pixel_data_type, data));
	}

	if (0 < tex->mipmaps) {
		glCheck(glGenerateMipmap(sampler_type));
	}
	
	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_WRAP_S, TO_GL(tex->repeat_s)));

	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_WRAP_T, TO_GL(tex->repeat_t)));
	
	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_MIN_FILTER, TO_GL((SamplerFilter_)(tex->min_filter + (2 * (0 < tex->mipmaps))))));

	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_MAG_FILTER, TO_GL(tex->mag_filter)));
	
	int32_t const swizzle_mask[4]{
		(tex->swizzle_r == TextureSwizzle_Identity) ? GL_RED : TO_GL(tex->swizzle_r),
		(tex->swizzle_g == TextureSwizzle_Identity) ? GL_GREEN : TO_GL(tex->swizzle_g),
		(tex->swizzle_b == TextureSwizzle_Identity) ? GL_BLUE : TO_GL(tex->swizzle_b),
		(tex->swizzle_a == TextureSwizzle_Identity) ? GL_ALPHA : TO_GL(tex->swizzle_a) };
	glCheck(glTexParameteriv(sampler_type, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & texture_attachments)
{
	_Framebuffer * fb{ memnew(_Framebuffer{}) };
	fb->texture_attachments = texture_attachments;

	uint32_t & handle{ GL_RID(fb->handle) };
	glCheck(glGenFramebuffers(1, &handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, handle));

	uint32_t color_attachment_index{};
	for (size_t i = 0; i < fb->texture_attachments.size(); ++i)
	{
		_Texture * texture{ VALIDATE((_Texture *)fb->texture_attachments[i]) };
		if (i == 0) { fb->width = texture->width; }
		if (i == 0) { fb->height = texture->height; }

		if (is_depth_stencil_color_format(texture->color_format)) {
			FLAG_SET(texture->usage_flags, TextureFlags_DepthStencilAttachment);
		}
		else {
			FLAG_SET(texture->usage_flags, TextureFlags_ColorAttachment);
		}

		texture_bind((RID)texture);

		if (FLAG_READ(texture->usage_flags, TextureFlags_ColorAttachment))
		{
			ASSERT(!FLAG_READ(texture->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), TO_GL(texture->texture_type), GL_RID(texture->handle), 0));
		}
		else
		{
			ASSERT(FLAG_READ(texture->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TO_GL(texture->texture_type), GL_RID(texture->handle), 0));
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
	glCheck(glDeleteFramebuffers(1, &GL_RID(fb->handle)));
	memdelete(fb);
}

void RenderingDeviceOpenGL::framebuffer_bind(RID framebuffer)
{
	if (framebuffer) {
		_Framebuffer * const fb{ (_Framebuffer *)framebuffer };
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, GL_RID(fb->handle)));
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

	uint32_t & handle{ GL_RID(fb->handle) };
	if (handle) { glCheck(glDeleteFramebuffers(1, &handle)); }
	glCheck(glGenFramebuffers(1, &handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, handle));

	uint32_t color_attachment_index{};
	for (size_t i = 0; i < fb->texture_attachments.size(); ++i)
	{
		_Texture * texture{ VALIDATE((_Texture *)fb->texture_attachments[i]) };
		texture->width = fb->width;
		texture->height = fb->height;

		texture_update((RID)texture);

		if (FLAG_READ(texture->usage_flags, TextureFlags_ColorAttachment))
		{
			ASSERT(!FLAG_READ(texture->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), TO_GL(texture->texture_type), GL_RID(texture->handle), 0));
		}
		else
		{
			ASSERT(FLAG_READ(texture->usage_flags, TextureFlags_DepthStencilAttachment));
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, TO_GL(texture->texture_type), GL_RID(texture->handle), 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
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
	uint32_t & handle{ GL_RID(s->handle) };
	glCheck(handle = glCreateProgramObjectARB());

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

		glCheck(glAttachObjectARB(handle, obj));
		glCheck(glDeleteObjectARB(obj));
	}

	glCheck(glLinkProgramARB(handle));

	int32_t link_status;
	glCheck(glGetObjectParameterivARB(handle, GL_OBJECT_LINK_STATUS_ARB, &link_status));
	if (!link_status) {
		glCheck(glGetInfoLogARB(handle, sizeof(log_str), 0, nullptr));
		print_error(log_str);
		shader_destroy((RID)s);
		return RID{};
	}

	return (RID)s;
}

void RenderingDeviceOpenGL::shader_destroy(RID shader)
{
	_Shader * s{ VALIDATE((_Shader *)shader) };
	glCheck(glDeleteProgramsARB(1, &GL_RID(s->handle)));
	memdelete(s);
}

void RenderingDeviceOpenGL::shader_bind(RID shader)
{
	glCheck(glUseProgramObjectARB(shader ? GL_RID(((_Shader *)shader)->handle) : NULL));
}

struct RenderingDeviceOpenGL::UniformBinder final
{
	int32_t location{ -1 };

	uint32_t self{}, last{};

	operator bool() const noexcept { return -1 < location; }

	UniformBinder(_Shader & s, cstring name)
	{
		if (!name || !*name || !(self = GL_RID(s.handle))) { return; }

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

RID RenderingDeviceOpenGL::sampler_create(RD::SamplerState const & sampler_state)
{
	return RID();
}

void RenderingDeviceOpenGL::sampler_destroy(RID sampler)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::render_pipeline_create(RD::RasterizationState const & rasterization_state, RD::MultisampleState const & multisample_state, RD::DepthStencilState const & depthstencil_state, RD::ColorBlendState const & colorblend_state)
{
	_Pipeline * rp{ memnew(_Pipeline) };

	return (RID)rp;
}

void RenderingDeviceOpenGL::render_pipeline_destroy(RID pipeline)
{
	_Pipeline * const rp{ VALIDATE((_Pipeline *)rp) };
	memdelete(rp);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::drawlist_begin_for_screen(WindowID window, Color const & clear_color)
{
	ASSERT(!m_drawlist);
	ASSERT(window);

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
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, GL_RID(fb->handle)));
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
	_Pipeline * const rp{ VALIDATE((_Pipeline *)rp) };
	dl->state.pipeline_shader = rp->shader;
}

void RenderingDeviceOpenGL::drawlist_bind_uniforms(RID list, RID uniforms)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
	_UniformSet * const us{ VALIDATE((_UniformSet *)uniforms) };
}

void RenderingDeviceOpenGL::drawlist_bind_vertices(RID list, RID vertexarray)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
	if (dl->state.vertexarray == vertexarray) { return; }
	dl->state.vertexarray = vertexarray;
	_VertexArray * const va{ VALIDATE((_VertexArray *)vertexarray) };
	glCheck(glBindVertexArray(GL_RID(va->handle)));
}

void RenderingDeviceOpenGL::drawlist_bind_indices(RID list, RID indexbuffer)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };
	if (dl->state.indexbuffer == indexbuffer) { return; }
	dl->state.indexbuffer = indexbuffer;
	_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)indexbuffer) };
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(ib->handle)));
}

void RenderingDeviceOpenGL::drawlist_draw(RID list, bool use_indices, uint32_t instances, uint32_t procedural_vertices)
{
	_DrawList * const dl{ VALIDATE((_DrawList *)list) };

	// TODO: bind uniforms here

	_VertexArray * const va{ VALIDATE((_VertexArray *)dl->state.vertexarray) };
	if (use_indices)
	{
		_IndexBuffer * const ib{ VALIDATE((_IndexBuffer *)dl->state.indexbuffer) };
		for (RID const vertexbuffer : va->buffers) {
			_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vb->handle)));
			glCheck(glDrawElements(GL_TRIANGLES, ib->data.size() / sizeof(uint32_t), GL_UNSIGNED_INT, nullptr));
		}
	}
	else
	{
		for (RID const vertexbuffer : va->buffers) {
			_VertexBuffer * const vb{ VALIDATE((_VertexBuffer *)vertexbuffer) };
			glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vb->handle)));
			glCheck(glDrawArrays(GL_TRIANGLES, 0, vb->data.size() / sizeof(float_t)));
		}
	}
}

void RenderingDeviceOpenGL::drawlist_end()
{
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	
	if (m_drawlist) { memdelete(m_drawlist); m_drawlist = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingDeviceOpenGL::clear(Color const & color, bool depth_stencil)
{
	glCheck(glClearColor(color[0], color[1], color[2], color[3]));

	uint32_t mask{ GL_COLOR_BUFFER_BIT };

	if (depth_stencil) { FLAG_SET(mask, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); }

	glCheck(glClear(mask));
}

void RenderingDeviceOpenGL::set_viewport(IntRect const & rect)
{
	glCheck(glViewport(rect[0], rect[1], rect[2], rect[3]));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
