#ifndef _ISM_FUNCTIONAL_HPP_
#define _ISM_FUNCTIONAL_HPP_

#include <functional>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if 1
	template <class Fx> ALIAS(StdFn) std::function<Fx>;
#else
	template <class Signature
	> class StdFn : public std::function<Signature> {
		using _Mybase = std::function<Signature>;
	public:
		using _Mybase::function;
		NODISCARD operator _Mybase & () & noexcept { return static_cast<_Mybase &>(*this); }
		NODISCARD operator _Mybase const & () const & noexcept { return static_cast<_Mybase const &>(*this); }
		NODISCARD operator _Mybase && () && noexcept { return static_cast<_Mybase &&>(std::move(*this)); }
	};
#endif // 1

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FUNCTIONAL_HPP_
