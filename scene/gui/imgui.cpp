#include <scene/gui/imgui.hpp>

#ifdef SYSTEM_WINDOWS
#include <imgui/backends/imgui_impl_glfw.h>
#endif

#ifdef OPENGL_ENABLED
#include <imgui/backends/imgui_impl_opengl3.h>
#endif

using namespace ism;

bool ism::ImGui_Init(WindowID window, bool install_callbacks)
{
#if defined(OPENGL_ENABLED) && defined(SYSTEM_WINDOWS)
	return ImGui_ImplGlfw_InitForOpenGL((GLFWwindow *)window, install_callbacks)
		&& ImGui_ImplOpenGL3_Init("#version 130");
#endif
}

void ism::ImGui_Shutdown()
{
#ifdef OPENGL_ENABLED
	ImGui_ImplOpenGL3_Shutdown();
#endif

#ifdef SYSTEM_WINDOWS
	ImGui_ImplGlfw_Shutdown();
#endif
}

void ism::ImGui_NewFrame()
{
#ifdef OPENGL_ENABLED
	ImGui_ImplOpenGL3_NewFrame();
#endif

#ifdef SYSTEM_WINDOWS
	ImGui_ImplGlfw_NewFrame();
#endif
}

void ism::ImGui_RenderDrawData(ImDrawData * draw_data)
{
#ifdef OPENGL_ENABLED
	ImGui_ImplOpenGL3_RenderDrawData(draw_data);
#endif
}