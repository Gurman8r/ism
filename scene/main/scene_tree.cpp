#include <scene/main/scene_tree.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::on_runtime_update(Duration dt)
	{
		if (!m_root) { return; }

		//m_reg.view<BehaviorComponent>().each([&](auto e, BehaviorComponent & scr)
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

	//template <> void SceneTree::on_component_added<tag_component>(Entity & e, TagComponent & c)
	//{
	//}
	//
	//template <> void SceneTree::on_component_added<transform_component>(Entity & e, TransformComponent & c)
	//{
	//}
	//
	//template <> void SceneTree::on_component_added<camera_component>(Entity & e, CameraComponent & c)
	//{
	//}
	//
	//template <> void SceneTree::on_component_added<behavior_component>(Entity & e, BehaviorComponent & c)
	//{
	//}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}