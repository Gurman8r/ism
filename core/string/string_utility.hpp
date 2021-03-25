#ifndef _ISM_STRING_UTILITY_HPP_
#define _ISM_STRING_UTILITY_HPP_

#include <core/string/identifier.hpp>

#include <core/templates/optional.hpp>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch> using is_char_t = any_of<
		std::is_same<Ch, char>,
#if ISM_CXX_20
		std::is_same<Ch, char8_t>,
#endif
		std::is_same<Ch, char16_t>,
		std::is_same<Ch, char32_t>,
		std::is_same<Ch, wchar_t>
	>;

	template <class Ch
	> constexpr bool is_char_v{ is_char_t<Ch>::value };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class> constexpr bool is_string_v{ false };
	
	template <class Ch
	> constexpr bool is_string_v<Ch *>{ is_char_v<Ch> };
	
	template <class Ch
	> constexpr bool is_string_v<Ch const *>{ is_char_v<Ch> };

	template <class Ch, size_t N
	> constexpr bool is_string_v<Ch [N]>{ is_char_v<Ch> };

	template <class Ch, size_t N
	> constexpr bool is_string_v<Ch const [N]>{ is_char_v<Ch> };

	template <class Ch
	> constexpr bool is_string_v<BasicString<Ch>>{ true };

	template <class Ch
	> constexpr bool is_string_v<std::basic_string<Ch>>{ true };

	template <> constexpr bool is_string_v<Identifier>{ true };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class SFINAE = void> struct format_descriptor {};

	template <class T, class SFINAE = void> struct is_format_numeric { static constexpr bool value{}; };

	template <class T> struct is_format_numeric<T, std::enable_if_t<std::is_arithmetic_v<T>>>
	{
		static constexpr bool value{ true };

		static constexpr int32_t index
		{
			(std::is_same_v<T, bool>
			? 0
			: 1 + (std::is_integral_v<T>
				? log2(sizeof(T)) * 2 + std::is_unsigned_v<T>
				: 8 + (std::is_same_v<T, float64_t>
					? 1 : std::is_same_v<T, float80_t>
					? 2 : 0)))
		};
	};

	template <class T> struct format_descriptor<T, std::enable_if_t<std::is_arithmetic_v<T>>>
	{
		static constexpr char const c{ "?bBhHiIqQfdg"[is_format_numeric<T>::index] };
		static constexpr char const value[2] = { c, '\0' };
		static String format() { return String{ 1, c }; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> NODISCARD constexpr size_t strlen(Ch const * s)
	{
		return *s ? (1 + ISM::strlen(s + 1)) : 0;
	}

	template <class Ch = char
	> NODISCARD Ch * strdup(Ch const * src)
	{
		int32_t n{};
		while (src[n]) { n++; }
		Ch * temp{ (Ch *)memalloc(n + 1 * sizeof(Ch)) }, * ptr{ temp };
		while (*src) { *ptr++ = *src++; }
		*ptr = static_cast<Ch>(0);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> NODISCARD constexpr Ch const * single_str(Ch const * items_separated_by_zeroes, size_t index)
	{
		Ch const *	items	{ (Ch const *)items_separated_by_zeroes };
		Ch const *	ptr		{ items };
		size_t		count	{};
		while (*ptr)
		{
			if (index == count) { break; }
			ptr += ISM::strlen(ptr) + 1;
			count++;
		}
		return *ptr ? ptr : (Ch const *)nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Fn, class ... Args
	> NODISCARD Optl<T> parse_answer(cstring ptr, Fn && fn, Args && ... args) noexcept
	{
		// from <string>

		char * end{};
		auto const answer{ FWD(fn)(ptr, &end, FWD(args)...) };
		if (!(*end != 0 || end == ptr))
		{
			return static_cast<T>(answer);
		}
		else
		{
			return std::nullopt;
		}
	}

	template <class T
	> NODISCARD String integral_to_string(T const value) noexcept
	{
		// from <string>

		static_assert(std::is_integral_v<T>);

		using U = typename std::make_unsigned_t<T>;

		char			buf[21]	{};
		char * const	end		{ std::end(buf) };
		char *			next	{ end };
		auto const		uval	{ static_cast<U>(value) };

		auto uint_to_string = [](char * next, auto uval) noexcept
		{
			if constexpr (sizeof(U) > 4)
			{
				while (uval > 0xFFFFFFFFU)
				{
					auto chunk{ static_cast<ulong_t>(uval % 1000000000) };
					uval /= 1000000000;
					for (int32_t_t i = 0; i != 9; ++i) {
						*--next = static_cast<char>('0' + chunk % 10);
						chunk /= 10;
					}
				}
			}
			auto trunc{ static_cast<ulong_t>(uval) };
			do {
				*--next = static_cast<char>('0' + trunc % 10);
				trunc /= 10;
			} while (trunc != 0);
			return next;
		};

		if (value < 0)
		{
			next = uint_to_string(next, 0 - uval);
			*--next = '-';
		}
		else
		{
			next = uint_to_string(next, uval);
		}
		
		return String{ next, end };
	}

	template <class T
	> NODISCARD String floating_point_to_string(T const value) noexcept
	{
		// from <string>

		static_assert(std::is_floating_point_v<T>);
		auto const len{ static_cast<size_t>(_CSTD _scprintf("%f", value)) };
		String temp{ len, 0, String::allocator_type{} };
		_CSTD sprintf_s(temp.data(), len + 1, "%f", value);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> NODISCARD auto widen(BasicString<Ch> const & value) noexcept
	{
		if constexpr (std::is_convertible_v<BasicString<Ch>, WideString>)
		{
			return (WideString)value;
		}
		else
		{
			WideString temp{};
			temp.reserve(value.size());
			for (Ch c : value) { temp.push_back(static_cast<wchar_t>(c)); }
			return temp;
		}
	}

	template <class Ch = wchar_t
	> NODISCARD auto narrow(BasicString<Ch> const & value) noexcept
	{
		if constexpr (std::is_convertible_v<BasicString<Ch>, String>)
		{
			return (String)value;
		}
		else
		{
			String temp{};
			temp.reserve(value.size());
			for (Ch c : value) { temp.push_back(static_cast<char>(c)); }
			return temp;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> NODISCARD BasicString<Ch> to_lower(BasicString<Ch> value) noexcept
	{
		for (Ch & elem : value) { elem = std::tolower(elem, {}); }
		return value;
	}

	template <class Ch = char
	> NODISCARD BasicString<Ch> to_upper(BasicString<Ch> value) noexcept
	{
		for (Ch & elem : value) { elem = std::toupper(elem, {}); }
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> NODISCARD Optl<bool> to_bool(BasicString<Ch> const & value) noexcept
	{
		switch (hashof(ISM::to_lower(value)))
		{
		default				: return std::nullopt;
		case hashof("1")	:
		case hashof("true")	:
		case hashof("on")	:
		case hashof("yes")	: return make_optl(true);
		case hashof("0")	:
		case hashof("false"):
		case hashof("off")	:
		case hashof("no")	: return make_optl(false);
		}
	}

	template <class Ch = char
	> NODISCARD Optl<int8_t> to_i8(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<int8_t>(value.c_str(), &_CSTD strtol, base);
	}

	template <class Ch = char
	> NODISCARD Optl<int16_t> to_i16(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<int16_t>(value.c_str(), &_CSTD strtol, base);
	}

	template <class Ch = char
	> NODISCARD Optl<int32_t> to_i32(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<int32_t>(value.c_str(), &_CSTD strtol, base);
	}

	template <class Ch = char
	> NODISCARD Optl<int64_t> to_i64(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<int64_t>(value.c_str(), &_CSTD strtoll, base);
	}

	template <class Ch = char
	> NODISCARD Optl<uint8_t> to_u8(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<uint8_t>(value.c_str(), &_CSTD strtoul, base);
	}

	template <class Ch = char
	> NODISCARD Optl<uint16_t> to_u16(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<uint16_t>(value.c_str(), &_CSTD strtoul, base);
	}

	template <class Ch = char
	> NODISCARD Optl<uint32_t> to_u32(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<uint32_t>(value.c_str(), &_CSTD strtoul, base);
	}

	template <class Ch = char
	> NODISCARD Optl<uint64_t> to_u64(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return ISM::parse_answer<uint64_t>(value.c_str(), &_CSTD strtoull, base);
	}

	template <class Ch = char
	> NODISCARD Optl<float32_t> to_f32(BasicString<Ch> const & value) noexcept
	{
		return ISM::parse_answer<float32_t>(value.c_str(), &_CSTD strtod);
	}

	template <class Ch = char
	> NODISCARD Optl<float64_t> to_f64(BasicString<Ch> const & value) noexcept
	{
		return ISM::parse_answer<float64_t>(value.c_str(), &_CSTD strtod);
	}

	template <class Ch = char
	> NODISCARD Optl<float80_t> to_f80(BasicString<Ch> const & value) noexcept
	{
		return ISM::parse_answer<float80_t>(value.c_str(), &_CSTD strtold);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char
	> NODISCARD BasicString<Ch> to_string(T && value) noexcept
	{
		BasicStringStream<Ch> ss{};
		ss << FWD(value);
		return ss.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_UTILITY_HPP_
