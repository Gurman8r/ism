#ifndef _ISM_STRING_NAME_HPP_
#define _ISM_STRING_NAME_HPP_

#include <core/string/string_view.hpp>

namespace ism
{
	class ISM_API StringName
	{
		String m_text;

	public:
		using self_type			= typename StringName;
		using storage_type		= typename String;
		using view_type			= typename StringView;
		using value_type		= typename storage_type::value_type;
		using reference			= typename storage_type::reference;
		using pointer			= typename storage_type::pointer;
		using const_reference	= typename storage_type::const_reference;
		using const_pointer		= typename storage_type::const_pointer;
		using iterator			= typename storage_type::iterator;
		using const_iterator	= typename storage_type::const_iterator;

	public:
		StringName() noexcept : m_text{} {}

		StringName(nullptr_t) noexcept : m_text{} {}

		StringName(const_pointer value) : m_text{ value } {}

		StringName(const_pointer value, size_t size) : m_text{ value, size } {}

		StringName(const_pointer first, const_pointer last) : m_text{ first, last } {}

		StringName(storage_type const & value) : m_text{ value } {}

		StringName(storage_type && value) noexcept : m_text{ std::move(value) } {}

		StringName(view_type const value) : m_text{ value.string() } {}

		StringName(self_type const & value) : m_text{ value } {}

		StringName(self_type && value) noexcept : m_text{ std::move(value) } {}

		self_type & operator=(self_type const & value) { return copy(value); }

	public:
		void clear() noexcept { m_text.clear(); }

		self_type & copy(self_type const & value) { return (m_text = value.m_text), (*this); }

		self_type & swap(self_type & value) noexcept { return m_text.swap(value.m_text), (*this); }

	public:
		operator void * () const noexcept { return !m_text.empty() ? (void *)m_text.data() : nullptr; }

		bool operator==(storage_type const & value) { return m_text == value; }

		bool operator==(const_pointer value) { return m_text == value; }

		bool operator!=(storage_type const & value) { return m_text != value; }

		bool operator<(self_type const & value) { return m_text < value.m_text; }

		bool operator>(self_type const & value) { return m_text > value.m_text; }

		bool operator==(self_type const & value) { return m_text == value.m_text; }

		auto data() noexcept -> pointer { return m_text.data(); }

		auto data() const noexcept -> const_pointer { return m_text.data(); }

		auto c_str() const noexcept -> const_pointer { return m_text.data(); }

		auto length() const noexcept -> size_t { return m_text.size(); }

		auto size() const noexcept -> size_t { return m_text.size(); }

		auto hash_code() const noexcept -> hash_t { return hash(m_text); }

		bool empty() const noexcept { return m_text.empty(); }

		auto view() const noexcept -> view_type { return { m_text.data(), m_text.size() }; }

		auto string() & noexcept -> storage_type & { return m_text; }

		auto string() const & noexcept -> storage_type const & { return m_text; }

		auto string() && noexcept -> storage_type && { return std::move(m_text); }

		operator view_type () noexcept { return { m_text.data(), m_text.size() }; }

		operator storage_type & () & noexcept { return m_text; }

		operator storage_type const & () const & noexcept { return m_text; }

		operator storage_type && () && noexcept { return std::move(m_text); }

	public:
		auto begin() noexcept -> iterator { return m_text.begin(); }

		auto begin() const noexcept -> const_iterator { return m_text.begin(); }

		auto cbegin() const noexcept -> const_iterator { return m_text.cbegin(); }

		auto end() noexcept -> iterator { return m_text.end(); }

		auto end() const noexcept -> const_iterator { return m_text.end(); }

		auto cend() const noexcept -> const_iterator { return m_text.cend(); }
	};

	inline void to_json(Json & json, StringName const & value)
	{
		json = (String const &)value;
	}

	inline void from_json(Json const & json, StringName & value)
	{
		json.get_to((String &)value);
	}

	template <> struct ism::Hasher<StringName>
	{
		hash_t operator()(StringName const & value) const
		{
			return value.hash_code();
		}
	};

	inline bool operator==(String const & lhs, StringName const & rhs)
	{
		return lhs == rhs.string();
	}

	inline bool operator!=(String const & lhs, StringName const & rhs)
	{
		return lhs != rhs.string();
	}

	inline bool operator==(cstring lhs, StringName const & rhs)
	{
		return lhs == rhs.string();
	}

	inline bool operator!=(cstring lhs, StringName const & rhs)
	{
		return lhs != rhs.string();
	}
}

template <> constexpr bool ism::mpl::is_string_v<ism::StringName>{ true };

#endif // !_ISM_STRING_NAME_HPP_
