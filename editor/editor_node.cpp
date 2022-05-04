#include <editor/editor_node.hpp>
#include <scene/gui/imgui.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

static Mat4 object_matrix[] = // 
{
	{
		1.f, 0.f, 0.f, 0.f,
		0.f, 1.f, 0.f, 0.f,
		0.f, 0.f, 1.f, 0.f,
		0.f, 0.f, 0.f, 1.f
	},
};

MEMBER_IMPL(EditorNode::singleton) {};

OBJECT_EMBED(EditorNode, t) {}

EditorNode::EditorNode()
{
	singleton = this;

	textures["earth_dm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_dm_2k.png");
	meshes["sphere32x24"].instance("../../../assets/meshes/sphere32x24.obj");
	shaders["2d"].instance("../../../assets/shaders/2d.json");
	shaders["3d"].instance("../../../assets/shaders/3d.json");

	Vector<RID> fb_textures{
		RENDERING_DEVICE->texture_create({ TextureType_2D, ColorFormat_R8G8B8_UNORM, 1280, 720 }),
		RENDERING_DEVICE->texture_create({ TextureType_2D, ColorFormat_D24_UNORM_S8_UINT, 1280, 720 }) };
	m_framebuffer = RENDERING_DEVICE->framebuffer_create(fb_textures);
	m_editor_view.set_main_texture(fb_textures[0]);

	m_pipeline = RENDERING_DEVICE->pipeline_create(
		shaders["3d"]->get_rid(),
		RenderPrimitive_Triangles,
		COMPOSE(RD::RasterizationState, rs) {},
		COMPOSE(RD::MultisampleState, ms) {},
		COMPOSE(RD::DepthStencilState, ds) {},
		RD::ColorBlendState::create_blend());
}

EditorNode::~EditorNode()
{
	RENDERING_DEVICE->framebuffer_destroy(m_framebuffer);
	RENDERING_DEVICE->pipeline_destroy(m_pipeline);
}

void EditorNode::process(Duration const dt)
{
	char window_title[32]{};
	std::sprintf(window_title, "ism @ %.1f fps", (float_t)get_tree()->get_fps());
	get_tree()->get_root()->set_title(window_title);

	static EditorCamera * editor_camera{ m_editor_view.get_editor_camera() };
	static Vec2 view_size{ 1280, 720 }, view_size_prev{};
	if (m_editor_view.get_window()) { view_size = m_editor_view->InnerRect.GetSize(); }
	editor_camera->set_res(view_size);
	editor_camera->recalculate();
	if (view_size_prev != view_size) {
		RENDERING_DEVICE->framebuffer_set_size(m_framebuffer, (int32_t)view_size[0], (int32_t)view_size[1]);
		view_size_prev = view_size;
	}

	{
		static Color clear_color{ Colors::magenta };
		clear_color = rotate_hue(clear_color, (float_t)dt * 10.f);
		RID dl{ RENDERING_DEVICE->drawlist_begin(m_framebuffer, clear_color) };
		RENDERING_DEVICE->drawlist_bind_pipeline(dl, m_pipeline);
		RENDERING_DEVICE->drawlist_bind_uniforms(dl, nullptr);

		static Shader * shader{ *shaders["3d"] };
		static Texture * texture{ *textures["earth_dm_2k"] };
		shader->set_uniform("Model", object_matrix[0]);
		shader->set_uniform("View", editor_camera->get_view());
		shader->set_uniform("Proj", editor_camera->get_proj());
		shader->set_uniform("Texture0", texture->get_rid());

		static Mesh * mesh{ *meshes["sphere32x24"] };
		mesh->for_data([&](RID vertexarray, RID indexbuffer, auto &) {
			RENDERING_DEVICE->drawlist_bind_vertices(dl, vertexarray);
			RENDERING_DEVICE->drawlist_bind_indices(dl, indexbuffer);
			RENDERING_DEVICE->drawlist_draw(dl, true, 0, 0);
		});
		
		RENDERING_DEVICE->drawlist_end();
	}

	_draw_dockspace();
	if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }
	m_hierarchy.process(dt);
	m_editor_log.process(dt);
	m_editor_view.process(dt);

	Node::process(dt);
}

void EditorNode::_draw_dockspace()
{
	static bool opt_fullscreen{ true };
	static bool opt_padding{ false };
	static ImGuiDockNodeFlags dockspace_flags{ ImGuiDockNodeFlags_None };
	
	ImGuiWindowFlags window_flags{ ImGuiWindowFlags_NoDocking };
	if (m_show_main_menu_bar) { window_flags |= ImGuiWindowFlags_MenuBar; }
	
	if (opt_fullscreen) {
		ImGuiViewport * viewport{ ImGui::GetMainViewport() };
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}
	else {
		dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
	}

	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) { window_flags |= ImGuiWindowFlags_NoBackground; }
	
	if (!opt_padding) { ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f }); }
	
	bool const dockspace_open{ ImGui::Begin("Editor", nullptr, window_flags) };
	
	if (!opt_padding) { ImGui::PopStyleVar(); }
	
	if (opt_fullscreen) { ImGui::PopStyleVar(2); }
	
	ImGuiID const dockspace_id{ ImGui::GetID("##EditorDockSpace") };

	if (dockspace_open && !ImGui::DockBuilderGetNode(dockspace_id)) {
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id);
		_build_dockspace();
		ImGui::DockBuilderFinish(dockspace_id);
	}
	
	ImGui::DockSpace(dockspace_id, { 0.f, 0.f }, dockspace_flags);

	if (m_show_main_menu_bar && ImGui::BeginMenuBar()) {
		_draw_menu_bar();
		ImGui::EndMenuBar();
	}

	ImGui::End();
}

void EditorNode::_build_dockspace()
{
	ImGuiID const dockspace_id{ ImGui::GetID("##EditorDockSpace") };
	ImGuiID right{ dockspace_id };
	ImGuiID left_up{ ImGui::DockBuilderSplitNode(right, ImGuiDir_Left, 0.2f, nullptr, &right) };
	ImGuiID left_down{ ImGui::DockBuilderSplitNode(left_up, ImGuiDir_Down, 0.5f, nullptr, &left_up) };
	ImGui::DockBuilderDockWindow(m_editor_view.get_name(), right);
	ImGui::DockBuilderDockWindow(m_hierarchy.get_name(), left_up);
	ImGui::DockBuilderDockWindow(m_editor_log.get_name(), left_down);
}

void EditorNode::_draw_menu_bar()
{
	if (ImGui::BeginMenu("File")) {
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Alt+F4")) { get_tree()->get_root()->set_should_close(true); }
		ImGui::EndMenu();
	}
	
	if (ImGui::BeginMenu("View")) {
		if (ImGui::MenuItem("Dear ImGui Demo")) { m_show_imgui_demo = !m_show_imgui_demo; }
		if (ImGui::MenuItem(m_hierarchy.get_name(), "", m_hierarchy.is_open())) { m_hierarchy.toggle_open(); }
		if (ImGui::MenuItem(m_editor_log.get_name(), "", m_editor_log.is_open())) { m_editor_log.toggle_open(); }
		if (ImGui::MenuItem(m_editor_view.get_name(), "", m_editor_view.is_open())) { m_editor_view.toggle_open(); }
		ImGui::EndMenu();
	}
}
