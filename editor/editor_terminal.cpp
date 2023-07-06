#if TOOLS_ENABLED

#include <editor/editor_terminal.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EditorTerminal, t) {}

	EditorTerminal::EditorTerminal() : EditorPanel{ "Terminal##Editor" }
	{
	}

	EditorTerminal::~EditorTerminal()
	{
	}

	void EditorTerminal::process(Duration const & delta_time)
	{
		if (!is_open()) { return; }
		ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 320.f, 180.f }, ImGuiCond_FirstUseEver);
		bool open; EDITOR_PANEL_SCOPE(open);
		if (!open) { return; }

		ImGui::Text("terminal goes here");
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // TOOLS_ENABLED