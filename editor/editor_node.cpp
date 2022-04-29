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

EMBED_OBJECT_CLASS(EditorNode, t) {}

EditorNode::EditorNode()
{
	singleton = this;

	framebuffer = RD->framebuffer_create
	({
		{
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
		},
		{
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
		}
	});

	m_viewport.set_main_texture(RD->framebuffer_attachment(framebuffer, 0));

	textures["sanic"].instance<ImageTexture>("../../../assets/textures/Sanic.png");
	textures["earth_dm_2k"].instance<ImageTexture>("../../../assets/textures/earth/earth_dm_2k.png");

	meshes["sphere8x6"].instance("../../../assets/meshes/sphere8x6.obj");
	meshes["sphere32x24"].instance("../../../assets/meshes/sphere32x24.obj");
	//meshes["teapot"].instance("../../../assets/meshes/teapot.obj");

	shaders["2d"].instance("../../../assets/shaders/2d.json");
	shaders["3d"].instance("../../../assets/shaders/3d.json");

	// camera
	m_camera.set_eye({ 0.f, 0.f, -5.f });
	m_camera.set_yaw(90.f);
	m_camera.set_res({ 1280, 720 });
	m_camera.recalculate();
}

EditorNode::~EditorNode()
{
	RD->framebuffer_destroy(framebuffer);
}

void EditorNode::process(Duration const & dt)
{
	char window_title[32]{};
	std::sprintf(window_title, "ism @ %.1f fps", (float_t)get_tree()->get_fps());
	get_tree()->get_root()->set_title(window_title);

	static Vec2 view_size{ 1280, 720 }, view_size_prev{};
	if (m_viewport.get_window()) { view_size = m_viewport->InnerRect.GetSize(); }
	m_camera.set_res(view_size);
	m_camera.recalculate();
	if (view_size_prev != view_size) {
		RD->framebuffer_resize(framebuffer, (int32_t)view_size[0], (int32_t)view_size[1]);
		view_size_prev = view_size;
	}

	RD->framebuffer_bind(framebuffer);
	RD->clear(Colors::magenta);
	{
		static Mesh * mesh{ *meshes["sphere32x24"] };
		static Shader * shader{ *shaders["3d"] };
		static Texture * texture{ *textures["earth_dm_2k"] };
		shader->bind();
		shader->set_uniform("Model", object_matrix[0]);
		shader->set_uniform("View", m_camera.get_view());
		shader->set_uniform("Proj", m_camera.get_projection());
		shader->set_uniform("Tint", Colors::white);
		shader->set_uniform("Texture0", texture->get_rid());
		mesh->draw();
	}
	RD->framebuffer_bind(0);

	draw_interface();

	Node::process(dt);
}

void EditorNode::draw_interface()
{
	// DOCKSPACE
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID id{ ImGui::GetID("##EditorDockspace") };
		ImGuiViewportP * const vp{ ImGui::GetCurrentContext()->Viewports[0] };
		ImGui::SetNextWindowPos(vp->Pos);
		ImGui::SetNextWindowSize(vp->Size);
		ImGui::SetNextWindowViewport(vp->ID);
		ImGui::SetNextWindowBgAlpha(0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f });
		bool const open{ ImGui::Begin("##EditorDockspace", nullptr,
			ImGuiWindowFlags_NoTitleBar |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBackground |
			(m_show_main_menu_bar ? ImGuiWindowFlags_MenuBar : ImGuiWindowFlags_None)) };
		ImGui::PopStyleVar(3);
		if (open)
		{
			if (!ImGui::DockBuilderGetNode(id))
			{
				ImGui::DockBuilderRemoveNode(id);
				ImGui::DockBuilderAddNode(id);
				{
					// TODO: build dockspace here
				}
				ImGui::DockBuilderFinish(id);
			}
			ImGui::DockSpace(id);
		}
		ImGui::End();
	}

	// MAIN MENU BAR
	if (m_show_main_menu_bar && ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::Separator();

			if (ImGui::MenuItem("Exit", "Alt+F4")) { get_tree()->get_root()->set_should_close(true); }

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Dear ImGui Demo")) { m_show_imgui_demo = !m_show_imgui_demo; }
			
			if (ImGui::MenuItem(m_hierarchy.get_name(), "", m_hierarchy.is_open())) { m_hierarchy.toggle_open(); }
			
			if (ImGui::MenuItem(m_log.get_name(), "", m_log.is_open())) { m_log.toggle_open(); }
			
			if (ImGui::MenuItem(m_viewport.get_name(), "", m_viewport.is_open())) { m_viewport.toggle_open(); }
			
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}

	// PANELS
	if (m_show_imgui_demo) { ImGui::ShowDemoWindow(&m_show_imgui_demo); }
	m_hierarchy.draw();
	m_log.draw();
	m_viewport.draw();
}
