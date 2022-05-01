#include <editor/editor_viewport.hpp>
#include <editor/editor_node.hpp>

using namespace ism;

OBJECT_EMBED(EditorViewport, t) {}

EditorViewport::EditorViewport()
	: EditorPanel		{ "Editor Viewport", true, ImGuiWindowFlags_NoScrollbar }
	, m_main_texture	{}
	, m_editor_camera	{}
	, m_grid_enabled	{ true }
	, m_grid_matrix		{ Mat4::identity() }
	, m_grid_size		{ 100.f }
{
	//m_framebuffer.instance(FramebufferSpecification{ 1280, 720, { ColorFormat_R8G8B8_UNORM, ColorFormat_D24_UNORM_S8_UINT } });
	m_editor_camera.set_eye({ 0.f, 0.f, -5.f });
	m_editor_camera.set_yaw(90.f);
}

EditorViewport::~EditorViewport()
{
}

void EditorViewport::process(Duration const dt)
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
	bool dragging_view{};
	if (ImGui::ItemAdd(view_rect, NULL)) {
		window->DrawList->AddImage(m_main_texture, view_rect.Min, view_rect.Max, { 0, 1 }, { 1, 0 });
		dragging_view = !ImGuizmo::IsUsing() && ImGui::IsItemHovered() && ImGui::IsMouseDragging(0);
	}

	ImGuizmo::SetDrawlist(window->DrawList);
	ImGuizmo::SetRect(view_rect.Min.x, view_rect.Min.y, view_rect.GetWidth(), view_rect.GetHeight());
	if (m_grid_enabled) { ImGuizmo::DrawGrid(m_editor_camera.get_view(), m_editor_camera.get_proj(), m_grid_matrix, m_grid_size); }

	if (dragging_view) {
		Vec2 const md{ Input::get_singleton()->get_mouse_delta() * (float_t)dt * 50 };
		m_editor_camera.do_yaw(-md[0]);
		m_editor_camera.do_pitch(+md[1]);
	}

	//if (0 < m_object_count) { ImGuizmo::DrawCubes(view_matrix, proj_matrix, &m_object_matrix[0][0], m_object_count); }
	//for (int32_t i = 0; i < m_object_count; ++i) {
	//	ImGuizmo::SetID(i);
	//	if (m_xeditor.Manipulate(view_matrix, proj_matrix, m_object_matrix[i])) { m_object_index = i; }
	//}
}
