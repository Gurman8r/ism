#include <editor/editor_node.hpp>

using namespace ism;

EMBED_CLASS(EditorNode, t) {}

EditorNode::EditorNode() : Node{}
{
}

EditorNode::~EditorNode()
{
}

void EditorNode::initialize()
{
	m_gui = get_tree()->get_root()->find_instance<ImGuiNode>();

	VERIFY(m_gui);
}

void EditorNode::finalize()
{
}

void EditorNode::step(Duration const & delta_time)
{
	char window_title[32]{};
	std::sprintf(window_title, "ism @ %.1f fps", get_tree()->get_framerate());
	get_tree()->get_root()->set_title(window_title);

	ImGui::ShowDemoWindow();
}