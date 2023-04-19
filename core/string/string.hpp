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
	> using _StringBase = std::basic_string<C, std::char_traits<C>, PolymorphicAllocator<C>>;

	// basic string
	template <class C = char
	> class BasicString : public _StringBase<C>
	{
	public:
		using self_type = BasicString<C>;
		using base_type = _StringBase<C>;
		using base_type::base_type;
		using base_type::operator=;

		using typename base_type::traits_type;
		using typename base_type::difference_type;
		using typename base_type::size_type;
		using typename base_type::iterator;
		using typename base_type::const_iterator;
		using typename base_type::reverse_iterator;
		using typename base_type::const_reverse_iterator;

	public:
		static constexpr bool is_narrow{ 1 == sizeof(C) };
		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }
		NODISCARD auto hash_code() const noexcept { return hash_representation(data(), size()); }
		NODISCARD auto view() const noexcept { return BasicStringView<C>{ data(), size() }; }
		NODISCARD operator BasicStringView<C>() const noexcept { return { data(), size() }; }

	public:
		template <class = std::enable_if_t<!is_narrow>
		> auto narrow() const {
			BasicString<char> temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(static_cast<char>(c)); }
			return temp;
		}

		template <class = std::enable_if_t<is_narrow>
		> auto widen() const {
			BasicString<wchar_t> temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(static_cast<wchar_t>(c)); }
			return temp;
		}
		
		auto to_upper() const {
			self_type temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(std::toupper(c)); }
			return temp;
		}

		auto to_lower() const {
			self_type temp;
			temp.reserve(size());
			for (auto const c : *this) { temp.push_back(std::tolower(c)); }
			return temp;
		}

		auto & make_upper() {
			return std::transform(begin(), end(), begin(), std::toupper), (*this);
		}

		auto & make_lower() {
			return std::transform(begin(), end(), begin(), std::tolower), (*this);
		}

		auto & trim_back() noexcept {
			while (!empty() && std::isspace(back())) { pop_back(); }
			return (*this);
		}

		auto & trim_front() noexcept {
			while (!empty() && std::isspace(front())) { erase(begin()); }
			return (*this);
		}

		auto & trim() noexcept {
			while (!empty() && std::isspace(back())) { pop_back(); }
			while (!empty() && std::isspace(front())) { erase(begin()); }
			return (*this);
		}
		
		auto & erase_duplicates(C const c) {
			return erase(std::unique(begin(), end(), [c](C a, C b) { return (a == b) && (a == c); }), end()), (*this);
		}

	public:
		template <size_t buffer_size = 0
		> static i32 format(self_type & s, cstring fmt, va_list args)
		{
			if constexpr (0 < buffer_size)
			{
				C buffer[buffer_size]{};
				i32 const n{ std::vsnprintf(buffer, buffer_size, fmt, args) };
				if (0 < n) {
					s = self_type{ buffer, buffer + (size_t)n, allocator_type{} };
				}
				return n;
			}
			else
			{
				va_list args_copy;
				va_copy(args_copy, args);
				i32 const n{ std::vsnprintf(nullptr, 0, fmt, args_copy) };
				va_end(args_copy);
				if (0 < n) {
					s.resize((size_t)n);
					std::vsnprintf(s.data(), s.size(), fmt, args);
				}
				return n;
			}
		}

		template <size_t buffer_size = 0
		> static i32 format(self_type & s, cstring fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			i32 const n{ format<buffer_size>(s, fmt, args) };
			va_end(args);
			return n;
		}

		template <size_t buffer_size = 0
		> NODISCARD static self_type format(cstring fmt, va_list args) noexcept {
			self_type s;
			format<buffer_size>(s, fmt, args);
			return s;
		}

		template <size_t buffer_size = 0
		> NODISCARD static self_type format(cstring fmt, ...) noexcept {
			va_list args;
			va_start(args, fmt);
			auto const s{ format<buffer_size>(fmt, args) };
			va_end(args);
			return s;
		}

	public:
		self_type & operator+=(self_type const & value) { return base_type::operator+=((base_type const &)value), (*this); }
		self_type & operator+=(self_type && value) noexcept { return base_type::operator+=(std::move((base_type &&)value)), (*this); }
		template <class T> self_type & operator+=(T const & value) { return base_type::operator+=(value), (*this); }
		template <class T> self_type & operator+=(T && value) noexcept { return base_type::operator+=(value), (*this); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// string
	using String = BasicString<char>;

	// unicode
	using Unicode = BasicString<wchar_t>;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <class C> constexpr bool is_string_v<BasicString<C>>{ true }; }

	inline String operator "" _s(cstring s, size_t n) noexcept { return String{ s, n }; }

	inline Unicode operator "" _s(cwstring s, size_t n) noexcept { return Unicode{ s, n }; }

	template <class C> struct Hasher<BasicString<C>> { size_t operator()(BasicString<C> const & s) const noexcept { return s.hash_code(); } };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> const & b) {
		const auto a_size = a.size();
		const auto b_size = b.size();
		if (a.max_size() - a_size < b_size) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> const & b) {
		using _Traits = typename BasicString<C>::traits_type;
		using _Size_type = typename BasicString<C>::size_type;
		const auto a_size = std::_Convert_size<_Size_type>(_Traits::length(a));
		const auto b_size = b.size();
		if (b.max_size() - b_size < a_size) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, b, a, a_size, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const a, BasicString<C> const & b) {
		const auto b_size = b.size();
		if (b_size == b.max_size()) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, b, std::addressof(a), 1, b.c_str(), b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const * const b) {
		using _Traits = typename BasicString<C>::traits_type;
		using _Size_type = typename BasicString<C>::size_type;
		const auto a_size = a.size();
		const auto b_size = std::_Convert_size<_Size_type>(_Traits::length(b));
		if (a.max_size() - a_size < b_size) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, b, b_size };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, C const b) {
		const auto a_size = a.size();
		if (a_size == a.max_size()) {
			std::_Xlen_string();
		}
		return { std::_String_constructor_concat_tag{}, a, a.c_str(), a_size, std::addressof(b), 1 };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> const & a, BasicString<C> && b) {
		return b.insert(0, a), std::move(b);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> const & b) {
		return a.append(b), std::move(a);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, BasicString<C> && b) {
#if _ITERATOR_DEBUG_LEVEL == 2
		_STL_VERIFY(std::addressof(a) != std::addressof(b),
			"You cannot concatenate the same moved string to itself. See N4910 16.4.5.9 [res.on.arguments]/1.3: "
			"If a function argument is bound to an rvalue reference parameter, the implementation may assume that "
			"this parameter is a unique reference to this argument, except that the argument passed to "
			"a move-assignment operator may be a reference to *this (16.4.6.15 [lib.types.movedfrom]).");
#endif // _ITERATOR_DEBUG_LEVEL == 2
		return { std::_String_constructor_concat_tag{}, a, b };
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const * const a, BasicString<C> && b) {
		return b.insert(0, a), std::move(b);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(C const a, BasicString<C> && b) {
		return b.insert(0, 1, a), std::move(b);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const * const b) {
		return a.append(b), std::move(a);
	}

	template <class C
	> NODISCARD BasicString<C> operator+(BasicString<C> && a, C const b) {
		return a.push_back(b), std::move(a);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class C> NODISCARD bool operator==(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a._Equal(b); }
	template <class C> NODISCARD bool operator==(BasicString<C> const & a, C const * const b) { return a._Equal(b); }
	template <class C> NODISCARD bool operator==(C const * const a, BasicString<C> const & b) { return b._Equal(a); }

	template <class C> NODISCARD bool operator!=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a == b); }
	template <class C> NODISCARD bool operator!=(C const * const a, BasicString<C> const & b) { return !(a == b); }
	template <class C> NODISCARD bool operator!=(BasicString<C> const & a, C const * const b) { return !(a == b); }

	template <class C> NODISCARD bool operator<(BasicString<C> const & a, BasicString<C> const & b) noexcept { return a.compare(b) < 0; }
	template <class C> NODISCARD bool operator<(C const * const a, BasicString<C> const & b) { return b.compare(a) > 0; }
	template <class C> NODISCARD bool operator<(BasicString<C> const & a, C const * const b) { return a.compare(b) < 0; }

	template <class C> NODISCARD bool operator>(BasicString<C> const & a, BasicString<C> const & b) noexcept { return b < a; }
	template <class C> NODISCARD bool operator>(C const * const a, BasicString<C> const & b) { return b < a; }
	template <class C> NODISCARD bool operator>(BasicString<C> const & a, C const * const b) { return b < a; }

	template <class C> NODISCARD bool operator<=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(b < a); }
	template <class C> NODISCARD bool operator<=(C const * const a, BasicString<C> const & b) { return !(b < a); }
	template <class C> NODISCARD bool operator<=(BasicString<C> const & a, C const * const b) { return !(b < a); }

	template <class C> NODISCARD bool operator>=(BasicString<C> const & a, BasicString<C> const & b) noexcept { return !(a < b); }
	template <class C> NODISCARD bool operator>=(C const * const a, BasicString<C> const & b) { return !(a < b); }
	template <class C> NODISCARD bool operator>=(BasicString<C> const & a, C const * const b) { return !(a < b); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_HPP_
