#ifndef _ISM_STRING_UTILITY_HPP_
#define _ISM_STRING_UTILITY_HPP_

#include <core/string/path.hpp>

#include <core/templates/optional.hpp>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> constexpr size_t strlen(Ch const * s)
	{
		return *s ? (1 + strlen(s + 1)) : 0;
	}

	template <class Ch = char
	> Ch * strdup(Ch const * src)
	{
		int32_t n{};
		while (src[n]) { n++; }
		Ch * temp{ (Ch *)memalloc(n + 1 * sizeof(Ch)) }, * ptr{ temp };
		while (*src) { *ptr++ = *src++; }
		*ptr = static_cast<Ch>(0);
		return temp;
	}

	template <class Ch = char
	> BasicString<Ch> & trim(BasicString<Ch> & s) noexcept
	{
		while (!s.empty() && std::isspace(s.back())) { s.pop_back(); }
		while (!s.empty() && std::isspace(s.front())) { s.erase(s.begin()); }
		return s;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> constexpr Ch const * single_str(Ch const * items_separated_by_zeroes, size_t index)
	{
		Ch const * items{ (Ch const *)items_separated_by_zeroes };
		Ch const * ptr{ items };
		size_t count{};
		while (*ptr) {
			if (index == count) { break; }
			ptr += strlen(ptr) + 1;
			count++;
		}
		return *ptr ? ptr : (Ch const *)nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Fn, class ... Args
	> Optional<T> parse_answer(cstring ptr, Fn && fn, Args && ... args) noexcept
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
	> String integral_to_string(T const value) noexcept
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
					for (int32_t i = 0; i != 9; ++i) {
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
	> String floating_point_to_string(T const value) noexcept
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
	> WideString widen(BasicString<Ch> const & value) noexcept
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
	> String narrow(BasicString<Ch> const & value) noexcept
	{
		if constexpr (std::is_convertible_v<BasicString<Ch>, String>)
		{
			return (String)value;
		}
		else
		{
			String temp{};
			temp.reserve(value.size());
			for (Ch const c : value) { temp.push_back(static_cast<char>(c)); }
			return temp;
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Str = BasicString<char>
	> Str to_lower(Str value) {
		for (auto & elem : value) { elem = std::tolower(elem); }
		return value;
	}

	template <class Str = BasicString<char>
	> Str to_upper(Str value) {
		for (auto & elem : value) { elem = std::toupper(elem); }
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class Ch = char
	> Optional<bool> to_bool(BasicString<Ch> const & value) noexcept
	{
		switch (hash(to_lower(value)))
		{
		default				: return nullopt;
		case "1"_hash		:
		case "true"_hash	:
		case "on"_hash		:
		case "yes"_hash		: return make_optional(true);
		case "0"_hash		:
		case "false"_hash	:
		case "off"_hash		:
		case "no"_hash		: return make_optional(false);
		}
	}

	template <class Ch = char
	> Optional<int8_t> to_i8(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<int8_t>(value.c_str(), &_CSTD strtol, base);
	}

	template <class Ch = char
	> Optional<int16_t> to_i16(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<int16_t>(value.c_str(), &_CSTD strtol, base);
	}

	template <class Ch = char
	> Optional<int32_t> to_i32(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<int32_t>(value.c_str(), &_CSTD strtol, base);
	}

	template <class Ch = char
	> Optional<int64_t> to_i64(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<int64_t>(value.c_str(), &_CSTD strtoll, base);
	}

	template <class Ch = char
	> Optional<uint8_t> to_u8(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<uint8_t>(value.c_str(), &_CSTD strtoul, base);
	}

	template <class Ch = char
	> Optional<uint16_t> to_u16(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<uint16_t>(value.c_str(), &_CSTD strtoul, base);
	}

	template <class Ch = char
	> Optional<uint32_t> to_u32(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<uint32_t>(value.c_str(), &_CSTD strtoul, base);
	}

	template <class Ch = char
	> Optional<uint64_t> to_u64(BasicString<Ch> const & value, int32_t base = 10) noexcept
	{
		return parse_answer<uint64_t>(value.c_str(), &_CSTD strtoull, base);
	}

	template <class Ch = char
	> Optional<float32_t> to_f32(BasicString<Ch> const & value) noexcept
	{
		return parse_answer<float32_t>(value.c_str(), &_CSTD strtod);
	}

	template <class Ch = char
	> Optional<float64_t> to_f64(BasicString<Ch> const & value) noexcept
	{
		return parse_answer<float64_t>(value.c_str(), &_CSTD strtod);
	}

	template <class Ch = char
	> Optional<float80_t> to_f80(BasicString<Ch> const & value) noexcept
	{
		return parse_answer<float80_t>(value.c_str(), &_CSTD strtold);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T, class Ch = char, std::enable_if_t<mpl::is_string_v<T>, int> = 0
	> auto to_string(T && value) noexcept -> BasicString<Ch>
	{
		return BasicString<Ch>{ FWD(value) };
	}

	template <class T, class Ch = char, std::enable_if_t<!mpl::is_string_v<T>, int> = 0
	> auto to_string(T && value) noexcept -> BasicString<Ch>
	{
		BasicStringStream<Ch> ss{};
		ss << FWD(value);
		return ss.str();
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr StringView operator "" _sv(cstring str, size_t len) noexcept { return StringView{ str, len }; }

	inline String operator "" _s(cstring str, size_t len) noexcept { return String{ str, len }; }

	inline WideString operator "" _s(cwstring str, size_t len) noexcept { return WideString{ str, len }; }

	inline StringName operator "" _name(cstring str, size_t len) noexcept { return StringName{ String{ str, len } }; }

	inline Path operator "" _path(cstring str, size_t len) noexcept { return Path{ String{ str, len } }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_UTILITY_HPP_
