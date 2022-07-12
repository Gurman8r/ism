#include <editor/editor_node.hpp>
#include <scene/imgui.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	enum {
		SCENE_STATE_UNIFORMS,
		RENDER_PASS_UNIFORMS,
		TRANSFORMS_UNIFORMS,
		MATERIAL_UNIFORMS,
		MAX_UNIFORMS
	};

	static RID uniform_buffers[MAX_UNIFORMS]{};
	static RID uniform_sets[MAX_UNIFORMS]{};
	static RID material{};
	static RID pipeline{};
	static RID framebuffer{}, backbuffer{};
	static RID render_target{};
	static RID viewport{};

	static Mat4 object_matrix[] = // 
	{
		Mat4::identity(),
	};

	static void _setup_pipeline(RID const shader)
	{
		pipeline = RD::get_singleton()->render_pipeline_create
		(
			shader,
			RD::RenderPrimitive_Triangles,
			MAKE(RD::RasterizationState, r)
			{
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
			MAKE(RD::MultisampleState, m)
			{
				m.sample_count = RD::TextureSamples_1;
				m.enable_sample_shading = false;
				m.sample_mask = {};
				m.enable_alpha_to_coverage = false;
				m.enable_alpha_to_one = false;
			},
			MAKE(RD::DepthStencilState, d)
			{
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
			MAKE(RD::ColorBlendState, c)
			{
				c.enable_logic_op = true;
				c.logic_op = LogicOperation_Clear;
				c.blend_constant = Colors::clear;
				c.attachments.push_back({ true,
					RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
					RD::BlendFactor_SrcAlpha, RD::BlendFactor_OneMinusSrcAlpha, RD::BlendOperation_Add,
					true, true, true, true });
			}
		);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorNode * EditorNode::__singleton{};

	OBJECT_EMBED(EditorNode, t) {}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	EditorNode::EditorNode()
	{
		ASSERT(!__singleton); __singleton = this;

		m_textures["earth_dm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_dm_2k.png");
		m_textures["earth_sm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_sm_2k.png");
		m_meshes["sphere32x24"].instance("../../../assets/meshes/sphere32x24.obj");
		m_shaders["3D"].instance("../../../assets/shaders/3d.json");

		RID const shader{ m_shaders["3D"]->get_rid() };
		_setup_pipeline(shader);

		uniform_buffers[SCENE_STATE_UNIFORMS] = RD::get_singleton()->uniform_buffer_create(sizeof(RD::Std140<Mat4, Mat4>));
		uniform_sets[SCENE_STATE_UNIFORMS] = RD::get_singleton()->uniform_set_create({
			{ RD::UniformType_UniformBuffer, SCENE_STATE_UNIFORMS, { uniform_buffers[SCENE_STATE_UNIFORMS] } },
		}, shader);

		uniform_buffers[RENDER_PASS_UNIFORMS] = RD::get_singleton()->uniform_buffer_create(sizeof(RD::Std140<Mat4>));
		uniform_sets[RENDER_PASS_UNIFORMS] = RD::get_singleton()->uniform_set_create({
			{ RD::UniformType_UniformBuffer, RENDER_PASS_UNIFORMS, { uniform_buffers[RENDER_PASS_UNIFORMS] } },
		}, shader);

		uniform_buffers[TRANSFORMS_UNIFORMS] = RD::get_singleton()->uniform_buffer_create(sizeof(RD::Std140<Mat4>));
		uniform_sets[TRANSFORMS_UNIFORMS] = RD::get_singleton()->uniform_set_create({
			{ RD::UniformType_UniformBuffer, TRANSFORMS_UNIFORMS, { uniform_buffers[TRANSFORMS_UNIFORMS] } },
		}, shader);

		uniform_buffers[MATERIAL_UNIFORMS] = RD::get_singleton()->uniform_buffer_create(sizeof(RD::Std140<Vec4, Vec4, Vec4, f32>));
		uniform_sets[MATERIAL_UNIFORMS] = RD::get_singleton()->uniform_set_create({
			{ RD::UniformType_UniformBuffer, MATERIAL_UNIFORMS, { uniform_buffers[MATERIAL_UNIFORMS] } },
			{ RD::UniformType_Texture, 0, { m_textures["earth_dm_2k"]->get_rid() } },
			{ RD::UniformType_Texture, 1, { m_textures["earth_sm_2k"]->get_rid() } },
		}, shader);
	
		// material (WIP)
		material = RS::get_singleton()->material_create();
		RS::get_singleton()->material_set_shader(material, shader);
		RS::get_singleton()->material_update(material, {
			{ "Ambient", Vec4{ 0.8f, 0.4f, 0.2f, 1.0f } },
			{ "Diffuse", Vec4{ 0.5f, 0.5f, 0.5f, 1.0f } },
			{ "Specular", Vec4{ 1.0f, 1.0f, 1.0f, 1.0f } },
			{ "Shininess", 32.f },
			{ "Diffuse_Map", m_textures["earth_dm_2k"]->get_rid() },
			{ "Specular_Map", m_textures["earth_sm_2k"]->get_rid() },
		});
	
		// framebuffers
		Vector<RID> fb_textures{
			RD::get_singleton()->texture_create(MAKE(RD::TextureCreateInfo, t) {
				t.color_format = RD::DataFormat_R8G8B8_UNORM;
				t.usage_flags = RD::TextureFlags_Sampling | RD::TextureFlags_CanCopyFrom | RD::TextureFlags_ColorAttachment;
			}),
			RD::get_singleton()->texture_create(MAKE(RD::TextureCreateInfo, t) {
				t.color_format = RD::DataFormat_D24_UNORM_S8_UINT;
				t.usage_flags = RD::TextureFlags_Sampling | RD::TextureFlags_CanCopyFrom | RD::TextureFlags_DepthStencilAttachment;
			}),
		};
		framebuffer = RD::get_singleton()->framebuffer_create(fb_textures);
		m_viewport.set_main_texture(fb_textures[0]);
	}

	EditorNode::~EditorNode()
	{
		ASSERT(this == __singleton); ON_SCOPE_EXIT(&) { __singleton = nullptr; };

		if (material) { RS::get_singleton()->material_destroy(material); }

		for (size_t i = 0; i < ARRAY_SIZE(uniform_buffers); ++i) {
			if (uniform_buffers[i]) { RD::get_singleton()->buffer_destroy(uniform_buffers[i]); }
		}

		for (size_t i = 0; i < ARRAY_SIZE(uniform_sets); ++i) {
			if (uniform_sets[i]) { RD::get_singleton()->uniform_set_destroy(uniform_sets[i]); }
		}

		if (framebuffer) { RD::get_singleton()->framebuffer_destroy(framebuffer); }
		if (backbuffer) { RD::get_singleton()->framebuffer_destroy(backbuffer); }
		if (pipeline) { RD::get_singleton()->render_pipeline_destroy(pipeline); }
	}

	void EditorNode::_notification(i32 notification_id)
	{
		switch (notification_id)
		{
		case Notification_Process: {

			Duration const delta_time{ get_tree()->get_delta_time() };

			char window_title[32];
			std::sprintf(window_title, "ism @ %.3f fps", get_tree()->get_fps().value);
			get_tree()->get_root()->set_title(window_title);

			static EditorCamera * editor_camera{ m_viewport.get_editor_camera() };
			static Vec2i view_size{ 1280, 720 }, view_size_prev{};
			if (m_viewport.get_window()) { view_size = { (i32)m_viewport->InnerRect.GetWidth(), (i32)m_viewport->InnerRect.GetHeight() }; }
			if (view_size_prev != view_size) {
				view_size_prev = view_size;
				editor_camera->set_res(view_size);
				RD::get_singleton()->framebuffer_set_size(framebuffer, view_size[0], view_size[1]);
			}
			editor_camera->recalculate();

			{
				RD::Std140<Mat4, Mat4> scene_state_ubo;
				scene_state_ubo.set<0>(editor_camera->get_proj()); // camera projection
				scene_state_ubo.set<1>(editor_camera->get_view()); // camera view
				RD::get_singleton()->buffer_update(uniform_buffers[SCENE_STATE_UNIFORMS], 0, scene_state_ubo, sizeof(scene_state_ubo));

				RD::Std140<Mat4> render_pass_ubo;
				render_pass_ubo.set<0>(Mat4::identity()); // placeholder
				RD::get_singleton()->buffer_update(uniform_buffers[RENDER_PASS_UNIFORMS], 0, render_pass_ubo, sizeof(render_pass_ubo));

				RD::Std140<Mat4> transforms_ubo;
				transforms_ubo.set<0>(object_matrix[0]); // model transform
				RD::get_singleton()->buffer_update(uniform_buffers[TRANSFORMS_UNIFORMS], 0, transforms_ubo, sizeof(transforms_ubo));

				RD::Std140<Vec4, Vec4, Vec4, f32> material_ubo;
				material_ubo.set<0>({ 0.8f, 0.4f, 0.2f, 1.0f }); // ambient
				material_ubo.set<1>({ 0.5f, 0.5f, 0.5f, 1.0f }); // diffuse
				material_ubo.set<2>({ 1.0f, 1.0f, 1.0f, 1.0f }); // specular
				material_ubo.set<3>(32.f); // shininess
				RD::get_singleton()->buffer_update(uniform_buffers[MATERIAL_UNIFORMS], 0, material_ubo, sizeof(material_ubo));

				static Vector<Color> clear_colors{ Colors::magenta };
				clear_colors[0] = rotate_hue(clear_colors[0], 10.f * delta_time);

				RD::DrawListID const draw_list{ RD::get_singleton()->draw_list_begin(framebuffer, RD::InitialAction_Clear, RD::FinalAction_Read, RD::InitialAction_Keep, RD::FinalAction_Discard, clear_colors) };
				RD::get_singleton()->draw_list_bind_pipeline(draw_list, pipeline);
				RD::get_singleton()->draw_list_bind_uniform_set(draw_list, uniform_sets[SCENE_STATE_UNIFORMS], SCENE_STATE_UNIFORMS);
				RD::get_singleton()->draw_list_bind_uniform_set(draw_list, uniform_sets[RENDER_PASS_UNIFORMS], RENDER_PASS_UNIFORMS);
				RD::get_singleton()->draw_list_bind_uniform_set(draw_list, uniform_sets[TRANSFORMS_UNIFORMS], TRANSFORMS_UNIFORMS);
				RD::get_singleton()->draw_list_bind_uniform_set(draw_list, uniform_sets[MATERIAL_UNIFORMS], MATERIAL_UNIFORMS);

				static Mesh * mesh{ *m_meshes["sphere32x24"] };
				for (size_t i = 0; i < mesh->get_surface_count(); ++i) {
					RD::get_singleton()->draw_list_bind_vertex_array(draw_list, mesh->surface_get_vertex_array(i));
					RD::get_singleton()->draw_list_bind_index_array(draw_list, mesh->surface_get_index_array(i));
					RD::get_singleton()->draw_list_draw(draw_list, true, 1, 0);
				}

				RD::get_singleton()->draw_list_end();
			}

			_draw_dockspace();
			if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }
			m_hierarchy.process(delta_time);
			m_log.process(delta_time);
			m_viewport.process(delta_time);

		} break;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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
		ImGui::DockBuilderDockWindow(m_viewport.get_name(), right);
		ImGui::DockBuilderDockWindow(m_hierarchy.get_name(), left_up);
		ImGui::DockBuilderDockWindow(m_log.get_name(), left_down);
	}

	void EditorNode::_draw_menu_bar()
	{
		if (ImGui::BeginMenu("File")) {
			//ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4")) { get_tree()->quit(); }
			ImGui::EndMenu();
		}
	
		if (ImGui::BeginMenu("View")) {
			if (ImGui::MenuItem("Dear ImGui Demo")) { m_show_imgui_demo = !m_show_imgui_demo; }
			if (ImGui::MenuItem(m_hierarchy.get_name(), "", m_hierarchy.is_open())) { m_hierarchy.toggle_open(); }
			if (ImGui::MenuItem(m_log.get_name(), "", m_log.is_open())) { m_log.toggle_open(); }
			if (ImGui::MenuItem(m_viewport.get_name(), "", m_viewport.is_open())) { m_viewport.toggle_open(); }
			ImGui::EndMenu();
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}