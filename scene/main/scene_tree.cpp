#include <scene/main/scene_tree.hpp>
#include <scene/components/components.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::~SceneTree()
	{
		if (m_root)
		{
			memdelete(m_root);

			m_root = nullptr;
		}
	}

	SceneTree::SceneTree(String const & name)
	{
		m_name = name.empty() ? "New Scene" : name;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::on_runtime_update(Duration dt)
	{
		if (!m_root) { return; }

		//m_reg.view<ScriptComponent>().each([&](auto e, ScriptComponent & scr)
		//{
		//	if (!scr.instance)
		//	{
		//		scr.instance = scr.create_instance();
		//	
		//		scr.instance->on_create();
		//	}
		//	
		//	scr.instance->on_update(dt);
		//});
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
}