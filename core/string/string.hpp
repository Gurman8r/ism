#ifndef _ISM_STRING_HPP_
#define _ISM_STRING_HPP_

#include <core/error/error_macros.hpp>
#include <core/string/string_view.hpp>
#include <core/os/memory.hpp>

#include <string>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string base
	template <class C = char
	> ALIAS(_StringBase) std::basic_string<C, std::char_traits<C>, PolymorphicAllocator<C>>;

	// basic string
	template <class C = char
	> class BasicString : public _StringBase<C>
	{
	public:
		using base_type = _StringBase<C>;
		using base_type::base_type;
		using base_type::operator=;

		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }

		NODISCARD auto hash_code() const noexcept { return ism::hash(data(), size()); }

		NODISCARD auto view() const noexcept { return BasicStringView<C>{ data(), size() }; }

		NODISCARD operator BasicStringView<C>() const noexcept { return { data(), size() }; }

		auto & trim() & noexcept {
			while (!empty() && std::isspace(back())) { pop_back(); }
			while (!empty() && std::isspace(front())) { erase(begin()); }
			return (*this);
		}

	public:
		static int32_t format(BasicString & s, C const * fmt, va_list args)
		{
			va_list args_copy;
			va_copy(args_copy, args);
			auto const n{ std::vsnprintf(nullptr, 0, fmt, args_copy) };
			va_end(args_copy);
			if (0 < n) {
				s.resize((size_t)n);
				std::vsnprintf(s.data(), s.size(), fmt, args);
			}
			return n;
		}

		static int32_t format(BasicString & s, C const * fmt, ...) noexcept
		{
			va_list args;
			va_start(args, fmt);
			auto const n{ format(s, fmt, args) };
			va_end(args);
			return n;
		}

		NODISCARD static BasicString format(C const * fmt, va_list args) noexcept
		{
			BasicString s;
			format(s, fmt, args);
			return s;
		}

		NODISCARD static BasicString format(C const * fmt, ...) noexcept
		{
			va_list args;
			va_start(args, fmt);
			auto const s{ format(fmt, args) };
			va_end(args);
			return s;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string
	ALIAS(String) BasicString<char>;

	// wide string
	ALIAS(WideString) BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <class C> constexpr bool is_string_v<BasicString<C>>{ true }; }

	inline String operator "" _s(cstring s, size_t n) noexcept { return String{ s, n }; }

	inline WideString operator "" _s(cwstring s, size_t n) noexcept { return WideString{ s, n }; }

	template <class C> struct Hasher<BasicString<C>>
	{
		hash_t operator()(BasicString<C> const & s) const noexcept { return s.hash_code(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> const & b)
	{
		auto const a_size{ a.size() };
		auto const b_size{ b.size() };
		if (a.max_size() - a_size < b_size) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> const & b)
	{
		auto const a_size{ std::_Convert_size<size_t>(std::char_traits<C>:length(a)) };
		auto const b_size{ b.size() };
		if (b.max_size() - b_size < a_size) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, b, a, a_size, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const a, BasicString<C> const & b)
	{
		auto const b_size{ b.size() };
		if (b_size == b.max_size()) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, b, std::addressof(a), 1, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const * const b)
	{
		auto const a_size{ a.size() };
		auto const b_size{ std::_Convert_size<size_t>(std::char_traits<C>::length(b)) };
		if (a.max_size() - a_size < b_size) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b, b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const b)
	{
		auto const a_size{ a.size() };
		if (a_size == a.max_size()) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, std::addressof(b), 1 };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> && b)
	{
		return std::move(b.insert(0, a));
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> const & b)
	{
		return std::move(a.append(b));
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> && b)
	{
		ASSERT(std::addressof(a) != std::addressof(b) &&
			"You cannot concatenate the same moved string to itself. See "
			"N4849 [res.on.arguments]/1.3: If a function argument binds to an rvalue reference "
			"parameter, the implementation may assume that this parameter is a unique reference "
			"to this argument");
		return { std::_String_constructor_concat_tag{}, a, b };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> && b)
	{
		return std::move(b.insert(0, a));
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const a, BasicString<C> && b)
	{
		return std::move(b.insert(0, 1, a));
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const * const b)
	{
		return std::move(a.append(b));
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const b)
	{
		return a.push_back(b), std::move(a);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C
	> NODISCARD bool operator==(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a._Equal(b); }

	template <class C
	> NODISCARD bool operator==(BasicString<C> const & a, C const * const b) { return a._Equal(b); }

	template <class C
	> NODISCARD bool operator==(C const * const a, BasicString<C> const & b) { return b._Equal(a); }

	template <class C
	> NODISCARD bool operator!=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a == b); }

	template <class C
	> NODISCARD bool operator!=(C const * const a, BasicString<C> const & b) { return !(a == b); }

	template <class C
	> NODISCARD bool operator!=(BasicString<C> const & a, C const * const b) { return !(a == b); }

	template <class C
	> NODISCARD bool operator<(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a.compare(b) < 0; }

	template <class C
	> NODISCARD bool operator<(C const * const a, BasicString<C> const & b) { return b.compare(a) > 0; }

	template <class C
	> NODISCARD bool operator<(BasicString<C> const & a, C const * const b) { return a.compare(b) < 0; }

	template <class C
	> NODISCARD bool operator>(BasicString<C> const & a, BasicString<C> const & b) noexcept { return b < a; }

	template <class C
	> NODISCARD bool operator>(C const * const a, BasicString<C> const & b) { return b < a; }

	template <class C
	> NODISCARD bool operator>(BasicString<C> const & a, C const * const b) { return b < a; }

	template <class C
	> NODISCARD bool operator<=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(b < a); }

	template <class C
	> NODISCARD bool operator<=(C const * const a, BasicString<C> const & b) { return !(b < a); }

	template <class C
	> NODISCARD bool operator<=(BasicString<C> const & a, C const * const b) { return !(b < a); }

	template <class C
	> NODISCARD bool operator>=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a < b); }

	template <class C
	> NODISCARD bool operator>=(C const * const a, BasicString<C> const & b) { return !(a < b); }

	template <class C
	> NODISCARD bool operator>=(BasicString<C> const & a, C const * const b) { return !(a < b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_HPP_
