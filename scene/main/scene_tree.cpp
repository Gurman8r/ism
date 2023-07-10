#include <scene/main/scene_tree.hpp>
#include <core/config/engine.hpp>
#include <scene/gui/imgui.hpp>

#include <scene/main/window.hpp>
#include <scene/main/missing_node.hpp>
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
		t.tp_bind = CLASS_(SceneTree, klass)
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
		//m_root->set_title("");

		if (is_singleton() && !ImGui::GetCurrentContext()) {
			m_imgui = VALIDATE(ImGui_Initialize());
		}
	}

	SceneTree::~SceneTree()
	{
		if (is_singleton()) { set_singleton(nullptr); }
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::initialize()
	{
		m_initialized = true;
		m_root->set_tree(this);
		MainLoop::initialize();
	}

	bool SceneTree::process(Duration const & dt)
	{
		if (MainLoop::process(dt)) {
			m_should_close = true;
		}

		m_dt = dt;
		m_fps.update(dt);

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

		MainLoop::finalize();

		if (m_imgui) {
			ImGui_Finalize(m_imgui);
			m_imgui = nullptr;
		}

		if (m_root) {
			m_root->set_tree(nullptr);
			//m_root->_propagate_after_exit_tree();
			memdelete(m_root);
			m_root = nullptr;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void SceneTree::_notification(Notification_ id)
	{
		switch (id)
		{
		case Notification_TranslationChanged: {
			if (!engine()->is_editor_hint()) {
				get_root()->propagate_notification(id);
			}
		} break;

		case Notification_OS_MemoryWarning:
		case Notification_OS_IME_Update:
		case Notification_WM_About:
		case Notification_Crash:
		case Notification_ApplicationResumed:
		case Notification_ApplicationPaused:
		case Notification_ApplicationFocusOut:
		case Notification_ApplicationFocusIn: {
			get_root()->propagate_notification(id);
		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}