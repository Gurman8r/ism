#include <editor/register_editor_types.hpp>
#include <editor/editor_node.hpp>
#include <editor/editor_interface.hpp>

using namespace ism;

void ism::register_editor_types()
{
	SINGLETON(Internals)->bind_class<
		EditorNode,
		EditorInterface
	>();
}

void ism::unregister_editor_types()
{
}