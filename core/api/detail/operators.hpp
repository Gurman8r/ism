#ifndef _ISM_API_DETAIL_OPERATORS_HPP_
#define _ISM_API_DETAIL_OPERATORS_HPP_

#include <core/api/detail/common.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD bool operator==(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.equal_to(b); }

	template <class T
	> NODISCARD bool operator!=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.not_equal_to(b); }

	template <class T
	> NODISCARD bool operator<(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less(b); }

	template <class T
	> NODISCARD bool operator<=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.less_equal(b); }

	template <class T
	> NODISCARD bool operator>(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater(b); }

	template <class T
	> NODISCARD bool operator>=(ObjectAPI<T> const & a, ObjectAPI<T> const & b) { return a.greater_equal(b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hash<Handle<T>>
	{
		NODISCARD hash_t operator()(Handle<T> const & o) const { return hash(o); }
	};

	template <class T> struct EqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.equal_to(b); }
	};

	template <class T> struct NotEqualTo<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.not_equal_to(b); }
	};

	template <class T> struct Less<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less(b); }
	};

	template <class T> struct Greater<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater(b); }
	};

	template <class T> struct LessEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.less_equal(b); }
	};

	template <class T> struct GreaterEqual<Handle<T>>
	{
		template <class U
		> NODISCARD bool operator()(Handle<T> const & a, Handle<U> const & b) const { return a.greater_equal(b); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_DETAIL_OPERATORS_HPP_
