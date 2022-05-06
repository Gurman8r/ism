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
	shaders["3d"].instance("../../../assets/shaders/3d.json");

	m_uniform_buffer = RENDERING_DEVICE->uniform_buffer_create(2 * sizeof(Mat4));
	m_uniform_set = RENDERING_DEVICE->uniform_set_create({
		RD::Uniform{ UniformType_UniformBuffer, 0, {m_uniform_buffer} },
	}, shaders["3d"]->get_rid());

	Vector<RID> fb_textures{
		RENDERING_DEVICE->texture_create({ TextureType_2D, ColorFormat_R8G8B8_UNORM, 1280, 720 }),
		RENDERING_DEVICE->texture_create({ TextureType_2D, ColorFormat_D24_UNORM_S8_UINT, 1280, 720 }) };
	m_framebuffer = RENDERING_DEVICE->framebuffer_create(fb_textures);
	m_editor_view.set_main_texture(fb_textures[0]);

	m_pipeline = RENDERING_DEVICE->pipeline_create(
		shaders["3d"]->get_rid(),
		RenderPrimitive_Triangles,
		COMPOSE(RD::RasterizationState, r) {
			r.enable_depth_clamp = false;
			r.discard_primitives = false;
			r.enable_wireframe = false;
			r.cull_mode = PolygonCullMode_Front;
			r.front_face = PolygonFrontFace_Clockwise;
			r.enable_depth_bias = false;
			r.depth_bias_constant_factor = 0.f;
			r.depth_bias_clamp = 0.f;
			r.depth_bias_slope_factor = 0.f;
			r.line_width = 1.f;
			r.patch_control_points = 1;
		},
		COMPOSE(RD::MultisampleState, m) {
			m.sample_count = TextureSamples_1;
			m.enable_sample_shading = false;
			m.sample_mask = {};
			m.enable_alpha_to_coverage = false;
			m.enable_alpha_to_one = false;
		},
		COMPOSE(RD::DepthStencilState, d) {
			d.enable_depth_test = true;
			d.enable_depth_write = true;
			d.depth_compare_operator = CompareOperator_Less;
			d.enable_depth_range = false;
			d.depth_range_min = 0.f;
			d.depth_range_max = 0.f;
			d.enable_stencil = false;
			d.front_op.fail = d.back_op.fail = StencilOperation_Zero;
			d.front_op.pass = d.back_op.pass = StencilOperation_Zero;
			d.front_op.depth_fail = d.back_op.depth_fail = StencilOperation_Zero;
			d.front_op.compare = d.back_op.compare = CompareOperator_Always;
			d.front_op.compare_mask = d.back_op.compare_mask = 0;
			d.front_op.write_mask = d.back_op.write_mask = 0;
			d.front_op.reference = d.back_op.reference = 0;
		},
		COMPOSE(RD::ColorBlendState, c) {
			c.enable_logic_op = true;
			c.logic_op = LogicOperation_Clear;
			c.blend_constant = Colors::clear;
			c.attachments.push_back({ true,
				BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add,
				BlendFactor_SrcAlpha, BlendFactor_OneMinusSrcAlpha, BlendOperation_Add,
				true, true, true, true });
		});
}

EditorNode::~EditorNode()
{
	RENDERING_DEVICE->uniform_set_destroy(m_uniform_set);
	RENDERING_DEVICE->uniform_buffer_destroy(m_uniform_buffer);
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
		RENDERING_DEVICE->uniform_buffer_update(m_uniform_buffer, 0, editor_camera->get_proj(), sizeof(Mat4));
		RENDERING_DEVICE->uniform_buffer_update(m_uniform_buffer, sizeof(Mat4), editor_camera->get_view(), sizeof(Mat4));

		static Color clear_color{ Colors::magenta };
		clear_color = rotate_hue(clear_color, (float_t)dt * 10.f);
		RID dl{ RENDERING_DEVICE->drawlist_begin(m_framebuffer, clear_color) };
		RENDERING_DEVICE->drawlist_bind_pipeline(dl, m_pipeline);
		RENDERING_DEVICE->drawlist_bind_uniform_set(dl, m_uniform_set, 0);

		static Shader * shader{ *shaders["3d"] };
		static Texture * texture{ *textures["earth_dm_2k"] };
		shader->set_uniform("u_Model", object_matrix[0]);
		shader->set_uniform("u_Texture0", texture->get_rid());

		static Mesh * mesh{ *meshes["sphere32x24"] };
		mesh->for_each([dl](RID vertex_array, RID index_array, auto & textures) {
			RENDERING_DEVICE->drawlist_bind_vertex_array(dl, vertex_array);
			RENDERING_DEVICE->drawlist_bind_index_array(dl, index_array);
			RENDERING_DEVICE->drawlist_draw(dl, true, 1, 0);
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
