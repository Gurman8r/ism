#include <editor/editor_node.hpp>
#include <scene/gui/imgui.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

enum {
	SCENE,
	RENDER_PASS,
	TRANSFORMS,
	MATERIAL,
	MAX_UNIFORMS
};

static RID uniform_buffers[MAX_UNIFORMS]{};
static RID uniform_sets[MAX_UNIFORMS]{};
static RID pipeline{};
static RID framebuffer{}, backbuffer{};
static RID render_target{};
static RID viewport{};

static void _setup_pipeline(RID const shader)
{
	pipeline = RENDERING_DEVICE->pipeline_create(
		shader,
		RD::RenderPrimitive_Triangles,
		COMPOSE(RD::RasterizationState, r) {
			r.enable_depth_clamp = false;
			r.discard_primitives = false;
			r.enable_wireframe = false;
			r.cull_mode = RD::PolygonCullMode_Front;
			r.front_face = RD::PolygonFrontFace_Clockwise;
			r.enable_depth_bias = false;
			r.depth_bias_constant_factor = 0.f;
			r.depth_bias_clamp = 0.f;
			r.depth_bias_slope_factor = 0.f;
			r.line_width = 1.f;
			r.patch_control_points = 1;
		},
		COMPOSE(RD::MultisampleState, m) {
			m.sample_count = RD::TextureSamples_1;
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
			d.front_op.fail = d.back_op.fail = RD::StencilOperation_Zero;
			d.front_op.pass = d.back_op.pass = RD::StencilOperation_Zero;
			d.front_op.depth_fail = d.back_op.depth_fail = RD::StencilOperation_Zero;
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
				RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
				RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
				true, true, true, true });
		});
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
	ASSERT(!singleton); singleton = this;

	subscribe<WindowKeyEvent, WindowMouseButtonEvent, WindowMousePositionEvent, WindowScrollEvent>();
	
	//m_textures["earth_cm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_cm_2k.png");
	m_textures["earth_dm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_dm_2k.png");
	//m_textures["earth_hm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_hm_2k.png");
	//m_textures["earth_lm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_lm_2k.png");
	//m_textures["earth_nm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_nm_2k.png");
	//m_textures["earth_sm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_sm_2k.png");
	m_meshes["sphere32x24"].instance("../../../assets/meshes/sphere32x24.obj");
	m_shaders["3d"].instance("../../../assets/shaders/3d.json");

	RID const shader{ m_shaders["3d"]->get_rid() };
	_setup_pipeline(shader);

	uniform_buffers[SCENE] = RENDERING_DEVICE->uniform_buffer_create(sizeof(RD::ConstantBuffer<Mat4, Mat4>));
	uniform_sets[SCENE] = RENDERING_DEVICE->uniform_set_create({
		{ RD::UniformType_UniformBuffer, SCENE, { uniform_buffers[SCENE] } },
	}, shader);

	uniform_buffers[RENDER_PASS] = nullptr;
	uniform_sets[RENDER_PASS] = RENDERING_DEVICE->uniform_set_create({
		//{ RD::UniformType_UniformBuffer, RENDER_PASS, { uniform_buffers[RENDER_PASS] } },
	}, shader);

	uniform_buffers[TRANSFORMS] = RENDERING_DEVICE->uniform_buffer_create(sizeof(RD::ConstantBuffer<Mat4>));
	uniform_sets[TRANSFORMS] = RENDERING_DEVICE->uniform_set_create({
		{ RD::UniformType_UniformBuffer, TRANSFORMS, { uniform_buffers[TRANSFORMS] } },
	}, shader);

	uniform_buffers[MATERIAL] = RENDERING_DEVICE->uniform_buffer_create(sizeof(RD::ConstantBuffer<Vec4, Vec4, Vec4, float_t>));
	uniform_sets[MATERIAL] = RENDERING_DEVICE->uniform_set_create({
		//{ RD::UniformType_UniformBuffer, MATERIAL, { uniform_buffers[MATERIAL] } },
		{ RD::UniformType_Texture, 0, { m_textures["earth_dm_2k"]->get_rid() } },
	}, shader);

	Vector<RID> fb_textures{
		RENDERING_DEVICE->texture_create({ RD::TextureType_2D, RD::DataFormat_R8G8B8_UNORM, 1280, 720 }),
		RENDERING_DEVICE->texture_create({ RD::TextureType_2D, RD::DataFormat_D24_UNORM_S8_UINT, 1280, 720 }) };
	framebuffer = RENDERING_DEVICE->framebuffer_create(fb_textures);
	m_editor_view.set_main_texture(fb_textures[0]);
}

EditorNode::~EditorNode()
{
	ASSERT(this == singleton); SCOPE_EXIT(&) { singleton = nullptr; };

	for (size_t i = 0; i < MAX_UNIFORMS; ++i) {
		if (uniform_buffers[i]) { RENDERING_DEVICE->buffer_destroy(uniform_buffers[i]); }
		if (uniform_sets[i]) { RENDERING_DEVICE->uniform_set_destroy(uniform_sets[i]); }
	}
	if (framebuffer) { RENDERING_DEVICE->framebuffer_destroy(framebuffer); }
	if (backbuffer) { RENDERING_DEVICE->framebuffer_destroy(backbuffer); }
	if (pipeline) { RENDERING_DEVICE->pipeline_destroy(pipeline); }
}

void EditorNode::handle_event(Event const & event)
{
	switch (event)
	{
	case WindowKeyEvent::ID: {
		WindowKeyEvent const & ev{ (WindowKeyEvent const &)event };
	} break;
	case WindowMouseButtonEvent::ID: {
		WindowMouseButtonEvent const & ev{ (WindowMouseButtonEvent const &)event };
	} break;
	case WindowMousePositionEvent::ID: {
		WindowMousePositionEvent const & ev{ (WindowMousePositionEvent const &)event };
	} break;
	case WindowScrollEvent::ID: {
		WindowScrollEvent const & ev{ (WindowScrollEvent const &)event };
	} break;
	}
}

void EditorNode::process(Duration const dt)
{
	char window_title[32]{};
	std::sprintf(window_title, "ism @ %.1f fps", (float_t)get_tree()->get_fps());
	get_tree()->get_root()->set_title(window_title);

	static EditorCamera * editor_camera{ m_editor_view.get_editor_camera() };
	static Vec2 view_size{ 1280, 720 }, view_size_prev{};
	if (m_editor_view.get_window()) { view_size = m_editor_view->InnerRect.GetSize(); }
	bool const viewport_resized{ view_size != view_size_prev };
	view_size_prev = view_size;

	if (viewport_resized) {
		editor_camera->set_res(view_size);
		RENDERING_DEVICE->framebuffer_set_size(framebuffer, (int32_t)view_size[0], (int32_t)view_size[1]);
	}
	editor_camera->recalculate();

	if constexpr (1)
	{
		Mat4 const cam_projection{ editor_camera->get_proj() }, cam_transform{ editor_camera->get_view() };

		static RD::ConstantBuffer<Mat4, Mat4> scene_ubo_data;
		scene_ubo_data.set<0>(cam_projection); // projection matrix
		scene_ubo_data.set<1>(cam_transform); // view matrix
		RENDERING_DEVICE->buffer_update(uniform_buffers[SCENE], 0, scene_ubo_data, sizeof(scene_ubo_data));

		static RD::ConstantBuffer<Mat4> transforms_ubo_data;
		transforms_ubo_data.set<0>(object_matrix[0]); // model matrix
		RENDERING_DEVICE->buffer_update(uniform_buffers[TRANSFORMS], 0, transforms_ubo_data, sizeof(transforms_ubo_data));

		static RD::ConstantBuffer<Vec4, Vec4, Vec4, float_t> material_ubo_data;
		material_ubo_data.set<0>({ 1, 1, 1, 1 }); // ambient
		material_ubo_data.set<1>({ 0.5f, 0.5f, 0.5f, 1.0f }); // diffuse
		material_ubo_data.set<2>({ 1.0f, 1.0f, 1.0f, 1.0f }); // specular
		material_ubo_data.set<3>(32.f); // shininess
		RENDERING_DEVICE->buffer_update(uniform_buffers[MATERIAL], 0, material_ubo_data, sizeof(material_ubo_data));

		static Color clear_color{ Colors::magenta };
		clear_color = rotate_hue(clear_color, (float_t)dt * 10.f);

		RID dl{ RENDERING_DEVICE->draw_list_begin(framebuffer, clear_color) };
		RENDERING_DEVICE->draw_list_bind_pipeline(dl, pipeline);
		RENDERING_DEVICE->draw_list_bind_uniform_set(dl, uniform_sets[SCENE], SCENE);
		RENDERING_DEVICE->draw_list_bind_uniform_set(dl, uniform_sets[RENDER_PASS], RENDER_PASS);
		RENDERING_DEVICE->draw_list_bind_uniform_set(dl, uniform_sets[TRANSFORMS], TRANSFORMS);
		RENDERING_DEVICE->draw_list_bind_uniform_set(dl, uniform_sets[MATERIAL], MATERIAL);
		
		static Mesh * mesh{ *m_meshes["sphere32x24"] };
		mesh->each([&](RID vertex_array, RID index_array, Vector<Ref<Texture>> & textures) {
			RENDERING_DEVICE->draw_list_bind_vertex_array(dl, vertex_array);
			RENDERING_DEVICE->draw_list_bind_index_array(dl, index_array);
			RENDERING_DEVICE->draw_list_draw(dl, (bool)index_array, 1, 0);
		});
		
		RENDERING_DEVICE->draw_list_end();
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
