#include <servers/rendering/render_target.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Framebuffer, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Framebuffer::Framebuffer(FramebufferSpecification const & spec)
{
	ASSERT(0 < spec.width && 0 < spec.height);

	Vector<RID> textures;

	textures.reserve(spec.attachments.size());

	for (ColorFormat_ const color_format : spec.attachments)
	{
		textures.push_back(RD::get_singleton()->texture_create
		({
			TextureType_2D,
			color_format,
			spec.width,
			spec.height,
			1, 1, 0,
			SamplerFilter_Linear,
			SamplerFilter_Linear,
			SamplerRepeatMode_ClampToEdge,
			SamplerRepeatMode_ClampToEdge,
			spec.samples,
			spec.usage_flags | (color_format == ColorFormat_D24_UNORM_S8_UINT ? TextureFlags_DepthStencilAttachment : TextureFlags_ColorAttachment)
		}));
	}

	m_framebuffer = RD::get_singleton()->framebuffer_create(std::move(textures));
}

Framebuffer::~Framebuffer()
{
	if (m_framebuffer) { RD::get_singleton()->framebuffer_destroy(m_framebuffer); }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Framebuffer::bind()
{
	RD::get_singleton()->framebuffer_bind(m_framebuffer);
}

void Framebuffer::unbind()
{
	RD::get_singleton()->framebuffer_bind(nullptr);
}

void Framebuffer::resize(int32_t width, int32_t height)
{
	RD::get_singleton()->framebuffer_resize(m_framebuffer, width, height);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t Framebuffer::get_attachment_count() const
{
	return ((RD::Framebuffer *)VALIDATE(m_framebuffer))->texture_attachments.size();
}

RID * Framebuffer::get_attachment_data() const
{
	return ((RD::Framebuffer *)VALIDATE(m_framebuffer))->texture_attachments.data();
}

int32_t Framebuffer::get_width() const
{
	return ((RD::Framebuffer *)VALIDATE(m_framebuffer))->width;
}

int32_t Framebuffer::get_height() const
{
	return ((RD::Framebuffer *)VALIDATE(m_framebuffer))->height;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */