#include <editor/editor_panel.hpp>

using namespace ism;

EMBED_CLASS(EditorPanel, t) {}

EditorPanel::~EditorPanel() {}

bool EditorPanel::begin_window()
{
	bool const is_open{ ImGui::Begin(m_name, &m_open, m_flags) };

	if (is_open) { update(); }

	return is_open;
}

void EditorPanel::end_window()
{
	ImGui::End();
}

void EditorPanel::update() noexcept
{
	ImGuiContext * const context{ VALIDATE(ImGui::GetCurrentContext()) };
	ImGuiWindow * const window{ VALIDATE(context->CurrentWindow) };
	do_update(window);
}

void EditorPanel::do_update(ImGuiWindow * window)
{
	m_window = VALIDATE(window);
}