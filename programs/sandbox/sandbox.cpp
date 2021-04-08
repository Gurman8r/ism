#include <main/main.hpp>
#include <core/api/callable.hpp>

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

	void say_hello() { MAIN_PRINT("Hello, World!\n"); }

	void pass_int(int i) { MAIN_PRINT("%d\n", i); }

	int get_int() { return 123; }

	void say(String const & s) { MAIN_PRINT("%s\n", s.c_str()); }

	Err test_main(int32_t argc, char * argv[])
	{
		bind_method(say_hello)->call(NULL);

		bind_method([&]() { MAIN_PRINT("Hello, World!\n"); })->call(NULL);

		bind_method(say)->call(NULL, String{ "Goodbye, World!" });

		Test test{};
		bind_method(&Test::test_static)->call(NULL);
		bind_method(&Test::test_const)->call(&test);

		return Err_None;

		OBJECT o{ make_generic(DICT::type_static()) };
		o["ABC"] = 42;
		o["DEF"] = "Hello, World!";

		MAIN_PRINT("%d\n", o["ABC"].cast<int>());
		MAIN_PRINT("%s\n", o["DEF"].cast<String>().c_str());

		MAIN_PRINT("%s\n", o.type().attr("__name__").cast<std::string>().c_str());

		o.type().attr("__name__") = "changed";

		MAIN_PRINT("%s\n", STR(o.type().attr("__name__"))->c_str());

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