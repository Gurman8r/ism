#ifndef _ISM_STRING_NAME_HPP_
#define _ISM_STRING_NAME_HPP_

#include <core/string/string.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API StringName
	{
		String m_text;

	public:
		using self_type					= typename StringName;
		using storage_type				= typename String;
		using view_type					= typename StringView;
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

	public:
		StringName() noexcept : m_text{} {}
		StringName(nullptr_t) noexcept : m_text{} {}
		StringName(const_pointer value) : m_text{ value } {}
		StringName(const_pointer value, size_type size) : m_text{ value, size } {}
		StringName(const_pointer first, const_pointer last) : m_text{ first, last } {}
		StringName(storage_type const & value) : m_text{ value } {}
		StringName(storage_type && value) noexcept : m_text{ std::move(value) } {}
		StringName(view_type const value) : m_text{ value.data(), value.size() } {}
		StringName(self_type const & value) : m_text{ value } {}
		StringName(self_type && value) noexcept : m_text{ std::move(value) } {}

		self_type & operator=(self_type const & value) { return (m_text = value.m_text), (*this); }
		
		self_type & swap(self_type & value) noexcept { return m_text.swap(value.m_text), (*this); }

		void clear() noexcept { m_text.clear(); }

		NODISCARD operator void * () const noexcept { return !empty() ? (void *)data() : nullptr; }
		NODISCARD auto data() noexcept -> pointer { return m_text.data(); }
		NODISCARD auto data() const noexcept -> const_pointer { return m_text.data(); }
		NODISCARD auto c_str() const noexcept -> const_pointer { return m_text.data(); }
		NODISCARD bool empty() const noexcept { return m_text.empty(); }
		NODISCARD auto length() const noexcept -> size_type { return m_text.size(); }
		NODISCARD auto size() const noexcept -> size_type { return m_text.size(); }
		NODISCARD auto hash_code() const noexcept -> hash_t { return m_text.hash_code(); }
		NODISCARD char & operator[](size_type i) & noexcept { return m_text[i]; }
		NODISCARD char operator[](size_type i) const & noexcept { return m_text[i]; }

		NODISCARD auto string() & noexcept -> storage_type & { return m_text; }
		NODISCARD auto string() const & noexcept -> storage_type const & { return m_text; }
		NODISCARD auto string() && noexcept -> storage_type && { return std::move(m_text); }
		NODISCARD auto view() const noexcept -> view_type { return { m_text.data(), m_text.size() }; }

		NODISCARD operator storage_type & () & noexcept { return m_text; }
		NODISCARD operator storage_type const & () const & noexcept { return m_text; }
		NODISCARD operator storage_type && () && noexcept { return std::move(m_text); }
		NODISCARD operator view_type () const noexcept { return { m_text.data(), m_text.size() }; }

		NODISCARD auto begin() noexcept -> iterator { return m_text.begin(); }
		NODISCARD auto begin() const noexcept -> const_iterator { return m_text.begin(); }
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_text.cbegin(); }
		NODISCARD auto end() noexcept -> iterator { return m_text.end(); }
		NODISCARD auto end() const noexcept -> const_iterator { return m_text.end(); }
		NODISCARD auto cend() const noexcept -> const_iterator { return m_text.cend(); }
		NODISCARD auto rbegin() noexcept -> reverse_iterator { return m_text.rbegin(); }
		NODISCARD auto rbegin() const noexcept -> const_reverse_iterator { return m_text.rbegin(); }
		NODISCARD auto crbegin() const noexcept -> const_reverse_iterator { return m_text.crbegin(); }
		NODISCARD auto rend() noexcept -> reverse_iterator { return m_text.rend(); }
		NODISCARD auto rend() const noexcept -> const_reverse_iterator { return m_text.rend(); }
		NODISCARD auto crend() const noexcept -> const_reverse_iterator { return m_text.crend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace mpl { template <> constexpr bool is_string_v<StringName>{ true }; }

	inline StringName operator "" _name(cstring s, size_t n) noexcept { return StringName{ String{ s, n } }; }

	template <> struct Hasher<StringName>
	{
		hash_t operator()(StringName const & value) const { return value.hash_code(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	inline bool operator==(String const & lhs, StringName const & rhs) { return lhs == rhs.string(); }
	inline bool operator==(cstring lhs, StringName const & rhs) { return lhs == rhs.string(); }
	inline bool operator==(StringName const & lhs, cstring rhs) { return lhs.string() == rhs; }
	inline bool operator==(StringName const & lhs, StringName const & rhs) { return lhs.string() == rhs.string(); }

	inline bool operator!=(String const & lhs, StringName const & rhs) { return lhs != rhs.string(); }
	inline bool operator!=(cstring lhs, StringName const & rhs) { return lhs != rhs.string(); }
	inline bool operator!=(StringName const & lhs, cstring rhs) { return lhs.string() != rhs; }
	inline bool operator!=(StringName const & lhs, StringName const & rhs) { return lhs.string() != rhs.string(); }

	inline bool operator<(String const & lhs, StringName const & rhs) { return lhs < rhs.string(); }
	inline bool operator<(cstring lhs, StringName const & rhs) { return lhs < rhs.string(); }
	inline bool operator<(StringName const & lhs, cstring rhs) { return lhs.string() < rhs; }
	inline bool operator<(StringName const & lhs, StringName const & rhs) { return lhs.string() < rhs.string(); }

	inline bool operator>(String const & lhs, StringName const & rhs) { return lhs > rhs.string(); }
	inline bool operator>(cstring lhs, StringName const & rhs) { return lhs > rhs.string(); }
	inline bool operator>(StringName const & lhs, cstring rhs) { return lhs.string() > rhs; }
	inline bool operator>(StringName const & lhs, StringName const & rhs) { return lhs.string() > rhs.string(); }

	inline bool operator<=(String const & lhs, StringName const & rhs) { return lhs <= rhs.string(); }
	inline bool operator<=(cstring lhs, StringName const & rhs) { return lhs <= rhs.string(); }
	inline bool operator<=(StringName const & lhs, cstring rhs) { return lhs.string() <= rhs; }
	inline bool operator<=(StringName const & lhs, StringName const & rhs) { return lhs.string() <= rhs.string(); }

	inline bool operator>=(String const & lhs, StringName const & rhs) { return lhs >= rhs.string(); }
	inline bool operator>=(cstring lhs, StringName const & rhs) { return lhs >= rhs.string(); }
	inline bool operator>=(StringName const & lhs, cstring rhs) { return lhs.string() >= rhs; }
	inline bool operator>=(StringName const & lhs, StringName const & rhs) { return lhs.string() >= rhs.string(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_NAME_HPP_
