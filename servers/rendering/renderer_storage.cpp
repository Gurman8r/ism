#include <servers/rendering/renderer_storage.hpp>
#include <servers/rendering_server.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_EMBED(RendererStorage, t) {}

RendererStorage::RendererStorage()
{
}

RendererStorage::~RendererStorage()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RendererStorage::render_target_create()
{
	RenderTarget * render_target{ memnew(RenderTarget) };

	return (RID)render_target;
}

void RendererStorage::render_target_destroy(RID rid)
{
	ASSERT(rid);

	RenderTarget * render_target{ (RenderTarget *)rid };

	memdelete(render_target);
}

void RendererStorage::render_target_resize(RID rid, int32_t width, int32_t height)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */