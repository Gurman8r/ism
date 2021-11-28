#include <scene/main/scene_tree.hpp>
#include <scene/components/all_components.hpp>
#include <scene/gui/imgui.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

MEMBER_IMPL(SceneTree::singleton) {};

EMBED_CLASS(SceneTree, t) {}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

SceneTree::SceneTree(SceneSettings const & settings) : MainLoop{}
{
	if (!singleton) { singleton = this; }

	m_root.instance();
	m_root->m_tree = this;
}

SceneTree::~SceneTree()
{
	m_root = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void SceneTree::initialize()
{
	m_initialized = true;
	for_nodes([&](NODE & node) { node->m_tree = this; node->initialize(); });
	MainLoop::initialize();
}

bool SceneTree::process(Duration const & delta_time)
{
	bool should_exit{ false };

	m_fps.update(delta_time);

	m_root->poll_events();

	for_nodes([&](NODE & node) { node->begin_step(); });
	for_nodes([&](NODE & node) { node->step(delta_time); });
	for_nodes([&](NODE & node) { node->end_step(); });

	MainLoop::process(delta_time);

	m_root->swap_buffers();

	should_exit |= m_root->get_should_close();

	return should_exit;
}

void SceneTree::finalize()
{
	m_initialized = false;
	MainLoop::finalize();
	for_nodes([&](NODE & node) { node->finalize(); }, true, true);
	m_root = nullptr;
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