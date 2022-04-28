#include <editor/register_editor_types.hpp>
#include <editor/editor_node.hpp>
#include <editor/editor_hierarchy.hpp>
#include <editor/editor_viewport.hpp>

using namespace ism;

void ism::register_editor_types()
{
	SINGLETON(Internals)->bind_class<
		EditorNode,
		EditorPanel,
		EditorHierarchy,
		EditorViewport
	>();
}

void ism::unregister_editor_types()
{
}