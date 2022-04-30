#include <editor/editor_viewport.hpp>

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
	if (!is_open()) { return; }
	ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
	ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 1280.f, 720.f }, ImGuiCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f });
	bool open; EDITOR_PANEL_SCOPE(open);
	ImGui::PopStyleVar(1);
	if (!open) { return; }

	ImGuiWindow * const window{ get_window() };
	ImRect const rect{ window->InnerRect };
	if (ImGui::ItemAdd(rect, NULL)) { window->DrawList->AddImage(m_main_texture, rect.Min, rect.Max, { 0, 1 }, { 1, 0 }); }
	//ImGui::GetForegroundDrawList(window)->AddRect(rect.Min, rect.Max, 0xff00ffff);
}
