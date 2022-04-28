#include <editor/editor_node.hpp>
#include <scene/gui/imgui.hpp>
#include <core/io/filesystem.hpp>

using namespace ism;

EMBED_CLASS(EditorNode, t) {}

EditorNode::EditorNode()
{
	color_buffer = SINGLETON(RenderingDevice)->texture_create({
		TextureType_2D,
		ColorFormat_R8G8B8_UNORM,
		1280, 720,
		1, 1, 0,
		SamplerFilter_Linear,
		SamplerFilter_Linear,
		SamplerRepeatMode_ClampToEdge,
		SamplerRepeatMode_ClampToEdge,
		TextureSamples_1,
		TextureFlags_Default | TextureFlags_ColorAttachment
	});
	depth_stencil_buffer = SINGLETON(RenderingDevice)->texture_create({
		TextureType_2D,
		ColorFormat_D24_UNORM_S8_UINT,
		1280, 720,
		1, 1, 0,
		SamplerFilter_Linear,
		SamplerFilter_Linear,
		SamplerRepeatMode_ClampToEdge,
		SamplerRepeatMode_ClampToEdge,
		TextureSamples_1,
		TextureFlags_Default | TextureFlags_DepthStencilAttachment
	});
	framebuffer = SINGLETON(RenderingDevice)->framebuffer_create({ color_buffer, depth_stencil_buffer });
	m_viewport.set_main_texture(((RD_Texture *)((RD_Framebuffer *)framebuffer)->texture_attachments[0])->handle);

	textures["sanic"].instance<ImageTexture>("../../../assets/textures/Sanic.png");
	textures["earth_dm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_dm_2k.png");

	meshes["sphere8x6"].instance("../../../assets/meshes/sphere8x6.obj");
	meshes["sphere32x24"].instance("../../../assets/meshes/sphere32x24.obj");
	//meshes["teapot"].instance("../../../assets/meshes/teapot.obj");

	shaders["basic_2d"].instance("../../../assets/shaders/basic_2d.json");
	shaders["basic_3d"].instance("../../../assets/shaders/basic_3d.json");

	Shader * shader{ *shaders["basic_3d"] };
	shader->bind();
	shader->set_uniform("Time", 1.f);
	shader->set_uniform("DeltaTime", 16_ms);
	shader->set_uniform("Transform.Position", Vec3{ 0.f, 0.f, 0.f });
	shader->set_uniform("Transform.Scale", Vec3{ 1.f, 1.f, 1.f });
	shader->set_uniform("Transform.Rotation", Vec4f{ 0.f, 0.1f, 0.f, 0.25f });
	shader->set_uniform("Camera.Position", Vec3{ 0.f, 0.f, 5.f });
	shader->set_uniform("Camera.Direction", Vec3{ 0.f, 0.f, -1.f });
	shader->set_uniform("Camera.Fov", 45.f);
	shader->set_uniform("Camera.Near", 0.001f);
	shader->set_uniform("Camera.Far", 1000.f);
	shader->set_uniform("Camera.View", Vec2{ 1280, 720 });
	shader->set_uniform("Tint", Colors::white);
	SINGLETON(RenderingDevice)->texture_bind(textures["earth_dm_2k"]->get_rid(), 0);
	shader->set_uniform("Texture0", 0);
}

EditorNode::~EditorNode()
{
	SINGLETON(RenderingDevice)->texture_destroy(color_buffer);
	SINGLETON(RenderingDevice)->texture_destroy(depth_stencil_buffer);
	SINGLETON(RenderingDevice)->framebuffer_destroy(framebuffer);
}

void EditorNode::process(Duration const & dt)
{
	char window_title[32]{};
	std::sprintf(window_title, "ism @ %.1f fps", (float_t)get_tree()->get_fps());
	get_tree()->get_root()->set_title(window_title);

	{
		static RenderingDevice * dev{ SINGLETON(RenderingDevice) };
		static Mesh * mesh{ *meshes["sphere32x24"] };
		static Shader * shader{ *shaders["basic_3d"] };
		
		static Vec2 view_size{ 1280, 720 };
		if (m_viewport.get_window()) { view_size = m_viewport->InnerRect.GetSize(); }
		dev->framebuffer_bind(framebuffer);
		dev->framebuffer_resize(framebuffer, (int32_t)view_size[0], (int32_t)view_size[1]);
		dev->clear(Colors::magenta);

		shader->bind();
		shader->set_uniform("Camera.View", view_size);
		shader->set_uniform("Texture0", textures["earth_dm_2k"]->get_rid());
		mesh->draw();

		dev->framebuffer_bind(0);
	}

	on_gui();

	Node::process(dt);
}

void EditorNode::on_gui()
{
	// dockspace
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID const dockspace_id{ ImGui::GetID("##EditorDockspace") };
		ImGuiViewportP * dockspace_vp{ ImGui::GetCurrentContext()->Viewports[0] };
		ImGui::SetNextWindowPos(dockspace_vp->Pos);
		ImGui::SetNextWindowSize(dockspace_vp->Size);
		ImGui::SetNextWindowViewport(dockspace_vp->ID);
		ImGui::SetNextWindowBgAlpha(0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f });
		bool const dockspace_open{ ImGui::Begin("##EditorDockspace", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBackground |
			(m_show_main_menu_bar ? ImGuiWindowFlags_MenuBar : ImGuiWindowFlags_None)
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

	// main menu bar
	if (m_show_main_menu_bar && ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "Alt+F4"))
			{
				get_tree()->get_root()->set_should_close(true);
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("ImGui Demo")) { m_show_imgui_demo = !m_show_imgui_demo; }
			if (ImGui::MenuItem(m_hierarchy.get_name(), "", m_hierarchy.is_open())) { m_hierarchy.toggle_open(); }
			if (ImGui::MenuItem(m_log.get_name(), "", m_log.is_open())) { m_log.toggle_open(); }
			if (ImGui::MenuItem(m_viewport.get_name(), "", m_viewport.is_open())) { m_viewport.toggle_open(); }
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	// panels
	if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }
	m_hierarchy.draw();
	m_log.draw();
	m_viewport.draw();
}
