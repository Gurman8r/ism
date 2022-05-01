#include <editor/editor_viewport.hpp>
#include <editor/editor_node.hpp>

using namespace ism;

OBJECT_EMBED(EditorViewport, t) {}

EditorViewport::EditorViewport()
	: EditorPanel	{ "Viewport", true, ImGuiWindowFlags_NoScrollbar }
	, m_main_texture{}
{
}

EditorViewport::~EditorViewport()
{
}

void EditorViewport::draw()
{
	m_dragging_view = false;

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
		window->DrawList->AddImage(m_main_texture, view_rect.Min, view_rect.Max, { 0, 1 }, { 1, 0 });
		m_dragging_view = !ImGuizmo::IsUsing() && ImGui::IsItemHovered() && ImGui::IsMouseDragging(0);
	}
	//ImGui::GetForegroundDrawList(window)->AddRect(view_rect.Min, view_rect.Max, 0xff00ffff);
}
