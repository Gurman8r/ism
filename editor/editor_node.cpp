#include <editor/editor_node.hpp>
#include <scene/gui/imgui.hpp>
#include <servers/display_server.hpp>

using namespace ism;

EMBEDDED_CLASS_TYPE(EditorNode, t) {}

EditorNode::EditorNode()
{
	m_imgui_context = ImGui::CreateContext();
	m_imgui_context->IO.LogFilename = nullptr;
	m_imgui_context->IO.IniFilename = nullptr;
	m_imgui_context->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	m_imgui_context->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	m_imgui_context->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	VERIFY(ImGui_Init(get_display_server().get_context_current()));
}

EditorNode::~EditorNode()
{
	ImGui_Shutdown();
	ImGui::DestroyContext(m_imgui_context);
}