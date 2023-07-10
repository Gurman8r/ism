#include <scene/main/scene_tree.hpp>
#include <scene/main/missing_node.hpp>
#include <scene/main/window.hpp>
#include <scene/gui/imgui.hpp>
#include <scene/resources/world_2d.hpp>
#include <scene/resources/world_3d.hpp>

// components
#include <scene/component/camera_behavior.hpp>
#include <scene/component/collider_component.hpp>
#include <scene/component/mesh_renderer_component.hpp>
#include <scene/component/rigidbody_component.hpp>
#include <scene/component/rigidbody2d_component.hpp>
#include <scene/component/sprite_renderer_component.hpp>
#include <scene/component/transform_component.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SINGLETON_EMBED(SceneTree);

	OBJECT_EMBED(SceneTree, t)
	{
		t.tp_bind = BIND_CLASS(SceneTree, klass)
		{
			return klass
				.def("initialize", &SceneTree::initialize)
				.def("process", &SceneTree::process)
				.def("finalize", &SceneTree::finalize)
				;
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	SceneTree::SceneTree()
	{
		if (has_singleton(nullptr)) { set_singleton(this); }

		m_root = memnew(Window);
		m_root->set_name("root");
		m_root->set_title("");
	}

	SceneTree::~SceneTree()
	{
		memdelete(m_root);

		if (has_singleton(this)) { set_singleton(nullptr); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::initialize()
	{
		m_initialized = true;
		m_root->set_tree(this);
		base_type::initialize();

		if (has_singleton(this) && !ImGui::GetCurrentContext()) {
			m_imgui = VALIDATE(ImGui_Initialize());
		}
	}

	bool SceneTree::process(Duration const & dt)
	{
		base_type::process(dt);
		m_fps_tracker.update(dt);
		m_delta_time = dt;

		if (m_imgui) {
			ImGui_BeginFrame(m_imgui);
		}

		m_root->propagate_notification(Notification_Internal_Process);
		m_root->propagate_notification(Notification_Process);

		if (m_imgui) {
			ImGui::Render();
			rendering_device()->draw_list_begin_for_screen();
			ImGui_RenderDrawData(&m_imgui->Viewports[0]->DrawDataP);
			rendering_device()->draw_list_end();
			ImGui_EndFrame(m_imgui);
		}

		return m_should_close;
	}

	void SceneTree::finalize()
	{
		m_initialized = false;
		base_type::finalize();

		if (m_imgui) {
			ImGui_Finalize(m_imgui);
			m_imgui = nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}