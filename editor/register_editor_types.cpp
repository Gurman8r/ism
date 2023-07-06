#if TOOLS_ENABLED

#include <editor/register_editor_types.hpp>
#include <core/extension/extension_manager.hpp>

#include <editor/editor_node.hpp>
#include <editor/editor_assets.hpp>
#include <editor/editor_hierarchy.hpp>
#include <editor/editor_inspector.hpp>
#include <editor/editor_log.hpp>
#include <editor/editor_terminal.hpp>
#include <editor/editor_viewport.hpp>

namespace Ism
{
	void register_editor_types()
	{
		REGISTER_CLASS
		(
			EditorNode,

			// editor panels
			EditorPanel,
			EditorAssets,
			EditorHierarchy,
			EditorInspector,
			EditorLog,
			EditorTerminal,
			EditorViewport
		);
	}

	void register_editor_singletons()
	{
	}
	
	void unregister_editor_types()
	{
	}
}

#endif // TOOLS_ENABLED