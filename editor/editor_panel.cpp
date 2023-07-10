#if TOOLS_ENABLED

#include <editor/editor_panel.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OBJECT_EMBED(EditorPanel, t) {}

	EditorPanel::EditorPanel(cstring name, bool start_open, ImGuiWindowFlags flags)
		: m_window	{}
		, m_name	{ name }
		, m_is_open	{ start_open }
		, m_flags	{ flags }
	{
	}

	EditorPanel::~EditorPanel()
	{
	}

	bool EditorPanel::begin_window()
	{
		ImGui::PushID(this);
		bool const open{ ImGui::Begin(m_name, &m_is_open, m_flags) };
		m_window = (open && !m_window) ? ImGui::GetCurrentContext()->CurrentWindow : nullptr;
		m_is_focused = (open && m_window) && ImGui::IsWindowFocused();
		return open;
	}

	void EditorPanel::end_window()
	{
		ImGui::End();
		ImGui::PopID();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // TOOLS_ENABLED