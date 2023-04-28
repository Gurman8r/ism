#ifndef _ISM_ENGINE_HPP_
#define _ISM_ENGINE_HPP_

#include <core/os/main_loop.hpp>

namespace ism
{
	class Engine final
	{
		static Engine * __singleton;

	public:
		Engine() noexcept;
		~Engine() noexcept;
		FORCE_INLINE static Engine * get_singleton() noexcept { return __singleton; }
#define ENGINE (ism::Engine::get_singleton())

	public:

	};
}

#endif // !_ISM_ENGINE_HPP_