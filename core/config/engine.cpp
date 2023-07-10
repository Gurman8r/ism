#include <core/config/engine.hpp>

namespace Ism
{
	SINGLETON_EMBED(Engine);

	Engine::Engine()
	{
		SINGLETON_CTOR();
	}

	Engine::~Engine()
	{
		SINGLETON_DTOR();
	}
}