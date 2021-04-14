#include <main/main.hpp>
#include <core/api/bind.hpp>

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
	//struct Test
	//{
	//	static void test_static() { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }
	//
	//	auto test_const() const { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }
	//};
	//
	//void hello() { MAIN_PRINT("Hello: %s\n", PRETTY_FUNCTION); }
	//void say(String const & s) { MAIN_PRINT("Say: %s\n", s.c_str()); }
	//auto get_int() { return 123; }
	//auto get_uint() { return 456u; }
	//auto get_float() { return 7.89f; }
	//auto get_string() -> String { return "abc"; }

	void test_main(int32_t argc, char * argv[])
	{
		VERIFY(core::get_interpreter());

		LIST list = LIST::create();
		list->append("");
		list[0] = "IT WORKS\n\n";
		MAIN_PRINT("%s", STR(list[0])->c_str());

		OBJECT o{ DICT::create() };
		o["ABC"] = 42;
		o["DEF"] = "Hello, World!";
		MAIN_PRINT("%d\n", o["ABC"].cast<int>());
		MAIN_PRINT("%s\n", o["DEF"].cast<String>().c_str());
		MAIN_PRINT("%s\n", o.type().attr("__name__").cast<std::string>().c_str());
		o.type().attr("__name__") = "changed";
		MAIN_PRINT("%s\n", STR(o.type().attr("__name__"))->c_str());


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
	case Err_None: break;
	default: FATAL("An unknown error occurred during setup and the program was unable to start.");
	}

	VERIFY(Main::start());

	test_main(argc, argv);

	Main::cleanup();

	return MAIN_PAUSE();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */