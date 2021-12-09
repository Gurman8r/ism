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

	images["sanic"].instance();
	ImageLoader::load_image(images["sanic"], "../../../assets/textures/Sanic.png");
	textures["sanic"].instance<ImageTexture>(images["sanic"]);

	meshes["sphere8x6"].instance("../../../assets/meshes/sphere8x6.obj");

	//shaders["basic_2d"].instance("../../../assets/shaders/basic_2d.json");
	shaders["basic_3d"].instance("../../../assets/shaders/basic_3d.json");

	static RenderingDevice * dev{ SINGLETON(RenderingDevice) };
	static RID shader{ shaders["basic_3d"]->get_rid() };
	static RID texture{ textures["sanic"]->get_rid() };
	dev->shader_bind(shader);
	dev->shader_set_uniform1f(shader, "Time", 1.f);
	dev->shader_set_uniform1f(shader, "DeltaTime", 16_ms);
	dev->shader_set_uniform3f(shader, "Transform.Position", { 0.f, 0.f, 0.f });
	dev->shader_set_uniform3f(shader, "Transform.Scale", { 1.f, 1.f, 1.f });
	dev->shader_set_uniform4f(shader, "Transform.Rotation", { 0.f, 0.1f, 0.f, 0.25f });
	dev->shader_set_uniform3f(shader, "Camera.Position", { 0.f, 0.f, 3.f });
	dev->shader_set_uniform3f(shader, "Camera.Direction", { 0.f, 0.f, -1.f });
	dev->shader_set_uniform1f(shader, "Camera.Fov", 45.f);
	dev->shader_set_uniform1f(shader, "Camera.Near", 0.001f);
	dev->shader_set_uniform1f(shader, "Camera.Far", 1000.f);
	dev->shader_set_uniform2f(shader, "Camera.View", { 1280, 720 });
	dev->shader_set_uniform4f(shader, "Tint", (Vec4f)Colors::red);
	dev->texture_bind(textures["sanic"]->get_rid(), 0);
	dev->shader_set_uniform1i(shader, "Texture0", 0);
	dev->shader_bind(0);
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

	_show_dockspace("##EditorDockspace");
	
	//ImGui::ShowDemoWindow();

	static RenderingDevice * dev{ SINGLETON(RenderingDevice) };
	static RID shader{ shaders["basic_3d"]->get_rid() };
	static RID vao{ meshes["sphere8x6"]->get_rid() };
	dev->framebuffer_bind(framebuffer);
	dev->clear(Colors::magenta);
	dev->shader_bind(shader);
	dev->vertexarray_draw(vao);
	dev->shader_bind(0);
	dev->framebuffer_bind(0);

	_show_viewport("Viewport");

	Node::process(dt);
}

void EditorNode::_show_dockspace(cstring label, bool has_main_menu_bar)
{
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable) {
		ImGuiID const dockspace_id{ ImGui::GetID(label) };
		ImGuiViewportP * dockspace_vp{ ImGui::GetCurrentContext()->Viewports[0] };
		ImGui::SetNextWindowPos(dockspace_vp->Pos);
		ImGui::SetNextWindowSize(dockspace_vp->Size);
		ImGui::SetNextWindowViewport(dockspace_vp->ID);
		ImGui::SetNextWindowBgAlpha(0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.f);
		ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0.f, 0.f });
		bool const dockspace_open{ ImGui::Begin(label, nullptr,
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

void EditorNode::_show_viewport(cstring label)
{
	ImGuiViewport * main_viewport{ ImGui::GetMainViewport() };
	ImGui::SetNextWindowPos(ImVec2(main_viewport->GetWorkPos().x + 32, main_viewport->GetWorkPos().y + 32), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);
	ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, { 0, 0 });
	bool const viewport_is_open{ ImGui::Begin(label, 0, ImGuiWindowFlags_NoScrollbar) };
	ImGui::PopStyleVar(1);
	if (viewport_is_open)
	{
		RID tex{ ((RD_Texture *)((RD_Framebuffer *)framebuffer)->texture_attachments[0])->handle };

		ImGuiWindow * current_window{ ImGui::GetCurrentContext()->CurrentWindow };
		if (ImGui::ItemAdd(current_window->InnerRect, NULL)) {
			current_window->DrawList->AddImage(
				tex,
				current_window->InnerRect.Min,
				current_window->InnerRect.Max,
				{ 0, 1 }, { 1, 0 }, 0xffffffff);
		}
	}
	ImGui::End();
}
