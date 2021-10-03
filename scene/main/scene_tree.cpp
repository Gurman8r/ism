#include <scene/main/scene_tree.hpp>
#include <scene/components/all_components.hpp>
#include <scene/gui/imgui.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(SceneTree::singleton) {};

EMBEDDED_CLASS_TYPE(SceneTree, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SceneTree::SceneTree(SceneSettings const & settings) : MainLoop{}
{
	if (!singleton) { singleton = this; }

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
	for_nodes([&](NODE & node) { node->m_tree = this; });
	MainLoop::initialize();
}

bool SceneTree::process(Duration const & delta_time)
{
	bool should_exit{ false };

	m_fps.update(delta_time);

	m_root->poll_events();

	// placeholder
	for_nodes([&](NODE & node) { node->begin_step(); });
	for_nodes([&](NODE & node) { node->step(delta_time); });
	for_nodes([&](NODE & node) { node->end_step(); });

	MainLoop::process(delta_time);

	m_root->swap_buffers();

	if (!m_root->is_open()) { should_exit = true; }

	return should_exit;
}

void SceneTree::finalize()
{
	m_initialized = false;
	MainLoop::finalize();
	m_root = nullptr;
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