#include <main/main.hpp>
#include <core/api/runtime.hpp>

using namespace ISM;
extern OS const * create_os(void * obj = {});
static OS const * ANONYMOUS{ CHECK(create_os()) };

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define MAIN_PRINT (ISM::get_os().print)

#if ISM_OS_WINDOWS
#define MAIN_PAUSE() \
	std::system("pause")
#else
#define MAIN_PAUSE() \
	([]() noexcept -> int { \
		ISM::MAIN_PRINT("Press enter to continue . . ."); \
		ISM::get_os().get_stdin_string(true); \
		return 0; \
	})()
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

namespace ISM
{
	struct Test
	{
		static void test_static() { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }

		auto test_const() const { MAIN_PRINT("%s\n", PRETTY_FUNCTION); }
	};

	void hello() { MAIN_PRINT("Hello: %s\n", PRETTY_FUNCTION); }

	void say(String const & s) { MAIN_PRINT("Say: %s\n", s.c_str()); }

	auto get_int() { return 123; }
	auto get_float() { return 4.56f; }

	Err test_main(int32_t argc, char * argv[])
	{
		METHOD{}.instance([&]() { MAIN_PRINT("AHHH: %s\n", PRETTY_FUNCTION); })->m_data->call(0);
		bind_method([&]() { MAIN_PRINT("Hello: %s\n", PRETTY_FUNCTION); })->call(0);
		MAIN_PRINT("\n");

		bind_method(hello)->call(0);
		bind_method(say)->call(0, String{ "Goodbye, World!" });
		MAIN_PRINT("%d\n", bind_method(get_int)->call<int>(0));
		MAIN_PRINT("%f\n", bind_method(get_float)->call<float>(0));
		MAIN_PRINT("\n");
		
		Test test{};
		bind_method(&Test::test_static)->call(0);
		bind_method(&Test::test_const)->call(&test);
		MAIN_PRINT("\n");

		OBJECT o{ make_generic(typeof<DICT>()) };
		o["ABC"] = 42;
		o["DEF"] = "Hello, World!";
		MAIN_PRINT("%d\n", o["ABC"].cast<int>());
		MAIN_PRINT("%s\n", o["DEF"].cast<String>().c_str());
		MAIN_PRINT("%s\n", o.type().attr("__name__").cast<std::string>().c_str());
		o.type().attr("__name__") = "changed";
		MAIN_PRINT("%s\n", STR(o.type().attr("__name__"))->c_str());

		MAIN_PRINT("\n");
		return Err_None;
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

int main(int argc, char * argv[])
{
	switch (Main::setup(argv[0], argc, argv))
	{
	case Err_None: break;
	default: FATAL("An unknown error occurred during setup and the program was unable to start.");
	}

	VERIFY(Main::start());

	Err result{ test_main(argc, argv) };

	Main::cleanup();

	VERIFY(result == Err_None);

	return MAIN_PAUSE();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */