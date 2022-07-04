#ifndef _ISM_STRING_VIEW_HPP_
#define _ISM_STRING_VIEW_HPP_

#include <core/templates/utility.hpp>

#include <string_view>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace util
	{
		template <class Ch = char
		> constexpr size_t strlen(Ch const * s)
		{
			return *s ? (1 + strlen(s + 1)) : 0;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string view base
	template <class Ch
	> ALIAS(_StringViewBase) std::basic_string_view<Ch, std::char_traits<Ch>>;

	// basic string view
	template <class Ch
	> class BasicStringView : public _StringViewBase<Ch>
	{
	public:
		using base_type = _StringViewBase<Ch>;
		using base_type::base_type;
		using base_type::operator=;
		constexpr auto hash_code() const noexcept { return ism::hash(data(), size()); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string view
	ALIAS(StringView) BasicStringView<char>;

	// wide string view
	ALIAS(WideStringView) BasicStringView<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl
	{
		template <class Ch> constexpr bool is_string_v<BasicStringView<Ch>>{ true };
		template <> constexpr bool is_string_v<StringView>{ true };
		template <> constexpr bool is_string_v<WideStringView>{ true };
	}

	constexpr StringView operator "" _sv(cstring s, size_t n) noexcept { return StringView{ s, n }; }

	constexpr WideStringView operator "" _sv(cwstring s, size_t n) noexcept { return WideStringView{ s, n }; }

	template <class Ch
	> struct Hasher<BasicStringView<Ch>>
	{
		hash_t operator()(BasicStringView<Ch> const s) const noexcept { return s.hash_code(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_VIEW_HPP_
