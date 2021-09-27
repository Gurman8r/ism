#include <runtime/renderer/rendering_context_default.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OBJECT_IMPL(RenderingContextDefault, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RenderingContextDefault::RenderingContextDefault() : RenderingContext{}
{
}

RenderingContextDefault::~RenderingContextDefault()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void RenderingContextDefault::initialize()
{
}

void RenderingContextDefault::finalize()
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingContextDefault::texture2d_create(Ref<Image> const & image)
{
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingContextDefault::shader_create()
{
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingContextDefault::material_create()
{
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

RID RenderingContextDefault::mesh_create()
{
	return nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */