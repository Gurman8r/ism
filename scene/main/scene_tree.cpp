#include <scene/main/scene_tree.hpp>
#include <scene/main/window.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree * SceneTree::__singleton{};

	OBJECT_EMBED(SceneTree, t)
	{
		t.tp_bind = CLASS_INSTALLER(SceneTree, t)
		{
			return t
				.def("initialize", &SceneTree::initialize)
				.def("process", &SceneTree::process)
				.def("finalize", &SceneTree::finalize)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::SceneTree()
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
		base_type::initialize();

		m_initialized = true;
	}

	bool SceneTree::process(Duration const & dt)
	{
		base_type::process(dt);
		m_fps_tracker.update(dt);
		m_delta_time = dt;

		m_root->propagate_notification(Node::Notification_Internal_Process);
		m_root->propagate_notification(Node::Notification_Process);

		m_should_close |= m_root->should_close();

		return m_should_close;
	}

	void SceneTree::finalize()
	{
		base_type::finalize();

		m_initialized = false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}