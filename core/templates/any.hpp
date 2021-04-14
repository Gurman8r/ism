#ifndef _ISM_ANY_HPP_
#define _ISM_ANY_HPP_

#include <core/templates/type_traits.hpp>

#include <any>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class Any : public std::any
	{
		using _Mybase = std::any;
	public:
		using _Mybase::any;
		NODISCARD operator _Mybase & () & noexcept { return static_cast<_Mybase &>(*this); }
		NODISCARD operator _Mybase const & () const & noexcept { return static_cast<_Mybase const &>(*this); }
		NODISCARD operator _Mybase && () && noexcept { return static_cast<_Mybase &&>(std::move(*this)); }
		
		void swap(Any & other) noexcept { std::swap(static_cast<_Mybase &>(*this), static_cast<_Mybase &>(other)); }
		
		template <class T> NODISCARD auto cast() -> T * { return _Mybase::_Cast<T>(); }
		
		template <class T> NODISCARD auto cast() const -> T const * { return _Mybase::_Cast<T>(); }
		
		template <class T> NODISCARD operator T() & { return *_Mybase::_Cast<T>(); }
		
		template <class T> NODISCARD operator T() const & { return *_Mybase::_Cast<T>(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class ... Args
	> NODISCARD Any make_any(Args && ... args)
	{
		return Any{ std::in_place_type<T>, FWD(args)... };
	}

	template <class T, class E, class ... Args
	> NODISCARD Any make_any(std::initializer_list<E> ilist, Args && ... args)
	{
		return Any{ std::in_place_type<T>, ilist, FWD(args)... };
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T
	> NODISCARD const T * any_cast(Any const * const value) noexcept
	{
		static_assert(!std::is_void_v<T>, "Any cannot contain void.");
		if constexpr (std::is_function_v<T> || std::is_array_v<T>) { return nullptr; }
		else if (!value) { return nullptr; }
		else { return value->_Cast<std::_Remove_cvref_t<T>>(); }
	}

	template <class T
	> NODISCARD T * any_cast(Any * const value) noexcept
	{
		static_assert(!std::is_void_v<T>, "Any cannot contain void.");
		if constexpr (std::is_function_v<T> || std::is_array_v<T>) { return nullptr; }
		else if (!value) { return nullptr; }
		else { return value->_Cast<std::_Remove_cvref_t<T>>(); }
	}

	template <class T
	> NODISCARD std::remove_cv_t<T> any_cast(Any const & value)
	{
		static_assert(
			std::is_constructible_v<std::remove_cv_t<T>, std::_Remove_cvref_t<T> const &>,
			"any_cast<T>(Any const&) requires remove_cv_t<T> to be constructible from "
			"const remove_cv_t<remove_reference_t<T>>&");
		auto const ptr{ ism::any_cast<std::_Remove_cvref_t<T>>(&value) };
		if (!ptr) { std::_Throw_bad_any_cast(); }
		return static_cast<std::remove_cv_t<T>>(*ptr);
	}

	template <class T
	> NODISCARD std::remove_cv_t<T> any_cast(Any & value)
	{
		static_assert(
			std::is_constructible_v<std::remove_cv_t<T>, std::_Remove_cvref_t<T> &>,
			"any_cast<T>(Any&) requires remove_cv_t<T> to be constructible from remove_cv_t<remove_reference_t<T>>&");
		auto const ptr = ism::any_cast<std::_Remove_cvref_t<T>>(&value);
		if (!ptr) { std::_Throw_bad_any_cast(); }
		return static_cast<std::remove_cv_t<T>>(*ptr);
	}

	template <class T
	> NODISCARD std::remove_cv_t<T> any_cast(Any && value)
	{
		static_assert(
			std::is_constructible_v<std::remove_cv_t<T>, std::_Remove_cvref_t<T>>,
			"any_cast<T>(Any&&) requires remove_cv_t<T> to be constructible from remove_cv_t<remove_reference_t<T>>");
		auto const ptr = ism::any_cast<std::_Remove_cvref_t<T>>(&value);
		if (!ptr) { std::_Throw_bad_any_cast(); }
		return static_cast<std::remove_cv_t<T>>(std::move(*ptr));
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_ANY_HPP_
