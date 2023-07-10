#ifndef _ISM_ENGINE_HPP_
#define _ISM_ENGINE_HPP_

#include <core/os/main_loop.hpp>

namespace Ism
{
	class Engine final
	{
		SINGLETON_CLASS(Engine);

		bool m_is_editor_hint : 1;

	public:
		Engine();
		~Engine();

		NODISCARD bool is_editor_hint() const noexcept { return m_is_editor_hint; }
	};

	SINGLETON_WRAPPER(Engine, engine);
}

#endif // !_ISM_ENGINE_HPP_
