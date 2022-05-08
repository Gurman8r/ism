#ifndef _ISM_STRING_NAME_HPP_
#define _ISM_STRING_NAME_HPP_

#include <core/string/string_view.hpp>

namespace ism
{
	class ISM_API StringName
	{
	private:
		String m_text;

	public:
		~StringName() {}

		StringName() : m_text{} {}

		StringName(nullptr_t) : m_text{} {}

		StringName(cstring value) : m_text{ value } {}

		StringName(StringName const & value) : m_text{ value } {}

		StringName(String const & value) : m_text{ value } {}

		StringName(String && value) noexcept : m_text{ std::move(value) } {}

		StringName(StringView const & value) : m_text{ value } {}

		StringName & swap(StringName & value) noexcept { return m_text.swap(value.m_text), (*this); }

		StringName & operator=(StringName const & value) { StringName temp{ value }; return swap(temp); }

		void clear() noexcept { m_text.clear(); }

		NODISCARD operator void * () const noexcept { return !m_text.empty() ? (void *)1 : nullptr; }

		NODISCARD bool operator==(String const & value) { return m_text == value; }

		NODISCARD bool operator==(cstring value) { return m_text == value; }

		NODISCARD bool operator!=(String const & value) { return m_text != value; }

		NODISCARD bool operator<(StringName const & value) { return m_text < value.m_text; }

		NODISCARD bool operator>(StringName const & value) { return m_text > value.m_text; }

		NODISCARD bool operator==(StringName const & value) { return m_text == value.m_text; }

		NODISCARD auto data() const noexcept -> cstring { return m_text.data(); }

		NODISCARD auto c_str() const noexcept -> cstring { return m_text.data(); }

		NODISCARD auto length() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto size() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto hash_code() const noexcept -> hash_t { return hash(m_text); }

		NODISCARD bool empty() const noexcept { return m_text.empty(); }

		NODISCARD auto string() const & noexcept -> String & { return const_cast<String &>(m_text); }

		NODISCARD auto string() && noexcept -> String && { return std::move(m_text); }

		NODISCARD operator String & () const noexcept { return string(); }

		NODISCARD operator String && () && noexcept { return std::move(*this).string(); }
	};

	inline void to_json(JSON & json, StringName const & value)
	{
		json = (String const &)value;
	}

	inline void from_json(JSON const & json, StringName & value)
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
