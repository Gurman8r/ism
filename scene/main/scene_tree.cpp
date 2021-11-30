#include <scene/main/scene_tree.hpp>
#include <scene/components/all_components.hpp>
#include <scene/gui/imgui.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(SceneTree::singleton) {};

EMBEDED_CLASS(SceneTree, t) {}

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

void SceneTree::finalize()
{
	m_initialized = false;

	MainLoop::finalize();
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <> void SceneTree::on_component_added<CameraComponent>(Entity & e, CameraComponent & c)
{
}

template <> void SceneTree::on_component_added<ScriptComponent>(Entity & e, ScriptComponent & c)
{
}

template <> void SceneTree::on_component_added<TagComponent>(Entity & e, TagComponent & c)
{
}

template <> void SceneTree::on_component_added<TransformComponent>(Entity & e, TransformComponent & c)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */