#ifndef _ISM_ENGINE_HPP_
#define _ISM_ENGINE_HPP_

#include <core/os/main_loop.hpp>

namespace Ism
{
	class Engine final
	{
		static Engine * __singleton;

	public:
		Engine() noexcept;
		~Engine() noexcept;
		FORCE_INLINE static Engine * get_singleton() noexcept { return __singleton; }

	public:

	};

	SINGLETON_WRAPPER(Engine, get_engine);
}

#endif // !_ISM_ENGINE_HPP_
