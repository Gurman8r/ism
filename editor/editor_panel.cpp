#if TOOLS_ENABLED

#include <editor/editor_panel.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EMBED_CLASS(EditorPanel, t) {}

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

		if (open)
		{
			if (!m_window)
			{
				m_window = ImGui::GetCurrentContext()->CurrentWindow;
			}
		}

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