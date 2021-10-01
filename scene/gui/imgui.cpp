#include <scene/gui/imgui.hpp>

#ifdef SYSTEM_WINDOWS
#include <imgui/backends/imgui_impl_glfw.h>
#define IMGUI_PLATFORM_INIT(window, install_callbacks) ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)window, install_callbacks)
#define IMGUI_PLATFORM_SHUTDOWN() ImGui_ImplGlfw_Shutdown()
#define IMGUI_PLATFORM_NEWFRAME() ImGui_ImplGlfw_NewFrame()
#endif

#ifdef OPENGL_ENABLED
#include <imgui/backends/imgui_impl_opengl3.h>
#define IMGUI_RENDERER_INIT() ImGui_ImplOpenGL3_Init("#version 130")
#define IMGUI_RENDERER_SHUTDOWN() ImGui_ImplOpenGL3_Shutdown()
#define IMGUI_RENDERER_NEWFRAME() ImGui_ImplOpenGL3_NewFrame()
#define IMGUI_RENDER_DRAW_DATA(draw_data) ImGui_ImplOpenGL3_RenderDrawData(draw_data)
#endif

using namespace ism;

bool ism::ImGui_Init(WindowID window, bool install_callbacks)
{
	return IMGUI_PLATFORM_INIT(window, install_callbacks) && IMGUI_RENDERER_INIT();
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
}

void ism::ImGui_RenderDrawData(ImDrawData * draw_data)
{
	IMGUI_RENDER_DRAW_DATA(draw_data);
}