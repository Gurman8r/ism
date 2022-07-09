#ifndef _ISM_STRING_VIEW_HPP_
#define _ISM_STRING_VIEW_HPP_

#include <core/templates/utility.hpp>

#include <string_view>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C = char
	> constexpr size_t strlen(C const * s)
	{
		return *s ? (1 + strlen(s + 1)) : 0;
	}

	constexpr cstring single_str(cstring items_separated_by_zeroes, size_t index)
	{
		cstring items{ (cstring)items_separated_by_zeroes };
		cstring ptr{ items };
		size_t count{};
		while (*ptr) {
			if (index == count) { break; }
			ptr += strlen(ptr) + 1;
			count++;
		}
		return *ptr ? ptr : (cstring)nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string view base
	template <class C
	> ALIAS(_StringViewBase) std::basic_string_view<C, std::char_traits<C>>;

	// basic string view
	template <class C
	> class BasicStringView : public _StringViewBase<C>
	{
	public:
		using base_type = _StringViewBase<C>;
		using base_type::base_type;
		using base_type::operator=;

	public:
		constexpr auto hash_code() const noexcept { return ism::hash(data(), size()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string view
	ALIAS(StringView) BasicStringView<char>;

	// wide string view
	ALIAS(WideStringView) BasicStringView<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <class C> constexpr bool is_string_v<BasicStringView<C>>{ true }; }

	constexpr StringView operator "" _sv(cstring s, size_t n) noexcept { return StringView{ s, n }; }

	constexpr WideStringView operator "" _sv(cwstring s, size_t n) noexcept { return WideStringView{ s, n }; }

	template <class C
	> struct Hasher<BasicStringView<C>>
	{
		hash_t operator()(BasicStringView<C> const s) const noexcept { return s.hash_code(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_VIEW_HPP_
