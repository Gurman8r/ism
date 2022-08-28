#include <editor/register_editor_types.hpp>
#include <editor/editor_node.hpp>
#include <editor/editor_hierarchy.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_viewport.hpp>

namespace ism
{
	void register_editor_types()
	{
		Internals::get_singleton()->initialize_class
		<
			EditorNode,
			EditorPanel,
			EditorHierarchy,
			EditorLog,
			EditorViewport
		>();
	}
	
	void unregister_editor_types()
	{
	}
}