#include <editor/editor_panel.hpp>

using namespace ism;

EMBED_CLASS(EditorPanel, t) {}

EditorPanel::~EditorPanel() {}

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