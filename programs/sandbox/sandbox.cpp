#include <main/main.hpp>
#include <core/api/class.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MAIN_PRINT (ism::get_os().print)

#if ISM_OS_WINDOWS
#define MAIN_PAUSE() \
	std::system("pause")
#else
#define MAIN_PAUSE() \
	([]() noexcept -> int { \
		ism::MAIN_PRINT("Press enter to continue . . ."); \
		ism::get_os().get_stdin_string(true); \
		return 0; \
	})()
#endif

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

	void test_main(int32_t argc, char * argv[])
	{
		MODULE m = create_extension_module("__main__")
			.def("hello", hello)
			.def("say", say)
			.def("get_int", get_int)
			.def("get_uint", get_uint)
			.def("get_float", get_float)
			.def("get_string", get_string)
			.def("pass_ptr", [](void * a) { return a; })
			.def("pass_ptr", [](void * a, void * b) { return b; })
			;

		//CLASS_<Test>(m, "Test")
		//	.def(init<>())
		//	.def(init<int>())
		//	.def(init<int, String const &>())
		//	.def_static("test_static", &Test::test_static)
		//	//.def("test_const", &Test::test_const)
		//	;

		m.attr("hello")();
		m.attr("say")(m.attr("get_string")());
		VERIFY(m.attr("pass_ptr")((void *)123).cast<void const *>() == (void *)123);
		VERIFY(m.attr("pass_ptr")((void *)123, (void *)321).cast<void const *>() == (void *)321);

		MAIN_PRINT("%s\n", typeof<METHOD>().attr("__subclasscheck__")(typeof<FUNCTION>()) ? "true" : "false");
		
		LIST list = m.attr("list") = LIST::new_();
		list.append("IT WORKS");
		MAIN_PRINT("%s\n", STR(list[0]).c_str());
		MAIN_PRINT("%s\n", STR(typeof(list).attr("__name__")).c_str());
		
		OBJECT d{ DICT::new_() };
		d["ABC"] = 42;
		d["DEF"] = "Hello, World!";
		VERIFY(d.contains("ABC"));
		MAIN_PRINT("%d\n", d["ABC"].cast<int>());
		MAIN_PRINT("%s\n", d["DEF"].cast<String>().c_str());
		MAIN_PRINT("%s\n", typeof(d).name().cast<String>().c_str());
		typeof(d).name() = "changed";
		MAIN_PRINT("%s\n", STR(typeof(d).name()).c_str());
		
		MAIN_PRINT("\n");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char * argv[])
{
	extern ISM_IMPLEMENTATION(void *);
	
	IMPLEMENT_ISM(nullptr);

	switch (Main::setup(argv[0], argc, argv))
	{
	case Error_None: break;
	default: FATAL("An unknown error occurred during setup and the program was unable to start.");
	}

	VERIFY(Main::start());

	test_main(argc, argv);

	Main::cleanup();

	return MAIN_PAUSE();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */