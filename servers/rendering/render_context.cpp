#include <servers/rendering/render_context.hpp>

using namespace ism;

namespace ism
{
	RenderContext::~RenderContext()
	{
	}
}

RenderContext * ism::get_render_context()
{
	return nullptr;
}

RenderContext * ism::set_render_context(RenderContext * value)
{
	return nullptr;
}