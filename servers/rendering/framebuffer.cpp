#include <servers/rendering/framebuffer.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Framebuffer, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

Framebuffer::Framebuffer(FramebufferSpecification const & spec)
{
	m_framebuffer = RS->framebuffer_create(spec);
}

Framebuffer::~Framebuffer()
{
	if (m_framebuffer) { RD->framebuffer_destroy(m_framebuffer); }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void Framebuffer::bind()
{
	RD->framebuffer_bind(m_framebuffer);
}

void Framebuffer::unbind()
{
	RD->framebuffer_bind(nullptr);
}

void Framebuffer::resize(int32_t width, int32_t height)
{
	RD->framebuffer_resize(m_framebuffer, width, height);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

size_t Framebuffer::get_attachment_count() const
{
	return ((RD_Framebuffer *)VALIDATE(m_framebuffer))->texture_attachments.size();
}

RID * Framebuffer::get_attachment_data() const
{
	return ((RD_Framebuffer *)VALIDATE(m_framebuffer))->texture_attachments.data();
}

int32_t Framebuffer::get_width() const
{
	return ((RD_Framebuffer *)VALIDATE(m_framebuffer))->width;
}

int32_t Framebuffer::get_height() const
{
	return ((RD_Framebuffer *)VALIDATE(m_framebuffer))->height;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */