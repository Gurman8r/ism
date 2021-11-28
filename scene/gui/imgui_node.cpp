#include <scene/gui/imgui_node.hpp>

using namespace ism;

EMBED_CLASS(ImGuiNode, t) {}

ImGuiNode::ImGuiNode(cstring log_filename, cstring ini_filename) : Node{}
{
	m_imgui_context = ImGui::CreateContext();
	m_imgui_context->IO.LogFilename = log_filename;
	m_imgui_context->IO.IniFilename = ini_filename;
	m_imgui_context->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	m_imgui_context->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	m_imgui_context->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	VERIFY(ImGui_Init(get_display_server().get_context_main()));
}

ImGuiNode::~ImGuiNode()
{
	ImGui_Shutdown();
	ImGui::DestroyContext(m_imgui_context);
}

void ImGuiNode::begin_step()
{
	ImGui_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	bool const has_main_menu_bar{/* TODO */};
	
	if (get_io().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		constexpr auto dockspace_name{ "##MainDockspace" };
		ImGuiID const dockspace_id{ ImGui::GetID(dockspace_name) };
		ImGuiViewportP * dockspace_vp{ get_viewport(0) };
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

void ImGuiNode::step(Duration const &)
{
}

void ImGuiNode::end_step()
{
	ImGui::Render();

	render_immediate(RenderingDevice::get_singleton()
	, RenderingCommand::set_viewport(get_tree()->get_root()->get_bounds())
	, RenderingCommand::clear(Color{})
	);

	ImGui_RenderDrawData(&get_viewport(0)->DrawDataP);

	if (get_io().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		WindowID const backup{ get_display_server().get_context_current() };
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		get_display_server().make_context_current(backup);
	}
}