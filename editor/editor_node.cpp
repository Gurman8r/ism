#include <editor/editor_node.hpp>
#include <scene/gui/imgui.hpp>
#include <servers/display_server.hpp>

using namespace ism;

OBJECT_EMBED(EditorNode, t) {}

EditorNode::EditorNode()
{
	m_imgui_ctx = ImGui::CreateContext();
	m_imgui_ctx->IO.LogFilename = nullptr;
	m_imgui_ctx->IO.IniFilename = nullptr;
	m_imgui_ctx->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	m_imgui_ctx->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	m_imgui_ctx->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	VERIFY(ImGui_Init(get_display_server().get_context_current()));
}

EditorNode::~EditorNode()
{
	ImGui_Shutdown();
	ImGui::DestroyContext(m_imgui_ctx);
}