#include <core/config/engine.hpp>

namespace ism
{
	Engine * Engine::__singleton{};

	Engine::Engine() noexcept
	{
		ASSERT(!__singleton);
		__singleton = this;
	}

	Engine::~Engine() noexcept
	{
	}
}