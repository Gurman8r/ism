#include <main/main.hpp>
#include <scene/main/scene_tree.hpp>
#include <drivers/opengl/opengl.hpp>
#include <scene/gui/imgui.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MAIN_PRINT (ism::get_os().print)

#define MAIN_PAUSE (ism::get_os().pause)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	void hello() { MAIN_PRINT("Hello: %s\n", PRETTY_FUNCTION); }
	void say(String const & s) { MAIN_PRINT("Say: %s\n", s.c_str()); }
	auto get_int() { return 123; }
	auto get_uint() { return 456u; }
	auto get_float() { return 7.89f; }
	auto get_string() { return "abc"; }

	void test_main()
	{
		VALIDATE(api::create_extension_module("__main__"))
			.def("hello", hello)
			.def("say", say)
			.def("get_int", get_int)
			.def("get_uint", get_uint)
			.def("get_float", get_float)
			.def("get_string", get_string)
			.def("pass_ptr", [](void * a, void * b) { return b; })
			.def("pass_ptr", [](void * a) { return a; })
			;

		DICT g = api::globals();
		g["test_static"]();
		g["hello"]();
		g["say"](g["get_string"]());
		VERIFY(g["pass_ptr"]((void *)123).cast<void const *>() == (void *)123);
		VERIFY(g["pass_ptr"]((void *)123, (void *)321).cast<void const *>() == (void *)321);

		MAIN_PRINT("%s\n", api::typeof<METHOD>().attr("__subclasscheck__")(api::typeof<FUNCTION>()) ? "true" : "false");

		VERIFY(api::typeof<LIST>().attr("__size__").cast<size_t>() == sizeof(ListObject));

		LIST list{ api::typeof<LIST>()() };
		list.append("IT WORKS");
		MAIN_PRINT("%s\n", STR(list[0]).c_str());
		OBJ nn = api::typeof<LIST>().attr("__name__");
		MAIN_PRINT("%s\n", STR(nn).c_str());
		
		OBJ d{ api::typeof<DICT>()() };
		d["ABC"] = 42;
		d["DEF"] = "Hello, World!";
		VERIFY(d.contains("ABC"));
		MAIN_PRINT("%d\n", d["ABC"].cast<int>());
		MAIN_PRINT("%s\n", d["DEF"].cast<String>().c_str());
		MAIN_PRINT("%s\n", api::typeof(d).name().cast<String>().c_str());
		api::typeof(d).name() = "changed";
		MAIN_PRINT("%s\n", STR(api::typeof(d).name()).c_str());

		MAIN_PRINT("\n");
	}

	void test_scene()
	{
		FPS_Tracker fps_tracker{ 120 };

		SceneTree * scene{ (SceneTree *)get_os().get_main_loop() };
		scene->initialize(); SCOPE_EXIT(&) { scene->finalize(); };

		Window * window{ scene->get_root() };
		while (window->is_open())
		{
			static Duration delta_time{ 16.f / 1000.f };
			Timer const loop_timer{ true };

			window->poll_events();
			fps_tracker.update(delta_time);

			StringStream title{};
			title << "ism @ " << fps_tracker.value << " fps";
			window->set_title(title.str());

			ImGuiViewportP * im_viewport{ scene->get_imgui()->Viewports[0] };
			ImGui_NewFrame();
			ImGui::NewFrame();
			ImGuizmo::BeginFrame();
			{
				constexpr auto dockspace_name{ "##MainDockspace" };
				ImGuiID const dockspace_id{ ImGui::GetID(dockspace_name) };
				ImGui::SetNextWindowPos(im_viewport->Pos);
				ImGui::SetNextWindowSize(im_viewport->Size);
				ImGui::SetNextWindowViewport(im_viewport->ID);
				ImGui::SetNextWindowBgAlpha(0.f);
				ImGui::PushStyleVar(ImGuiStyleVarType_WindowRounding, 0.f);
				ImGui::PushStyleVar(ImGuiStyleVarType_WindowBorderSize, 0.f);
				ImGui::PushStyleVar(ImGuiStyleVarType_WindowPadding, Vec2{ 0.f, 0.f });
				bool const dockspace_open{ ImGui::Begin(dockspace_name, 0,
					ImGuiWindowFlags_NoTitleBar |
					ImGuiWindowFlags_NoCollapse |
					ImGuiWindowFlags_NoResize |
					ImGuiWindowFlags_NoMove |
					ImGuiWindowFlags_NoBringToFrontOnFocus |
					ImGuiWindowFlags_NoNavFocus |
					ImGuiWindowFlags_NoDocking |
					ImGuiWindowFlags_NoBackground
				) };
				ImGui::PopStyleVar(3);
				if (dockspace_open)
				{
					if (!ImGui::DockBuilderGetNode(dockspace_id)) {
						ImGui::DockBuilderRemoveNode(dockspace_id);
						ImGui::DockBuilderAddNode(dockspace_id);
						// BUILD DOCKSPACE HERE
						ImGui::DockBuilderFinish(dockspace_id);
					}
					ImGui::DockSpace(dockspace_id);
				}
				ImGui::End();

				// GUI GOES HERE
				ImGui::ShowDemoWindow();
			}
			ImGui::Render();

			Vec2 const window_size{ window->get_size() };
			glViewport(0, 0, (int32_t)window_size[0], (int32_t)window_size[1]);
			glClearColor(0.f, 0.f, 0.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT);

			ImGui_RenderDrawData(&im_viewport->DrawDataP);
			if (scene->get_imgui()->IO.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
				WindowID const backup{ get_display_server().get_current_context() };
				ImGui::UpdatePlatformWindows();
				ImGui::RenderPlatformWindowsDefault();
				get_display_server().make_context_current(backup);
			}
			
			window->swap_buffers();

			delta_time = loop_timer.elapsed();
		}
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern OS_IMPL(void *);

int main(int argc, char * argv[])
{
	OS_MAIN(nullptr);

	switch (Main::setup(argv[0], argc, argv))
	{
	case Error_None: break;
	default: FATAL("An unknown error occurred during setup and the program was unable to start.");
	}

	VERIFY(Main::start());

	test_main();
	test_scene();
	//MAIN_PAUSE();

	Main::cleanup();

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */