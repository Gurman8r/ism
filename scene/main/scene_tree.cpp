#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MEMBER_IMPL(SceneTree::g_singleton) {};

	OBJECT_EMBED(SceneTree, t)
	{
		t.tp_install = CLASS_INSTALLER(SceneTree, t)
		{
			return t
				.def("initialize", &SceneTree::initialize)
				.def("process", &SceneTree::process)
				.def("finalize", &SceneTree::finalize)
				.def("handle_event", &SceneTree::handle_event)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::SceneTree() : MainLoop{}
	{
		if (g_singleton == nullptr) { g_singleton = this; }

		m_root = memnew(Window);
	}

	SceneTree::~SceneTree()
	{
		memdelete(m_root);

		if (g_singleton == this) { g_singleton = nullptr; }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::initialize()
	{
		m_initialized = true;

		base_type::initialize();
	}

	bool SceneTree::process(Duration const & dt)
	{
		base_type::process(dt);

		m_fps_tracker.update(dt);

		m_root->process(dt);

		m_should_close |= m_root->should_close();

		return m_should_close;
	}

	void SceneTree::finalize()
	{
		m_initialized = false;

		base_type::finalize();
	}

	void SceneTree::handle_event(Event const & event)
	{
		base_type::handle_event(event);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}