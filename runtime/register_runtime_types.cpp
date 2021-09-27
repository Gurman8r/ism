#include <runtime/register_runtime_types.hpp>
#include <runtime/window/display_context.hpp>
#include <runtime/renderer/rendering_context.hpp>

using namespace ism;

void ism::register_runtime_types()
{
	get_internals()
		.bind_class<DisplayContext>()
		.bind_class<RenderingContext>()
		;
}

void ism::unregister_server_types()
{
}