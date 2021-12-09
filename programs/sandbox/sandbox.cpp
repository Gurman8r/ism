#include <main/main.hpp>

using namespace ism;

//int main(int argc, char * argv[])
//{
//	OS_DEFAULT();
//
//	push_memory_arena((size_t)1_GiB);
//
//	switch (Main::setup(argv[0], argc, argv))
//	{
//	case Error_None: break;
//	default: CRASH("An unknown error occurred during setup. The program was unable to start.");
//	}
//
//	ASSERT(Main::start());
//
//	while (Main::iteration() == EXIT_SUCCESS);
//
//	Main::cleanup();
//
//	pop_memory_arena();
//
//	return EXIT_SUCCESS;
//}