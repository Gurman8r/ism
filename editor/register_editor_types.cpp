#include <editor/register_editor_types.hpp>
#include <editor/editor_camera.hpp>
#include <editor/editor_node.hpp>
#include <editor/editor_hierarchy.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_viewport.hpp>

using namespace ism;

void ism::register_editor_types()
{
	REGISTER_OBJECT_CLASS
	(
		EditorCamera,
		EditorNode,

		EditorPanel,
		EditorHierarchy,
		EditorLog,
		EditorViewport
	);
}

void ism::unregister_editor_types()
{
}