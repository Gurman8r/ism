#include <core/config/engine.hpp>

namespace ism
{
	Engine * Engine::__singleton{};

	Engine::Engine() noexcept
	{
		SINGLETON_CTOR();
	}

	Engine::~Engine() noexcept
	{
		SINGLETON_DTOR();
	}
}