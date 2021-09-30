#ifndef _ISM_STRING_NAME_HPP_
#define _ISM_STRING_NAME_HPP_

#include <core/string/string_view.hpp>

namespace ism
{
	// string name
	class ISM_API StringName
	{
	private:
		String m_text;

	public:
		~StringName();

		StringName() : m_text{} {}

		StringName(nullptr_t) : m_text{} {}

		StringName(cstring value) : m_text{ value } {}

		StringName(StringName const & value) : m_text{ value } {}

		StringName(String const & value) : m_text{ value } {}

		StringName(StringView const & value) : m_text{ value } {}

	public:
		void clear() noexcept { m_text.clear(); }

		void assign(StringName const & value) { m_text.assign(value.m_text); }

		void swap(StringName & value) noexcept { m_text.swap(value.m_text); }

		void operator=(StringName const & value) { assign(value); }

		void operator=(StringName && value) noexcept { this->swap(std::move(value)); }

	public:
		NODISCARD operator void * () const noexcept { return !m_text.empty() ? (void *)1 : nullptr; }

		NODISCARD bool operator==(String const & value) { return m_text == value; }

		NODISCARD bool operator==(cstring value) { return m_text == value; }

		NODISCARD bool operator!=(String const & value) { return m_text != value; }

		NODISCARD bool operator<(StringName const & value) { return m_text < value.m_text; }

		NODISCARD bool operator>(StringName const & value) { return m_text > value.m_text; }

		NODISCARD bool operator==(StringName const & value) { return m_text == value.m_text; }

		NODISCARD auto native() noexcept -> String & { return m_text; }

		NODISCARD auto native() const noexcept -> String const & { return m_text; }

		NODISCARD auto data() const noexcept -> cstring { return m_text.data(); }

		NODISCARD auto c_str() const noexcept -> cstring { return m_text.data(); }

		NODISCARD auto length() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto size() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto hash_code() const noexcept -> hash_t { return hash(m_text); }

		NODISCARD bool empty() const noexcept { return m_text.empty(); }

		NODISCARD auto string() const noexcept -> String { return m_text; }

		NODISCARD operator String() const noexcept { return m_text; }
	};

	template <> struct ism::Hasher<StringName>
	{
		Hasher() = default;
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

template <
> constexpr bool ism::mpl::is_string_v<ism::StringName>{ true };

#endif // !_ISM_STRING_NAME_HPP_
