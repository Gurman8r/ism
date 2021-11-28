#include <main/main.hpp>
#include <editor/editor_node.hpp>
#include <drivers/opengl/opengl.hpp>

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

	void test_api()
	{
		VALIDATE(create_extension_module("__main__"))
			.def("hello", hello)
			.def("say", say)
			.def("get_int", get_int)
			.def("get_uint", get_uint)
			.def("get_float", get_float)
			.def("get_string", get_string)
			.def("pass_ptr", [](void * a, void * b) { return b; })
			.def("pass_ptr", [](void * a) { return a; })
			;

		DICT g = globals();
		g["test_static"]();
		g["hello"]();
		g["say"](g["get_string"]());
		VERIFY(g["pass_ptr"]((void *)123).cast<void const *>() == (void *)123);
		VERIFY(g["pass_ptr"]((void *)123, (void *)321).cast<void const *>() == (void *)321);

		MAIN_PRINT("%s\n", typeof<METHOD>().attr("__subclasscheck__")(typeof<FUNCTION>()) ? "true" : "false");

		VERIFY(typeof<LIST>().attr("__size__").cast<size_t>() == sizeof(ListObject));

		LIST list{ typeof<LIST>()() };
		list.append("IT WORKS");
		MAIN_PRINT("%s\n", STR(list[0]).c_str());
		OBJ nn = typeof<LIST>().attr("__name__");
		MAIN_PRINT("%s\n", STR(nn).c_str());
		
		OBJ d{ typeof<DICT>()() };
		d["ABC"] = 42;
		d["DEF"] = "Hello, World!";
		VERIFY(d.contains("ABC"));
		MAIN_PRINT("%d\n", d["ABC"].cast<int>());
		MAIN_PRINT("%s\n", d["DEF"].cast<String>().c_str());
		MAIN_PRINT("%s\n", typeof(d).attr("__name__").cast<String>().c_str());

		EventSystem bus{};
		bus.add_dummy_listener<InputEvent>([](Event const & ev)
		{
			if ((EventID)ev == InputEvent::ID)
			{
				MAIN_PRINT("Hello from Dummy!\n");
			}
		});
		bus.get_delegate<InputEvent>() += [](Event const & ev)
		{
			if ((EventID)ev == InputEvent::ID)
			{
				MAIN_PRINT("Hello from Delegate!\n");
			}
		};
		bus.fire_event(InputEvent{});

		MAIN_PRINT("\n");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char * argv[])
{
	OS_DEFAULT();

	switch (Main::setup(argv[0], argc, argv))
	{
	case Error_None: break;
	default: FATAL("An unknown error occurred during setup and the program was unable to start.");
	}

	VERIFY(Main::start());

	test_api();

	get_os().get_main_loop()->initialize();
	while (Main::iteration() == 0);
	get_os().get_main_loop()->finalize();

	//MAIN_PAUSE();

	Main::cleanup();

	return EXIT_SUCCESS;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */