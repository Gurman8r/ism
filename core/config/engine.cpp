#include <core/config/engine.hpp>

namespace Ism
{
	Engine * Engine::__singleton{};

	Engine::Engine() noexcept
	{
		SINGLETON_CTOR(__singleton, this);
	}

	Engine::~Engine() noexcept
	{
		SINGLETON_DTOR(__singleton, this);
	}
}