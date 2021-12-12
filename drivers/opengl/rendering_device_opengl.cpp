#if OPENGL_ENABLED

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

	SINGLETON(OS)->printerr(
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

EMBED_CLASS(RenderingDeviceOpenGL, t)
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

RID RenderingDeviceOpenGL::vertexarray_create(VertexLayout const & layout, RID indices, RID vertices)
{
	RD_Vertexarray * vertexarray{ memnew(RD_Vertexarray{}) };
	glCheck(glGenVertexArrays(1, &GL_RID(vertexarray->handle)));
	glCheck(glBindVertexArray(GL_RID(vertexarray->handle)));
	vertexarray->indices = indices;
	vertexarray->vertices = vertices;
	vertexarray->layout = layout;

	if (RD_Indexbuffer * indexbuffer{ (RD_Indexbuffer *)vertexarray->indices })
	{
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer->handle)));
	}

	if (RD_Vertexbuffer * vertexbuffer{ (RD_Vertexbuffer *)vertexarray->vertices })
	{
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer->handle)));

		for (size_t i = 0, imax = vertexarray->layout.elements.size(); i < imax; ++i)
		{
			VertexLayout::Element const & e{ vertexarray->layout.elements[i] };

			ASSERT(e.type == DataType_I32 || e.type == DataType_F32 || e.type == DataType_Bool);

			if (e.type == DataType_I32)
			{
				glCheck(glVertexAttribIPointer(
					(uint32_t)i,
					e.count,
					GL_INT,
					vertexarray->layout.stride,
					(void const *)(intptr_t)e.offset));
			}
			else
			{
				glCheck(glVertexAttribPointer(
					(uint32_t)i,
					e.count,
					e.type == DataType_F32 ? GL_FLOAT : GL_BOOL,
					e.normalized,
					vertexarray->layout.stride,
					(void const *)(intptr_t)e.offset));
			}
			glCheck(glEnableVertexAttribArray((uint32_t)i));
		}
	}

	return (RID)vertexarray;
}

void RenderingDeviceOpenGL::vertexarray_destroy(RID rid)
{
	ASSERT(rid);
	RD_Vertexarray * vertexarray{ (RD_Vertexarray *)rid };
	glCheck(glDeleteVertexArrays(1, &GL_RID(vertexarray->handle)));
	memdelete(vertexarray);
}

void RenderingDeviceOpenGL::vertexarray_bind(RID rid)
{
	ASSERT(rid);
	RD_Vertexarray * vertexarray{ (RD_Vertexarray *)rid };
	glCheck(glBindVertexArray(GL_RID(vertexarray->handle)));
}

void RenderingDeviceOpenGL::vertexarray_draw(RID rid)
{
	ASSERT(rid);
	RD_Vertexarray * vertexarray{ (RD_Vertexarray *)rid };
	ASSERT(vertexarray->vertices);
	glCheck(glBindVertexArray(GL_RID(vertexarray->handle)));

	if (RD_Indexbuffer * indexbuffer{ (RD_Indexbuffer *)vertexarray->indices })
	{
		// draw elements
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer->handle)));
		RD_Vertexbuffer * vertexbuffer{ (RD_Vertexbuffer *)vertexarray->vertices };
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer->handle)));
		glCheck(glDrawElements(GL_TRIANGLES, indexbuffer->data.size() / sizeof(uint32_t), GL_UNSIGNED_INT, nullptr));
	}
	else
	{
		// draw arrays
		RD_Vertexbuffer * vertexbuffer{ (RD_Vertexbuffer *)vertexarray->vertices };
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer->handle)));
		glCheck(glDrawArrays(GL_TRIANGLES, 0, vertexbuffer->data.size() / sizeof(float_t)));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::vertexbuffer_create(Buffer const & data)
{
	RD_Vertexbuffer * vertexbuffer{ memnew(RD_Vertexbuffer{}) };
	vertexbuffer->data = data;
	glCheck(glGenBuffers(1, &GL_RID(vertexbuffer->handle)));
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer->handle)));
	glCheck(glBufferData(GL_ARRAY_BUFFER, (uint32_t)vertexbuffer->data.size(), vertexbuffer->data.data(), vertexbuffer->data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
	return (RID)vertexbuffer;
}

void RenderingDeviceOpenGL::vertexbuffer_destroy(RID rid)
{
	ASSERT(rid);
	RD_Vertexbuffer * vertexbuffer{ (RD_Vertexbuffer *)rid };
	glCheck(glDeleteBuffers(1, &GL_RID(vertexbuffer->handle)));
	memdelete(vertexbuffer);
}

void RenderingDeviceOpenGL::vertexbuffer_bind(RID rid)
{
	if (rid) {
		RD_Vertexbuffer * vertexbuffer{ (RD_Vertexbuffer *)rid };
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer->handle)));
	}
	else {
		glCheck(glBindBuffer(GL_ARRAY_BUFFER, NULL));
	}
}

void RenderingDeviceOpenGL::vertexbuffer_update(RID rid, Buffer const & data, size_t offset)
{
	ASSERT(rid);
	RD_Vertexbuffer * vertexbuffer{ (RD_Vertexbuffer *)rid };
	vertexbuffer->data = data;
	glCheck(glBindBuffer(GL_ARRAY_BUFFER, GL_RID(vertexbuffer->handle)));
	glCheck(glBufferSubData(GL_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)vertexbuffer->data.size(), vertexbuffer->data.data()));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::indexbuffer_create(Buffer const & data)
{
	RD_Indexbuffer * indexbuffer{ memnew(RD_Indexbuffer{}) };
	indexbuffer->data = data;
	glCheck(glGenBuffers(1, &GL_RID(indexbuffer->handle)));
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer->handle)));
	glCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)indexbuffer->data.size(), indexbuffer->data.data(), indexbuffer->data.empty() ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
	return (RID)indexbuffer;
}

void RenderingDeviceOpenGL::indexbuffer_destroy(RID rid)
{
	ASSERT(rid);
	RD_Indexbuffer * indexbuffer{ (RD_Indexbuffer *)rid };
	glCheck(glDeleteBuffers(1, &GL_RID(indexbuffer->handle)));
	memdelete(indexbuffer);
}

void RenderingDeviceOpenGL::indexbuffer_bind(RID rid)
{
	if (rid) {
		RD_Indexbuffer * indexbuffer{ (RD_Indexbuffer *)rid };
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer->handle)));
	}
	else {
		glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL));
	}
}

void RenderingDeviceOpenGL::indexbuffer_update(RID rid, Buffer const & data, size_t offset)
{
	ASSERT(rid);
	RD_Indexbuffer * indexbuffer{ (RD_Indexbuffer *)rid };
	indexbuffer->data = data;
	glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_RID(indexbuffer->handle)));
	glCheck(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)offset, (uint32_t)indexbuffer->data.size(), indexbuffer->data.data()));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::texture_create(TextureFormat const & format, Buffer const & data)
{
	RD_Texture * texture{ memnew(RD_Texture{}) };
	texture->texture_type = format.texture_type;
	texture->color_format = format.color_format;
	texture->width_2d = texture->width = format.width;
	texture->height_2d = texture->height = format.height;
	texture->depth = format.depth;
	texture->layers = format.layers;
	texture->mipmaps = format.mipmaps;
	texture->samples = format.samples;
	texture->repeat_s = format.repeat_s;
	texture->repeat_t = format.repeat_t;
	texture->min_filter = format.min_filter;
	texture->mag_filter = format.mag_filter;
	texture->usage_flags = format.usage_flags;
	texture->color_format_srgb = format.color_format_srgb;
	texture->swizzle_r = format.swizzle_r;
	texture->swizzle_g = format.swizzle_g;
	texture->swizzle_b = format.swizzle_b;
	texture->swizzle_a = format.swizzle_a;

	get_color_format_info(texture->color_format, &texture->image_format, 0, 0, 0);

	_texture_update((RID)texture, data.data());

	return (RID)texture;
}

void RenderingDeviceOpenGL::texture_destroy(RID rid)
{
	ASSERT(rid);
	RD_Texture * texture{ (RD_Texture *)rid };
	glCheck(glDeleteTextures(1, &GL_RID(texture->handle)));
	memdelete(texture);
}

void RenderingDeviceOpenGL::texture_bind(RID rid, size_t slot)
{
	glCheck(glBindTextureUnit((uint32_t)slot, rid ? GL_RID(((RD_Texture *)rid)->handle) : NULL));
}

void RenderingDeviceOpenGL::texture_update(RID rid, Buffer const & data)
{
	ASSERT(rid);
	RD_Texture * texture{ (RD_Texture *)rid };
	if (texture->handle) { glCheck(glDeleteTextures(1, &GL_RID(texture->handle))); }
	_texture_update((RID)texture, data.data());
}

Buffer RenderingDeviceOpenGL::texture_get_data(RID rid)
{
	ASSERT(rid);
	RD_Texture * texture{ (RD_Texture *)rid };
	uint32_t const sampler_type{ TO_GL(texture->texture_type) };

	uint32_t color_composition{}, pixel_data_type{};
	get_color_format_info(texture->color_format, 0, &color_composition, 0, &pixel_data_type);

	size_t channel_count{};
	switch (texture->image_format)
	{
	case ImageFormat_R8: { channel_count = 1; } break;
	case ImageFormat_RG8: { channel_count = 2; } break;
	case ImageFormat_RGB8: { channel_count = 3; } break;
	case ImageFormat_RGBA8: { channel_count = 4; } break;
	}

	Buffer data;
	data.resize((size_t)(texture->width * texture->height * channel_count));
	glCheck(glBindTexture(sampler_type, GL_RID(texture->handle)));
	glCheck(glGetTexImage(sampler_type, 0, color_composition, pixel_data_type, data.data()));
	return data;
}

void RenderingDeviceOpenGL::_texture_update(RID rid, void const * data)
{
	ASSERT(rid);

	RD_Texture * texture{ (RD_Texture *)rid };

	uint32_t color_composition{}, pixel_data_format{}, pixel_data_type{};
	get_color_format_info(texture->color_format, 0, &color_composition, &pixel_data_format, &pixel_data_type);

	uint32_t & handle{ GL_RID(texture->handle) };
	uint32_t const sampler_type{ TO_GL(texture->texture_type) };
	glCheck(glGenTextures(1, &handle));
	glCheck(glBindTexture(sampler_type, handle));

	if (texture->texture_type == TextureType_2D) {
		glCheck(glTexImage2D(sampler_type, 0, color_composition, texture->width, texture->height, 0, pixel_data_format, pixel_data_type, data));
	}

	if (0 < texture->mipmaps) {
		glCheck(glGenerateMipmap(sampler_type));
	}
	
	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_WRAP_S, TO_GL(texture->repeat_s)));

	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_WRAP_T, TO_GL(texture->repeat_t)));
	
	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_MIN_FILTER, TO_GL((SamplerFilter_)(texture->min_filter + (2 * (0 < texture->mipmaps))))));

	glCheck(glTexParameteri(sampler_type, GL_TEXTURE_MAG_FILTER, TO_GL(texture->mag_filter)));
	
	int32_t const swizzle_mask[4]{
		(texture->swizzle_r == TextureSwizzle_Identity) ? GL_RED : TO_GL(texture->swizzle_r),
		(texture->swizzle_g == TextureSwizzle_Identity) ? GL_GREEN : TO_GL(texture->swizzle_g),
		(texture->swizzle_b == TextureSwizzle_Identity) ? GL_BLUE : TO_GL(texture->swizzle_b),
		(texture->swizzle_a == TextureSwizzle_Identity) ? GL_ALPHA : TO_GL(texture->swizzle_a) };
	glCheck(glTexParameteriv(sampler_type, GL_TEXTURE_SWIZZLE_RGBA, swizzle_mask));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingDeviceOpenGL::framebuffer_create(Vector<RID> const & texture_attachments)
{
	RD_Framebuffer * framebuffer{ memnew(RD_Framebuffer{}) };
	framebuffer->texture_attachments = texture_attachments;

	uint32_t & handle{ GL_RID(framebuffer->handle) };
	glCheck(glGenFramebuffers(1, &handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, handle));

	uint32_t color_attachment_index{};

	for (size_t i = 0; i < framebuffer->texture_attachments.size(); ++i)
	{
		ASSERT(framebuffer->texture_attachments[i]);

		RD_Texture * texture{ (RD_Texture *)framebuffer->texture_attachments[i] };

		if (i == 0) { framebuffer->width = texture->width; framebuffer->height = texture->height; }

		uint32_t & texture_handle{ GL_RID(texture->handle) };

		uint32_t const sampler_type{ TO_GL(texture->texture_type) };
		
		glCheck(glBindTexture(sampler_type, texture_handle));

		bool const
			is_color{ FLAG_READ(texture->usage_flags, TextureFlags_ColorAttachment) },
			is_depth{ FLAG_READ(texture->usage_flags, TextureFlags_DepthStencilAttachment) };

		if (!is_color && !is_depth)
		{
			FLAG_SET(texture->usage_flags, TextureFlags_ColorAttachment);
		}

		ASSERT((is_color && !is_depth) || (is_depth && !is_color));

		if (is_color)
		{
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + (color_attachment_index++), sampler_type, texture_handle, 0));
		}
		else
		{
			glCheck(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, sampler_type, texture_handle, 0));
		}
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	return (RID)framebuffer;
}

void RenderingDeviceOpenGL::framebuffer_destroy(RID rid)
{
	ASSERT(rid);
	RD_Framebuffer * framebuffer{ (RD_Framebuffer *)rid };
	glCheck(glDeleteFramebuffers(1, &GL_RID(framebuffer->handle)));
	memdelete(framebuffer);
}

void RenderingDeviceOpenGL::framebuffer_bind(RID rid)
{
	if (rid) {
		RD_Framebuffer * framebuffer{ (RD_Framebuffer *)rid };
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, GL_RID(framebuffer->handle)));
		glCheck(glViewport(0, 0, framebuffer->width, framebuffer->height));
	}
	else {
		glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
	}
}

void RenderingDeviceOpenGL::framebuffer_resize(RID rid, int32_t width, int32_t height)
{
	ASSERT(rid);
	RD_Framebuffer * framebuffer{ (RD_Framebuffer *)rid };
	if (framebuffer->width == width && framebuffer->height == height) { return; }
	framebuffer->width = width;
	framebuffer->height = height;

	uint32_t & handle{ GL_RID(framebuffer->handle) };
	glCheck(glDeleteFramebuffers(1, &handle));
	glCheck(glGenFramebuffers(1, &handle));
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, handle));

	for (size_t i = 0; i < framebuffer->texture_attachments.size(); ++i)
	{
		RD_Texture * texture{ (RD_Texture *)framebuffer->texture_attachments[i] };
		texture->width = framebuffer->width;
		texture->height = framebuffer->height;
		texture_update((RID)texture);
	}

	ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	glCheck(glBindFramebuffer(GL_FRAMEBUFFER, NULL));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static uint32_t _compile_shader(ShaderStage_ shader_stage, cstring source_code)
{
	ASSERT(source_code && *source_code);
	uint32_t obj{};
	glCheck(obj = glCreateShaderObjectARB(TO_GL(shader_stage)));
	glCheck(glShaderSourceARB(obj, 1, &source_code, nullptr));
	glCheck(glCompileShaderARB(obj));
	return obj;
}

RID RenderingDeviceOpenGL::shader_create(Vector<ShaderStageData> const & stage_data)
{
	static char log_str[1024]{};
	ASSERT(!stage_data.empty());
	RD_Shader * shader{ memnew(RD_Shader{}) };
	uint32_t & handle{ GL_RID(shader->handle) };
	glCheck(handle = glCreateProgramObjectARB());

	for (ShaderStageData const & stage : stage_data) {
		if (!stage.source.empty()) {
			shader->stage_data.push_back(stage);
		}
	}

	for (ShaderStageData & stage : shader->stage_data)
	{
		uint32_t obj{ _compile_shader(stage.shader_stage, stage.source.c_str()) };

		int32_t compile_status;
		glCheck(glGetObjectParameterivARB(obj, GL_OBJECT_COMPILE_STATUS_ARB, &compile_status));
		if (!compile_status) {
			glCheck(glGetInfoLogARB(obj, sizeof(log_str), 0, nullptr));
			print_error(log_str);
			glCheck(glDeleteObjectARB(obj));
			shader_destroy((RID)shader);
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
		shader_destroy((RID)shader);
		return RID{};
	}

	return (RID)shader;
}

void RenderingDeviceOpenGL::shader_destroy(RID rid)
{
	ASSERT(rid);
	RD_Shader * shader{ (RD_Shader *)rid };
	glCheck(glDeleteProgramsARB(1, &GL_RID(shader->handle)));
	memdelete(shader);
}

void RenderingDeviceOpenGL::shader_bind(RID rid)
{
	glCheck(glUseProgramObjectARB(rid ? GL_RID(((RD_Shader *)rid)->handle) : NULL));
}

struct UniformBinder final
{
	int32_t location{ -1 };

	uint32_t self{}, last{};

	operator bool() const noexcept { return -1 < location; }

	UniformBinder(RD_Shader & shader, cstring name)
	{
		if (!name || !*name || !(self = GL_RID(shader.handle))) { return; }

		glCheck(last = glGetHandleARB(GL_PROGRAM_OBJECT_ARB));

		if (last != self) { glCheck(glUseProgramObjectARB(self)); }
		
		location = shader.bindings.find_or_add_fn(hash(name, std::strlen(name)), [&]()
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

int32_t RenderingDeviceOpenGL::shader_uniform_location(RID rid, cstring name)
{
	return UniformBinder{ *VALIDATE((RD_Shader *)rid), name }.location;
}

void RenderingDeviceOpenGL::shader_set_uniform1i(RID rid, cstring name, int32_t const value)
{
	if (UniformBinder u{ *VALIDATE((RD_Shader *)rid), name })
	{
		glCheck(glUniform1iARB(u.location, value));
	}
}

void RenderingDeviceOpenGL::shader_set_uniform1f(RID rid, cstring name, float_t const value)
{
	if (UniformBinder u{ *VALIDATE((RD_Shader *)rid), name })
	{
		glCheck(glUniform1fARB(u.location, value));
	}
}

void RenderingDeviceOpenGL::shader_set_uniform2f(RID rid, cstring name, Vec2f const & value)
{
	if (UniformBinder u{ *VALIDATE((RD_Shader *)rid), name })
	{
		glCheck(glUniform2fARB(u.location, value[0], value[1]));
	}
}

void RenderingDeviceOpenGL::shader_set_uniform3f(RID rid, cstring name, Vec3f const & value)
{
	if (UniformBinder u{ *VALIDATE((RD_Shader *)rid), name })
	{
		glCheck(glUniform3fARB(u.location, value[0], value[1], value[2]));
	}
}

void RenderingDeviceOpenGL::shader_set_uniform4f(RID rid, cstring name, Vec4f const & value)
{
	if (UniformBinder u{ *VALIDATE((RD_Shader *)rid), name })
	{
		glCheck(glUniform4fARB(u.location, value[0], value[1], value[2], value[3]));
	}
}

void RenderingDeviceOpenGL::shader_set_uniform16f(RID rid, cstring name, Mat4f const & value, bool transpose)
{
	if (UniformBinder u{ *VALIDATE((RD_Shader *)rid), name })
	{
		glCheck(glUniformMatrix4fvARB(u.location, 1, transpose, value));
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // OPENGL_ENABLED
