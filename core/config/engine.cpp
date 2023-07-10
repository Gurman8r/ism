#include <core/config/engine.hpp>

namespace Ism
{
	SINGLETON_EMBED(Engine);

	Engine::Engine() noexcept
	{
		SINGLETON_CTOR();
	}

	Engine::~Engine() noexcept
	{
		SINGLETON_DTOR();
	}
}