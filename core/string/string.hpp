#ifndef _ISM_STRING_HPP_
#define _ISM_STRING_HPP_

#include <core/string/string_view.hpp>
#include <core/os/memory.hpp>

#include <string>

// string
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string base
	template <class Ch = char
	> ALIAS(_StringBase) std::basic_string<Ch, std::char_traits<Ch>, PolymorphicAllocator<Ch>>;

	// basic string
	template <class Ch = char
	> class BasicString : public _StringBase<Ch>
	{
	public:
		using base_type = _StringBase<Ch>;
		using base_type::base_type;
		using base_type::operator=;
		auto hash_code() const noexcept { return ism::hash(data(), size()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string
	ALIAS(String) BasicString<char>;

	// wide string
	ALIAS(WideString) BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <class Ch> constexpr bool is_string_v<BasicString<Ch>>{ true }; }

	inline String operator "" _s(cstring s, size_t n) noexcept { return String{ s, n }; }

	inline WideString operator "" _s(cwstring s, size_t n) noexcept { return WideString{ s, n }; }

	template <class Ch> struct Hasher<BasicString<Ch>>
	{
		hash_t operator()(BasicString<Ch> const & s) const noexcept { return s.hash_code(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// string operators
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> const & a, BasicString<Ch> const & b)
	{
		auto const a_size{ a.size() };
		auto const b_size{ b.size() };
		if (a.max_size() - a_size < b_size) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b.c_str(), b_size };
	}

	template <class Ch
	> BasicString<Ch> operator+(Ch const * const a, BasicString<Ch> const & b)
	{
		auto const a_size{ std::_Convert_size<size_t>(std::char_traits<Ch>:length(a)) };
		auto const b_size{ b.size() };
		if (b.max_size() - b_size < a_size) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, b, a, a_size, b.c_str(), b_size };
	}

	template <class Ch
	> BasicString<Ch> operator+(Ch const a, BasicString<Ch> const & b)
	{
		auto const b_size{ b.size() };
		if (b_size == b.max_size()) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, b, std::addressof(a), 1, b.c_str(), b_size };
	}

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> const & a, Ch const * const b)
	{
		auto const a_size{ a.size() };
		auto const b_size{ std::_Convert_size<size_t>(std::char_traits<Ch>::length(b)) };
		if (a.max_size() - a_size < b_size) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b, b_size };
	}

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> const & a, Ch const b)
	{
		auto const a_size{ a.size() };
		if (a_size == a.max_size()) { std::_Xlen_string(); }
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, std::addressof(b), 1 };
	}

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> const & a, BasicString<Ch> && b)
	{
		return std::move(b.insert(0, a));
	}

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> && a, BasicString<Ch> const & b)
	{
		return std::move(a.append(b));
	}

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> && a, BasicString<Ch> && b)
	{
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(std::addressof(a) != std::addressof(b),
			"You cannot concatenate the same moved string to itself. See "
			"N4849 [res.on.arguments]/1.3: If a function argument binds to an rvalue reference "
			"parameter, the implementation may assume that this parameter is a unique reference "
			"to this argument");
#endif // _ITERATOR_DEBUG_LEVEL == 2
		return { std::_String_constructor_concat_tag{}, a, b };
	}

	template <class Ch
	> BasicString<Ch> operator+(Ch const * const a, BasicString<Ch> && b)
	{
		return std::move(b.insert(0, a));
	}

	template <class Ch
	> BasicString<Ch> operator+(Ch const a, BasicString<Ch> && b)
	{
		return std::move(b.insert(0, 1, a));
	}

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> && a, Ch const * const b)
	{
		return std::move(a.append(b));
	}

	template <class Ch
	> BasicString<Ch> operator+(BasicString<Ch> && a, Ch const b)
	{
		a.push_back(b);
		return std::move(a);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch
	> bool operator==(BasicString<Ch> const & a, BasicString<Ch> const & b) noexcept { return a._Equal(b); }

	template <class Ch
	> bool operator==(BasicString<Ch> const & a, Ch const * const b) { return a._Equal(b); }

	template <class Ch
	> bool operator==(Ch const * const a, BasicString<Ch> const & b) { return b._Equal(a); }

	template <class Ch
	> bool operator!=(BasicString<Ch> const & a, BasicString<Ch> const & b) noexcept { return !(a == b); }

	template <class Ch
	> bool operator!=(Ch const * const a, BasicString<Ch> const & b) { return !(a == b); }

	template <class Ch
	> bool operator!=(BasicString<Ch> const & a, Ch const * const b) { return !(a == b); }

	template <class Ch
	> bool operator<(BasicString<Ch> const & a, BasicString<Ch> const & b) noexcept { return a.compare(b) < 0; }

	template <class Ch
	> bool operator<(Ch const * const a, BasicString<Ch> const & b) { return b.compare(a) > 0; }

	template <class Ch
	> bool operator<(BasicString<Ch> const & a, Ch const * const b) { return a.compare(b) < 0; }

	template <class Ch
	> bool operator>(BasicString<Ch> const & a, BasicString<Ch> const & b) noexcept { return b < a; }

	template <class Ch
	> bool operator>(Ch const * const a, BasicString<Ch> const & b) { return b < a; }

	template <class Ch
	> bool operator>(BasicString<Ch> const & a, Ch const * const b) { return b < a; }

	template <class Ch
	> bool operator<=(BasicString<Ch> const & a, BasicString<Ch> const & b) noexcept { return !(b < a); }

	template <class Ch
	> bool operator<=(Ch const * const a, BasicString<Ch> const & b) { return !(b < a); }

	template <class Ch
	> bool operator<=(BasicString<Ch> const & a, Ch const * const b) { return !(b < a); }

	template <class Ch
	> bool operator>=(BasicString<Ch> const & a, BasicString<Ch> const & b) noexcept { return !(a < b); }

	template <class Ch
	> bool operator>=(Ch const * const a, BasicString<Ch> const & b) { return !(a < b); }

	template <class Ch
	> bool operator>=(BasicString<Ch> const & a, Ch const * const b) { return !(a < b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_HPP_
