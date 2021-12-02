#include <scene/main/scene_tree.hpp>
#include <scene/gui/imgui.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(SceneTree::singleton) {};

EMBEDED_CLASS(SceneTree, t)
{
	CLASS_DEF(SceneTree, t)
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
	singleton = this;

	m_root.instance();
}

SceneTree::~SceneTree()
{
	m_root = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SceneTree::initialize()
{
	m_initialized = true;

	//m_root->set_tree(this);

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