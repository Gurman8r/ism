#ifndef _ISM_ENGINE_HPP_
#define _ISM_ENGINE_HPP_

#include <core/os/main_loop.hpp>

namespace Ism
{
	class Engine final
	{
		SINGLETON_CLASS(Engine);

	public:
		Engine() noexcept;
		~Engine() noexcept;

	public:

	};

	SINGLETON_WRAPPER(Engine, engine);
}

#endif // !_ISM_ENGINE_HPP_
