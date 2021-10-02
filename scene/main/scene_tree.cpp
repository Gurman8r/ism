#include <scene/main/scene_tree.hpp>
#include <scene/components/all_components.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(SceneTree::singleton) {};

EMBEDDED_CLASS_TYPE(SceneTree, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SceneTree::SceneTree(SceneSettings const & settings) : MainLoop{}
{
	if (!singleton) { singleton = this; }

	m_root = memnew(Window);
}

SceneTree::~SceneTree()
{
	if (m_root) { memdelete(m_root); m_root = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SceneTree::initialize()
{
	m_initialized = true;
	m_root->set_tree(this);
	MainLoop::initialize();
}

bool SceneTree::process(Duration const & delta_time)
{
	bool should_exit{ false };

	MainLoop::process(delta_time);

	if (!m_root->is_open()) { should_exit = true; }

	return should_exit;
}

void SceneTree::finalize()
{
	m_initialized = false;

	MainLoop::finalize();

	if (m_root) { memdelete(m_root); m_root = nullptr; }
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

template <> void SceneTree::on_component_added<CameraComponent>(Node & e, CameraComponent & c)
{
}

template <> void SceneTree::on_component_added<ScriptComponent>(Node & e, ScriptComponent & c)
{
}

template <> void SceneTree::on_component_added<TagComponent>(Node & e, TagComponent & c)
{
}

template <> void SceneTree::on_component_added<TransformComponent>(Node & e, TransformComponent & c)
{
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */