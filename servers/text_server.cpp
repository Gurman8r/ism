#include <servers/text_server.hpp>
#include <servers/rendering/rendering_device.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(Glyph, t) {}

Glyph::Glyph(DynamicBuffer const & buffer, IntRect const & bounds, uint32_t advance)
	: m_texture	{}
	, m_bounds	{ bounds }
	, m_advance	{ advance }
{
}

Glyph::~Glyph()
{
	if (m_texture) { RENDERING_DEVICE->texture_destroy(m_texture); }
}

Ref<Image> Glyph::get_image() const
{
	return {};
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(TextServer::singleton) {};

OBJECT_EMBED(TextServer, t) {}

TextServer::TextServer() { singleton = this; }

TextServer::~TextServer() {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */