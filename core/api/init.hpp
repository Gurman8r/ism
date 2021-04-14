#ifndef _ISM_INIT_HPP_
#define _ISM_INIT_HPP_

#include <core/api/class.hpp>

namespace ism::detail::initimpl
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class ... Args
	> struct Constructor
	{
		template <class C, class ... Extra
		> static void execute(C & c, Extra && ... extra)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class F, class = function_signature_t<F>
	> struct Factory;

	template <class F, class R, class ... Args
	> struct Factory<F, R(Args...)>
	{
		Factory(F && f) {}

		template <class C, class ... Extra
		> void execute(C & c, Extra && ... extra)
		{
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INIT_HPP_
