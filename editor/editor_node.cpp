#include <editor/editor_node.hpp>
#include <scene/gui/imgui.hpp>

using namespace ism;

EMBEDED_CLASS(EditorNode, t) {}

EditorNode::EditorNode()
{
}

EditorNode::~EditorNode()
{
}

void EditorNode::process(Duration const & dt)
{
	char window_title[32]{};
	std::sprintf(window_title, "ism @ %.1f fps", get_tree()->get_framerate());
	get_tree()->get_root()->set_title(window_title);

	_show_dockspace();
	
	ImGui::ShowDemoWindow();

	Node::process(dt);
}

void EditorNode::_show_dockspace()
{
	bool const has_main_menu_bar{ /* TODO */ };
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		constexpr auto dockspace_name{ "##MainDockspace" };
		ImGuiID const dockspace_id{ ImGui::GetID(dockspace_name) };
		ImGuiViewportP * dockspace_vp{ ImGui::GetCurrentContext()->Viewports[0] };
		ImGui::SetNextWindowPos(dockspace_vp->Pos);
		ImGui::SetNextWindowSize(dockspace_vp->Size);
		ImGui::SetNextWindowViewport(dockspace_vp->ID);
		ImGui::SetNextWindowBgAlpha(0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, Vec2{ 0.f, 0.f });
		bool const dockspace_open{ ImGui::Begin(dockspace_name, nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBackground |
			(has_main_menu_bar ? ImGuiWindowFlags_MenuBar : ImGuiWindowFlags_None)
		) };
		ImGui::PopStyleVar(3);
		if (dockspace_open) {
			if (!ImGui::DockBuilderGetNode(dockspace_id)) {
				ImGui::DockBuilderRemoveNode(dockspace_id);
				ImGui::DockBuilderAddNode(dockspace_id);
				// BUILD DOCKSPACE HERE
				ImGui::DockBuilderFinish(dockspace_id);
			}
			ImGui::DockSpace(dockspace_id);
		}
		ImGui::End();
	}
}
