#ifndef _ISM_MAIN_LOOP_HPP_
#define _ISM_MAIN_LOOP_HPP_

#include <core/api/class.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API MainLoop : public Super
	{
		ISM_SUPER(MainLoop, Super);

	private:
		OBJECT m_startup_script{};

	public:
		virtual ~MainLoop() override;

		virtual void initialize();

		virtual bool process(Duration delta_time);

		virtual void finalize();

		void set_startup_script(OBJECT value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_MAIN_LOOP_HPP_
