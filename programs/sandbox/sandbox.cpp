#include <main/main.hpp>
#include <core/input/input_event.hpp>

int main(int argc, char * argv[])
{
	using namespace ism;

	OS_DEFAULT();

	switch (Main::setup(argv[0], argc, argv))
	{
	case Error_None: break;
	default: CRASH("An unknown error occurred during setup. The program was unable to start.");
	}

	ASSERT(Main::start());

	while (Main::iteration() == EXIT_SUCCESS);

	Main::cleanup();

	return EXIT_SUCCESS;
}