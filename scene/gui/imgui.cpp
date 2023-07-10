#include <scene/gui/imgui.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if SYSTEM_WINDOWS
#include <glfw/glfw3.h>
#include <imgui/backends/imgui_impl_glfw.h>
#define IMGUI_PLATFORM_INIT() ImGui_ImplGlfw_InitForOpenGL(glfwGetCurrentContext(), true)
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

namespace Ism
{
	ImGuiContext * ImGui_Initialize()
	{
		ImGuiContext * ctx{ ImGui::CreateContext() };
		ctx->IO.LogFilename = nullptr;
		ctx->IO.IniFilename = nullptr;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ctx->IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ASSERT(IMGUI_PLATFORM_INIT());
		ASSERT(IMGUI_RENDERER_INIT());

		ImGuiStyle & style{ ImGui::GetStyle() };
		style.Colors[ImGuiCol_Text					] = ImVec4(1.00f, 1.00f, 1.00f, 0.95f);
		style.Colors[ImGuiCol_TextDisabled			] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		style.Colors[ImGuiCol_WindowBg				] = ImVec4(0.05f, 0.05f, 0.05f, 1.00f);
		style.Colors[ImGuiCol_ChildBg				] = ImVec4(1.00f, 1.00f, 1.00f, 0.00f);
		style.Colors[ImGuiCol_PopupBg				] = ImVec4(0.05f, 0.05f, 0.05f, 0.94f);
		style.Colors[ImGuiCol_Border				] = ImVec4(0.53f, 0.53f, 0.53f, 0.46f);
		style.Colors[ImGuiCol_BorderShadow			] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style.Colors[ImGuiCol_FrameBg				] = ImVec4(0.00f, 0.00f, 0.00f, 0.85f);
		style.Colors[ImGuiCol_FrameBgHovered		] = ImVec4(0.22f, 0.22f, 0.22f, 0.40f);
		style.Colors[ImGuiCol_FrameBgActive			] = ImVec4(0.16f, 0.16f, 0.16f, 0.53f);
		style.Colors[ImGuiCol_TitleBg				] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TitleBgActive			] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		style.Colors[ImGuiCol_TitleBgCollapsed		] = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		style.Colors[ImGuiCol_MenuBarBg				] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarBg			] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		style.Colors[ImGuiCol_ScrollbarGrab			] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered	] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
		style.Colors[ImGuiCol_ScrollbarGrabActive	] = ImVec4(0.48f, 0.48f, 0.48f, 1.00f);
		style.Colors[ImGuiCol_CheckMark				] = ImVec4(0.79f, 0.79f, 0.79f, 1.00f);
		style.Colors[ImGuiCol_SliderGrab			] = ImVec4(0.48f, 0.47f, 0.47f, 0.91f);
		style.Colors[ImGuiCol_SliderGrabActive		] = ImVec4(0.56f, 0.55f, 0.55f, 0.62f);
		style.Colors[ImGuiCol_Button				] = ImVec4(0.50f, 0.50f, 0.50f, 0.63f);
		style.Colors[ImGuiCol_ButtonHovered			] = ImVec4(0.67f, 0.67f, 0.68f, 0.63f);
		style.Colors[ImGuiCol_ButtonActive			] = ImVec4(0.26f, 0.26f, 0.26f, 0.63f);
		style.Colors[ImGuiCol_Header				] = ImVec4(0.25f, 0.25f, 0.25f, 0.58f);
		style.Colors[ImGuiCol_HeaderHovered			] = ImVec4(0.64f, 0.65f, 0.65f, 0.80f);
		style.Colors[ImGuiCol_HeaderActive			] = ImVec4(0.25f, 0.25f, 0.25f, 0.80f);
		style.Colors[ImGuiCol_Separator				] = ImVec4(0.58f, 0.58f, 0.58f, 0.50f);
		style.Colors[ImGuiCol_SeparatorHovered		] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
		style.Colors[ImGuiCol_SeparatorActive		] = ImVec4(0.81f, 0.81f, 0.81f, 0.64f);
		style.Colors[ImGuiCol_ResizeGrip			] = ImVec4(0.87f, 0.87f, 0.87f, 0.53f);
		style.Colors[ImGuiCol_ResizeGripHovered		] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
		style.Colors[ImGuiCol_ResizeGripActive		] = ImVec4(0.87f, 0.87f, 0.87f, 0.74f);
		style.Colors[ImGuiCol_Tab					] = ImVec4(0.01f, 0.01f, 0.01f, 0.86f);
		style.Colors[ImGuiCol_TabHovered			] = ImVec4(0.29f, 0.29f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_TabActive				] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocused			] = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
		style.Colors[ImGuiCol_TabUnfocusedActive	] = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
		style.Colors[ImGuiCol_DockingPreview		] = ImVec4(0.38f, 0.48f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_DockingEmptyBg		] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
		style.Colors[ImGuiCol_PlotLines				] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		style.Colors[ImGuiCol_PlotLinesHovered		] = ImVec4(0.68f, 0.68f, 0.68f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogram			] = ImVec4(0.90f, 0.77f, 0.33f, 1.00f);
		style.Colors[ImGuiCol_PlotHistogramHovered	] = ImVec4(0.87f, 0.55f, 0.08f, 1.00f);
		style.Colors[ImGuiCol_TextSelectedBg		] = ImVec4(0.47f, 0.60f, 0.76f, 0.47f);
		style.Colors[ImGuiCol_DragDropTarget		] = ImVec4(0.58f, 0.58f, 0.58f, 0.90f);
		style.Colors[ImGuiCol_NavHighlight			] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		style.Colors[ImGuiCol_NavWindowingHighlight	] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		style.Colors[ImGuiCol_NavWindowingDimBg		] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		style.Colors[ImGuiCol_ModalWindowDimBg		] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

		return ctx;
	}

	void ImGui_Finalize(ImGuiContext * ctx)
	{
		IMGUI_RENDERER_SHUTDOWN();
		IMGUI_PLATFORM_SHUTDOWN();
		if (ctx) { ImGui::DestroyContext(ctx); }
	}

	void ImGui_BeginFrame(ImGuiContext * ctx)
	{
		IMGUI_RENDERER_NEWFRAME();
		IMGUI_PLATFORM_NEWFRAME();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGui_RenderDrawData(ImDrawData * draw_data)
	{
		IMGUI_RENDER_DRAW_DATA(draw_data);
	}

	void ImGui_EndFrame(ImGuiContext * ctx)
	{
		if (ctx->IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */