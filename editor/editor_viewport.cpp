#include <editor/editor_viewport.hpp>

using namespace ism;

EMBED_CLASS(EditorViewport, t) {}

EditorViewport::EditorViewport(Vec2f const (&uvs)[2], Color32 const & tint)
	: EditorPanel{ "Viewport", true, ImGuiWindowFlags_NoScrollbar }
	, m_main_texture{}
	, m_uv			{}
	, m_tint		{ tint }
{
	set_uvs(uvs);
}

EditorViewport::~EditorViewport()
{
}

void EditorViewport::draw()
{
	ImGuiViewport * const main_viewport{ ImGui::GetMainViewport() };
	ImGui::SetNextWindowPos(main_viewport->GetWorkPos() + Vec2{ 32, 32 }, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize({ 1280.f, 720.f }, ImGuiCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f });
	bool const is_open{ begin_window() };
	ImGui::PopStyleVar(1);
	if (is_open)
	{
		ImRect const r{ get_inner_rect() };
		if (ImGui::ItemAdd(r, NULL))
		{
			get_draw_list()->AddImage(m_main_texture, r.Min, r.Max, m_uv[0], m_uv[1], m_tint);
		}
	}
	end_window();
}
