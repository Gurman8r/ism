#include <editor/editor_viewport.hpp>
#include <editor/editor_node.hpp>

using namespace ism;

OBJECT_EMBED(EditorViewport, t) {}

EditorViewport::EditorViewport()
	: EditorPanel		{ "Viewport##Editor", true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar }
	, m_main_texture	{}
	, m_editor_camera	{}
	, m_grid_enabled	{ true }
	, m_grid_matrix		{ Mat4::identity() }
	, m_grid_size		{ 100.f }
{
	m_editor_camera.set_eye({ 0.f, 0.f, -5.f });
	m_editor_camera.set_yaw(90.f);
}

EditorViewport::~EditorViewport()
{
}

void EditorViewport::process(Duration const & dt)
{
	if (!is_open()) { return; }
	ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
	ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 1280.f, 720.f }, ImGuiCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f });
	bool open; EDITOR_PANEL_SCOPE(open);
	ImGui::PopStyleVar(1);
	if (!open) { return; }

	ImGuiWindow * const window{ get_window() };
	ImRect const view_rect{ window->InnerRect };

	if (ImGui::ItemAdd(view_rect, NULL)) {
		void * texture_handle{ RENDERING_DEVICE->texture_get_handle(m_main_texture) };
		window->DrawList->AddImage(texture_handle, view_rect.Min, view_rect.Max, { 0, 1 }, { 1, 0 });
	}

	bool const nav_enabled{ window == ImGui::GetCurrentContext()->NavWindow };
	bool const dragging_view{ nav_enabled && !ImGuizmo::IsUsing() && ImGui::IsItemHovered() && ImGui::IsMouseDragging(0) };

	if (dragging_view) {
		Vec2 const md{ INPUT->get_mouse_delta() * (float_t)dt * 50 };
		m_editor_camera.do_yaw(-md[0]);
		m_editor_camera.do_pitch(+md[1]);
	}

	if (ImGui::BeginMenuBar()) {
		ImGui::EndMenuBar();
	}

	ImGuizmo::SetDrawlist(window->DrawList);
	ImGuizmo::SetRect(view_rect.Min.x, view_rect.Min.y, view_rect.GetWidth(), view_rect.GetHeight());
	if (m_grid_enabled) { ImGuizmo::DrawGrid(m_editor_camera.get_view(), m_editor_camera.get_proj(), m_grid_matrix, m_grid_size); }
	//if (0 < m_object_count) { ImGuizmo::DrawCubes(view_matrix, proj_matrix, &m_object_matrix[0][0], m_object_count); }
	//for (int32_t i = 0; i < m_object_count; ++i) {
	//	ImGuizmo::SetID(i);
	//	if (m_xeditor.Manipulate(view_matrix, proj_matrix, m_object_matrix[i])) { m_object_index = i; }
	//}
}
