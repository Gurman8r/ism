#ifndef _ISM_PATH_HPP_
#define _ISM_PATH_HPP_

#include <core/string/string_name.hpp>

namespace ism
{
	class ISM_API Path
	{
	private:
		String m_text;

	public:
		~Path() {}

		Path() : m_text{} {}

		Path(nullptr_t) : m_text{} {}

		Path(cstring value) : m_text{ value } {}

		Path(Path const & value) : m_text{ value } {}

		Path(Path && value) noexcept : m_text{} { swap(value); }

		Path(String const & value) : m_text{ value } {}

		Path(StringName const & value) : m_text{ value } {}

		Path(StringName && value) noexcept : m_text{ std::move(value).string() } {}

		Path(StringView const & value) : m_text{ value } {}

		Path & swap(Path & value) noexcept { return m_text.swap(value.m_text), (*this); }

		Path & operator=(Path const & value) { Path temp{ value }; return swap(temp); }

		void clear() noexcept { m_text.clear(); }

		NODISCARD operator void * () const noexcept { return !m_text.empty() ? (void *)1 : nullptr; }

		NODISCARD bool operator==(String const & value) { return m_text == value; }

		NODISCARD bool operator==(cstring value) { return m_text == value; }

		NODISCARD bool operator!=(String const & value) { return m_text != value; }

		NODISCARD bool operator<(Path const & value) { return m_text < value.m_text; }

		NODISCARD bool operator>(Path const & value) { return m_text > value.m_text; }

		NODISCARD bool operator==(Path const & value) { return m_text == value.m_text; }

		NODISCARD auto data() const noexcept -> cstring { return m_text.data(); }

		NODISCARD auto c_str() const noexcept -> cstring { return m_text.data(); }

		NODISCARD auto length() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto size() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto hash_code() const noexcept -> hash_t { return hash(m_text); }

		NODISCARD bool empty() const noexcept { return m_text.empty(); }

		NODISCARD auto string() const & noexcept -> String & { return const_cast<String &>(m_text); }

		NODISCARD auto string() && noexcept -> String && { return std::move(m_text); }

		NODISCARD operator String() const noexcept { return m_text; }
	};

	inline void to_json(Json & json, Path const & value)
	{
		json = (String const &)value;
	}

	inline void from_json(Json const & json, Path & value)
	{
		json.get_to((String &)value);
	}

	template <> struct ism::Hasher<Path>
	{
		hash_t operator()(Path const & value) const
		{
			return value.hash_code();
		}
	};

	inline bool operator==(String const & lhs, Path const & rhs)
	{
		return lhs == rhs.string();
	}

	inline bool operator!=(String const & lhs, Path const & rhs)
	{
		return lhs != rhs.string();
	}

	inline bool operator==(cstring lhs, Path const & rhs)
	{
		return lhs == rhs.string();
	}

	inline bool operator!=(cstring lhs, Path const & rhs)
	{
		return lhs != rhs.string();
	}
}

template <> constexpr bool ism::mpl::is_string_v<ism::Path>{ true };

#endif // !_ISM_PATH_HPP_
