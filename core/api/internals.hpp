#ifndef _ISM_INTERNALS_HPP_
#define _ISM_INTERNALS_HPP_

#include <core/api/super.hpp>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Internals final
	{
	private:
		static Internals singleton;
		Internals() {}
		~Internals() {}

	public:
		FORCE_INLINE static Internals * get_singleton() { return &singleton; }

		Batch<SuperID, Super *> super_data;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	NODISCARD inline Internals & get_internals() { return *Internals::get_singleton(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_INTERNALS_HPP_
