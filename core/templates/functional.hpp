#ifndef _ISM_FUNCTIONAL_HPP_
#define _ISM_FUNCTIONAL_HPP_

#include <functional>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Signature
	> class StdFn : public std::function<Signature> {
		using _Mybase = std::function<Signature>;
	public:
		DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(StdFn);
		
		using typename _Mybase::result_type;
		using _Mybase::function;
		NODISCARD operator _Mybase & () & noexcept { return static_cast<_Mybase &>(*this); }
		NODISCARD operator _Mybase const & () const & noexcept { return static_cast<_Mybase const &>(*this); }
		NODISCARD operator _Mybase && () && noexcept { return static_cast<_Mybase &&>(std::move(*this)); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FUNCTIONAL_HPP_
