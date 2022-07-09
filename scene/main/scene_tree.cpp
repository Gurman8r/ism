#include <scene/main/scene_tree.hpp>
#include <scene/main/window.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree * SceneTree::__singleton{};

	OBJECT_EMBED(SceneTree, t)
	{
		t.tp_install = CLASS_INSTALLER(SceneTree, t)
		{
			return t
				.def("initialize", &SceneTree::initialize)
				.def("process", &SceneTree::process)
				.def("finalize", &SceneTree::finalize)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::SceneTree() : MainLoop{}
	{
		if (__singleton == nullptr) { __singleton = this; }

		m_root = memnew(Window);
	}

	SceneTree::~SceneTree()
	{
		memdelete(m_root);

		if (__singleton == this) { __singleton = nullptr; }
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

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}