#ifndef _ISM_STRING_VIEW_HPP_
#define _ISM_STRING_VIEW_HPP_

#include <core/string/string.hpp>

#include <string_view>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class StringView
	{
	public:
		using storage_type				= typename std::string_view;
		using value_type				= typename storage_type::value_type;
		using pointer					= typename storage_type::pointer;
		using const_pointer				= typename storage_type::const_pointer;
		using reference					= typename storage_type::reference;
		using const_reference			= typename storage_type::const_reference;
		using iterator					= typename storage_type::iterator;
		using const_iterator			= typename storage_type::const_iterator;
		using reverse_iterator			= typename storage_type::reverse_iterator;
		using const_reverse_iterator	= typename storage_type::const_reverse_iterator;
		using size_type					= typename storage_type::size_type;
		using difference_type			= typename storage_type::difference_type;

		static constexpr auto npos{ static_cast<size_type>(-1) };

	public:
		constexpr StringView() noexcept = default;
		constexpr StringView(StringView const &) noexcept = default;
		constexpr StringView & operator=(StringView const & other) noexcept = default;
		constexpr StringView(const_pointer ptr, size_type count) : m_text{ ptr, count } {}
		constexpr StringView(storage_type const & value) : m_text{ value } {}

	public:
		constexpr auto data() const noexcept -> const_pointer { return m_text.data(); }
		constexpr bool empty() const noexcept { return m_text.empty(); }
		constexpr auto length() const noexcept -> size_type { return m_text.length(); }
		constexpr auto size() const noexcept -> size_type { return m_text.size(); }
		constexpr auto hash_code() const noexcept -> hash_t { return hash(m_text.data(), m_text.size()); }
		constexpr auto max_size() const noexcept -> size_type { return m_text.max_size(); }

		constexpr auto operator[](size_type i) const noexcept { return m_text[i]; }
		String string() const noexcept { return String{ begin(), end() }; }
		operator String() const noexcept { return String{ begin(), end() }; }

	public:
		constexpr auto at(size_type i) const noexcept -> const_reference { return m_text.at(i); }
		constexpr auto front() const noexcept -> const_reference { return m_text.front(); }
		constexpr auto back() const noexcept -> const_reference { return m_text.back(); }

		constexpr StringView & remove_prefix(size_type count) noexcept { m_text.remove_prefix(count); return (*this); }
		constexpr StringView & remove_suffix(size_type count) noexcept { m_text.remove_suffix(count); return (*this); }

		auto copy(pointer ptr, size_type count, size_type off = 0) -> size_type { return m_text.copy(ptr, count, off); }
		constexpr void swap(StringView & other) noexcept { m_text.swap(other.m_text); }
		constexpr StringView substr(size_type off = 0, size_type count = npos) const { return StringView{ m_text.substr(off, npos) }; }

	public:
		constexpr auto compare(StringView const & other) const noexcept { return m_text.compare(other.m_text); }
		constexpr auto compare(size_type off, size_type nx, StringView const & other) const noexcept { return m_text.compare(off, nx, other.m_text); }
		constexpr auto compare(size_type off, size_type nx, StringView const & other, size_type roff, size_type count) const noexcept { return m_text.compare(off, nx, other.m_text, roff, count); }
		constexpr auto compare(const_pointer value) const noexcept { return m_text.compare(value); }

	public:
		template <class U = StringView
		> constexpr bool operator==(U && other) const noexcept { return this->compare(StringView{ FWD(other) }) == 0; }

		template <class U = StringView
		> constexpr bool operator!=(U && other) const noexcept { return this->compare(StringView{ FWD(other) }) != 0; }

		template <class U = StringView
		> constexpr bool operator<(U && other) const noexcept { return this->compare(StringView{ FWD(other) }) < 0; }

		template <class U = StringView
		> constexpr bool operator>(U && other) const noexcept { return this->compare(StringView{ FWD(other) }) > 0; }

		template <class U = StringView
		> constexpr bool operator<=(U && other) const noexcept { return this->compare(StringView{ FWD(other) }) <= 0; }

		template <class U = StringView
		> constexpr bool operator>=(U && other) const noexcept { return this->compare(StringView{ FWD(other) }) >= 0; }

	public:
		constexpr auto find(StringView const & other, size_type off = 0) const noexcept -> size_type { return m_text.find(other.m_text, off); }
		constexpr auto find(value_type c, size_type off = 0) const noexcept -> size_type { return m_text.find(c, off); }
		constexpr auto find(const_pointer ptr, size_type off, size_type count) const noexcept -> size_type { return m_text.find(ptr, off, count); }
		constexpr auto find(const_pointer ptr, size_type off = 0) const noexcept -> size_type { return m_text.find(ptr, off); }
		constexpr auto rfind(StringView const & other, size_type off = 0) const noexcept -> size_type { return m_text.rfind(other.m_text, off); }
		constexpr auto rfind(value_type c, size_type off = 0) const noexcept -> size_type { return m_text.rfind(c, off); }
		constexpr auto rfind(const_pointer ptr, size_type off, size_type count) const noexcept -> size_type { return m_text.rfind(ptr, off, count); }
		constexpr auto rfind(const_pointer ptr, size_type off = 0) const noexcept -> size_type { return m_text.rfind(ptr, off); }
		constexpr auto find_first_of(StringView other, size_type off = 0) const noexcept -> size_type { return m_text.find_first_of(other.m_text, off); }
		constexpr auto find_first_of(value_type c, size_type off = 0) const noexcept -> size_type { return m_text.find_first_of(c, off); }
		constexpr auto find_first_of(pointer ptr, size_type off, size_type count) const noexcept -> size_type { return m_text.find_first_of(ptr, off, count); }
		constexpr auto find_first_of(pointer ptr, size_type off = 0) const noexcept -> size_type { return m_text.find_first_of(ptr, off); }
		constexpr auto find_last_of(StringView other, size_type off = npos) const noexcept -> size_type { return m_text.find_last_of(other.m_text, off); }
		constexpr auto find_last_of(value_type c, size_type off = npos) const noexcept -> size_type { return m_text.find_last_of(c, off); }
		constexpr auto find_last_of(pointer ptr, size_type off, size_type count) const noexcept -> size_type { return m_text.find_last_of(ptr, off, count); }
		constexpr auto find_last_of(pointer ptr, size_type off = npos) const noexcept -> size_type { return m_text.find_last_of(ptr, off); }
		constexpr auto find_first_not_of(StringView other, size_type off = 0) const noexcept -> size_type { return m_text.find_first_not_of(other.m_text, off); }
		constexpr auto find_first_not_of(value_type c, size_type off = 0) const noexcept -> size_type { return m_text.find_first_not_of(c, off); }
		constexpr auto find_first_not_of(pointer ptr, size_type off, size_type count) const noexcept -> size_type { return m_text.find_first_not_of(ptr, off, count); }
		constexpr auto find_first_not_of(pointer ptr, size_type off = 0) const noexcept -> size_type { return m_text.find_first_not_of(ptr, off); }
		constexpr auto find_last_not_of(StringView other, size_type off = npos) const noexcept -> size_type { return m_text.find_last_not_of(other.m_text, off); }
		constexpr auto find_last_not_of(value_type c, size_type off = npos) const noexcept -> size_type { return m_text.find_last_not_of(c, off); }
		constexpr auto find_last_not_of(pointer ptr, size_type off, size_type count) const noexcept -> size_type { return m_text.find_last_not_of(ptr, off, count); }
		constexpr auto find_last_not_of(pointer ptr, size_type off = npos) const noexcept -> size_type { return m_text.find_last_not_of(ptr, off); }

	public:
		constexpr auto begin() noexcept -> iterator { return m_text.begin(); }
		constexpr auto begin() const noexcept -> const_iterator { return m_text.begin(); }
		constexpr auto cbegin() const noexcept -> const_iterator { return m_text.cbegin(); }

		constexpr auto end() noexcept -> iterator { return m_text.end(); }
		constexpr auto end() const noexcept -> const_iterator { return m_text.end(); }
		constexpr auto cend() const noexcept -> const_iterator { return m_text.cend(); }

		constexpr auto rbegin() noexcept -> reverse_iterator { return m_text.rbegin(); }
		constexpr auto rbegin() const noexcept -> const_reverse_iterator { return m_text.rbegin(); }
		constexpr auto crbegin() const noexcept -> const_reverse_iterator { return m_text.crbegin(); }

		constexpr auto rend() noexcept -> reverse_iterator { return m_text.rend(); }
		constexpr auto rend() const noexcept -> const_reverse_iterator { return m_text.rend(); }
		constexpr auto crend() const noexcept -> const_reverse_iterator { return m_text.crend(); }

	private:
		storage_type m_text;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	constexpr StringView operator "" _sv(cstring s, size_t n) noexcept { return StringView{ s, n }; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct Hasher<StringView>
	{
		hash_t operator()(StringView const s) const noexcept { return s.hash_code(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

template <> constexpr bool ism::mpl::is_string_v<ism::StringView>{ true };

#endif // !_ISM_STRING_VIEW_HPP_
