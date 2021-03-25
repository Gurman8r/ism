#include <main/main.hpp>
#include <core/api/runtime.hpp>

using namespace ISM;
extern OS const * create_os(void * instance = {});
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
	Err test_main(int32_t argc, char * argv[])
	{
		OBJECT o{ make_generic(CoreDict::type_static()) };

		o["ABC"] = 42;
		o["DEF"] = "Hello, World!";

		MAIN_PRINT("%d\n", o["ABC"].cast<int>());
		MAIN_PRINT("%s\n", o["DEF"].cast<String>().c_str());

		MAIN_PRINT("%s\n", o.type().name().cast<String>().c_str());

		o.type().name() = "changed";

		MAIN_PRINT("%s\n", STR(o.type().name()).c_str());
		MAIN_PRINT("%s\n", STR(123).c_str());

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