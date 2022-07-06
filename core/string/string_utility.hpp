#ifndef _ISM_STRING_UTILITY_HPP_
#define _ISM_STRING_UTILITY_HPP_

#include <core/templates/optional.hpp>
#include <core/string/string_stream.hpp>
#include <core/string/string_name.hpp>
#include <core/string/path.hpp>

namespace ism::util
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class S = String
	> S to_lower(S value)
	{
		for (auto & c : value) { c = std::tolower(c); }
		return value;
	}

	template <class S = String
	> S to_upper(S value)
	{
		for (auto & c : value) { c = std::toupper(c); }
		return value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline String narrow(WideString const & value)
	{
		String temp{};
		temp.reserve(value.size());
		for (auto const c : value) { temp.push_back(static_cast<char>(c)); }
		return temp;
	}

	inline WideString widen(String const & value)
	{
		WideString temp{};
		temp.reserve(value.size());
		for (auto const c : value) { temp.push_back(static_cast<wchar_t>(c)); }
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

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

	template <class T, class Fn, class ... Args
	> Optional<T> parse_answer(cstring ptr, Fn && fn, Args && ... args) noexcept
	{
		char * end{};
		auto const answer{ FWD(fn)(ptr, &end, FWD(args)...) };
		if (!(*end != 0 || end == ptr)) {
			return static_cast<T>(answer);
		}
		else {
			return std::nullopt;
		}
	}

	template <class T
	> char * uintegral_to_buffer(char * next, T value) noexcept
	{
		if constexpr (sizeof(T) > 4)
		{
			while (value > 0xFFFFFFFFU) {
				auto chunk{ static_cast<ulong_t>(value % 1000000000) };
				value /= 1000000000;
				for (int32_t i = 0; i != 9; ++i) {
					*--next = static_cast<char>('0' + chunk % 10);
					chunk /= 10;
				}
			}
		}
		auto trunc{ static_cast<ulong_t>(value) };
		do {
			*--next = static_cast<char>('0' + trunc % 10);
			trunc /= 10;
		} while (trunc != 0);
		return next;
	}

	template <class T
	> String integral_to_string(T value) noexcept
	{
		static_assert(std::is_integral_v<T>);
		char			buf[21]	{ /* can hold 2^64 - 1, plus NUL */ };
		char * const	end		{ std::end(buf) };
		char *			next	{ end };
		auto const		uval	{ static_cast<std::make_unsigned_t<T>>(value) };
		if (value < 0) {
			next = uintegral_to_buffer(next, 0 - uval);
			*--next = '-';
		}
		else {
			next = uintegral_to_buffer(next, uval);
		}
		return String{ next, end };
	}

	template <class T
	> String uintegral_to_string(T value) noexcept
	{
		static_assert(std::is_integral_v<T>, "T must be integral");
		static_assert(std::is_unsigned_v<T>, "T must be unsigned");
		char buf[21]{ /* can hold 2^64 - 1, plus NUL */ };
		char * const end{ std::end(buf) };
		char * const next{ uintegral_to_buffer(end, value) };
		return String{ next, end };
	}

	template <class T
	> String floating_point_to_string(T value) noexcept
	{
		static_assert(std::is_floating_point_v<T>);
		auto const n{ static_cast<size_t>(_CSTD _scprintf("%f", value)) };
		String temp{ n, '\0', String::allocator_type{} };
		_CSTD sprintf_s(temp.data(), n + 1, "%f", value);
		return temp;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline Optional<int8_t> to_i8(String const & value, int32_t base = 10) noexcept { return parse_answer<int8_t>(value.c_str(), &_CSTD strtol, base); }

	inline Optional<int16_t> to_i16(String const & value, int32_t base = 10) noexcept { return parse_answer<int16_t>(value.c_str(), &_CSTD strtol, base); }

	inline Optional<int32_t> to_i32(String const & value, int32_t base = 10) noexcept { return parse_answer<int32_t>(value.c_str(), &_CSTD strtol, base); }

	inline Optional<int64_t> to_i64(String const & value, int32_t base = 10) noexcept { return parse_answer<int64_t>(value.c_str(), &_CSTD strtoll, base); }

	inline Optional<uint8_t> to_u8(String const & value, int32_t base = 10) noexcept { return parse_answer<uint8_t>(value.c_str(), &_CSTD strtoul, base); }

	inline Optional<uint16_t> to_u16(String const & value, int32_t base = 10) noexcept { return parse_answer<uint16_t>(value.c_str(), &_CSTD strtoul, base); }

	inline Optional<uint32_t> to_u32(String const & value, int32_t base = 10) noexcept { return parse_answer<uint32_t>(value.c_str(), &_CSTD strtoul, base); }

	inline Optional<uint64_t> to_u64(String const & value, int32_t base = 10) noexcept { return parse_answer<uint64_t>(value.c_str(), &_CSTD strtoull, base); }

	inline Optional<float32_t> to_f32(String const & value) noexcept { return parse_answer<float32_t>(value.c_str(), &_CSTD strtod); }

	inline Optional<float64_t> to_f64(String const & value) noexcept { return parse_answer<float64_t>(value.c_str(), &_CSTD strtod); }

	inline Optional<float80_t> to_f80(String const & value) noexcept { return parse_answer<float80_t>(value.c_str(), &_CSTD strtold); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline String to_string(int8_t value) noexcept { return integral_to_string(value); }

	inline String to_string(int16_t value) noexcept { return integral_to_string(value); }

	inline String to_string(int32_t value) noexcept { return integral_to_string(value); }

	inline String to_string(int64_t value) noexcept { return integral_to_string(value); }

	inline String to_string(uint8_t value) noexcept { return uintegral_to_string(value); }

	inline String to_string(uint16_t value) noexcept { return uintegral_to_string(value); }

	inline String to_string(uint32_t value) noexcept { return uintegral_to_string(value); }

	inline String to_string(uint64_t value) noexcept { return uintegral_to_string(value); }

	inline String to_string(float32_t value) noexcept { return floating_point_to_string(value); }

	inline String to_string(float64_t value) noexcept { return floating_point_to_string(value); }

	inline String to_string(float80_t value) noexcept { return floating_point_to_string(value); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_UTILITY_HPP_
