#include <core/config/engine.hpp>
#include <corecrt_wstdio.h>

namespace Ism
{
	Engine * Engine::__singleton{};

	Engine::Engine() noexcept
	{
		SINGLETON_CTOR(__singleton, this);



		auto _formatter = []() {
			if constexpr (true) {
				return std::vsnprintf;
			}
			else {
				return std::vswprintf;
			}
		};
	}

	Engine::~Engine() noexcept
	{
		SINGLETON_DTOR(__singleton, this);
	}
}