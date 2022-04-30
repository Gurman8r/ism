#include <editor/editor_hierarchy.hpp>

using namespace ism;

OBJECT_EMBED(EditorHierarchy, t) {}

EditorHierarchy::EditorHierarchy() : EditorPanel{ "Hierarchy" }
{
}

EditorHierarchy::~EditorHierarchy()
{
}

void EditorHierarchy::draw()
{
	if (!is_open()) { return; }
	ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
	ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 320.f, 180.f }, ImGuiCond_FirstUseEver);
	bool open; EDITOR_PANEL_SCOPE(open);
	if (!open) { return; }

	ImGui::Text("editor hierarchy goes here");
}
