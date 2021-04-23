#include <main/main.hpp>
#include <core/api/modsupport.hpp>

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
		int x{}; String y;

		static void test_static() { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }
	
		auto test_const() const { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }
	};
	
	void hello() { MAIN_PRINT("Hello: %s\n", PRETTY_FUNCTION); }
	void say(String const & s) { MAIN_PRINT("Say: %s\n", s.c_str()); }
	auto get_int() { return 123; }
	auto get_uint() { return 456u; }
	auto get_float() { return 7.89f; }
	auto get_string() { return "abc"; }

	void test_main(int32_t argc, char * argv[])
	{
		MODULE m{ create_extension_module("__main__") };
		VERIFY(globals().is_valid());

		CoreClass<Test>(m, "Test")
			//.def(init<>())
			//.def_readwrite("x", &Test::x)
			//.def_static("hello", hello)
			;

		m->def("hello", hello);
		globals()["hello"]();

		globals()["say"] = CPP_FUNCTION({ say, detail::scope(m) });
		globals()["get_string"] = CPP_FUNCTION(get_string);
		globals()["say"](globals()["get_string"]());

		LIST list = globals()["a"] = LIST(CoreList{});
		list->append("IT WORKS");
		MAIN_PRINT("%s\n", STR(list[0])->c_str());

		OBJECT o{ DICT(CoreDict{}) };
		globals()["ABC"] = 42;
		o["DEF"] = "Hello, World!";
		MAIN_PRINT("%d\n", globals()["ABC"].cast<int>());
		MAIN_PRINT("%s\n", o["DEF"].cast<String>().c_str());
		MAIN_PRINT("%s\n", typeof(o).attr("__name__").cast<std::string>().c_str());
		typeof(o).attr("__name__") = "changed";
		MAIN_PRINT("%s\n", STR(typeof(o).attr("__name__"))->c_str());

		MAIN_PRINT("\n");
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

extern ism::OS & __implement_os(void * instance = nullptr);

int main(int argc, char * argv[])
{
	auto & ANONYMOUS{ __implement_os() };

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