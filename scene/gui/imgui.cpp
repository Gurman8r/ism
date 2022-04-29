#include <scene/gui/imgui.hpp>
#include <scene/main/scene_tree.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if SYSTEM_WINDOWS
#include <imgui/backends/imgui_impl_glfw.h>
#define IMGUI_PLATFORM_INIT(window, install_callbacks) ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)window, install_callbacks)
#define IMGUI_PLATFORM_SHUTDOWN() ImGui_ImplGlfw_Shutdown()
#define IMGUI_PLATFORM_NEWFRAME() ImGui_ImplGlfw_NewFrame()

#else
#error "imgui platform implementation undefined"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if OPENGL_ENABLED
#include <imgui/backends/imgui_impl_opengl3.h>
#define IMGUI_RENDERER_INIT() ImGui_ImplOpenGL3_Init("#version 130")
#define IMGUI_RENDERER_SHUTDOWN() ImGui_ImplOpenGL3_Shutdown()
#define IMGUI_RENDERER_NEWFRAME() ImGui_ImplOpenGL3_NewFrame()
#define IMGUI_RENDER_DRAW_DATA(draw_data) ImGui_ImplOpenGL3_RenderDrawData(draw_data)

#else
#error "imgui renderer implementation undefined"
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

using namespace ism;

bool ism::ImGui_Init(WindowID window, bool install_callbacks)
{
	if (!IMGUI_PLATFORM_INIT(window, install_callbacks)) { return false; }
	if (!IMGUI_RENDERER_INIT()) { return false; }
	return true;
}

void ism::ImGui_Shutdown()
{
	IMGUI_RENDERER_SHUTDOWN();
	IMGUI_PLATFORM_SHUTDOWN();
}

void ism::ImGui_NewFrame()
{
	IMGUI_RENDERER_NEWFRAME();
	IMGUI_PLATFORM_NEWFRAME();

	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
}

void ism::ImGui_RenderDrawData(ImDrawData * draw_data)
{
	IMGUI_RENDER_DRAW_DATA(draw_data);
}

void ism::ImGui_RenderFrame()
{
	ImGui::Render();

	RD->set_viewport(SceneTree::get_singleton()->get_root()->get_bounds());
	RD->clear();

	ImGui_RenderDrawData(&ImGui::GetCurrentContext()->Viewports[0]->DrawDataP);

	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		WindowID window{ DS->get_context_current() };
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		DS->make_context_current(window);
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */