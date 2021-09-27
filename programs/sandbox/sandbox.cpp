#include <main/main.hpp>
#include <runtime/scene/scene_tree.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MAIN_PRINT (ism::get_os().print)

#define MAIN_PAUSE (ism::get_os().pause)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ism
{
	struct Test
	{
		int x{};

		String y{};

		static void test_static() { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }

		int test_const() const { MAIN_PRINT("%s\n", PRETTY_FUNCTION); return 0; }
	};

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
			.def("test_static", &Test::test_static)
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
		SceneTree * scene{ (SceneTree *)get_os().get_main_loop() };
		scene->initialize(); SCOPE_EXIT(&) { scene->finalize(); };

		Window * window{ scene->get_window() };
		while (window && window->is_open())
		{
			static Duration delta_time{};

			Timer const loop_timer{ true };

			window->poll_events();

			StringStream title{};
			title << "ism @ " << (delta_time.count() * 1000.f) << " ms/frame";
			window->set_title(title.str());

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