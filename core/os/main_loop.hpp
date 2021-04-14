#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/api/super.hpp>

namespace ism
{
	class ISM_API MainLoop : public Super
	{
		Any m_startup_script;

	public:
		virtual ~MainLoop() noexcept = default;

		MainLoop() noexcept = default;

		virtual void initialize();

		virtual bool iteration(float_t dt);

		virtual void finalize();

		void set_startup_script(Any const & value);
	};
}

#endif // !_ISM_MAIN_LOOP_HPP_
