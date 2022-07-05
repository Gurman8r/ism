#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

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

SceneTree::SceneTree(SceneSettings const & settings) : MainLoop{}
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

	MainLoop::initialize();
}

bool SceneTree::process(Duration const & dt)
{
	MainLoop::process(dt);

	bool should_close{};

	m_fps.update(dt);

	m_root->process(dt);

	should_close |= m_root->get_should_close();

	return should_close;
}

void SceneTree::finalize()
{
	m_initialized = false;

	MainLoop::finalize();
}

void SceneTree::handle_event(Event const & event)
{
	MainLoop::handle_event(event);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */