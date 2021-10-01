#ifndef _ISM_PATH_HPP_
#define _ISM_PATH_HPP_

#include <core/string/string_utility.hpp>

namespace ism
{
	class ISM_API Path
	{
	private:
		WideString m_text{};

	public:
		~Path() {}

		Path() : m_text{} {}

		Path(nullptr_t) : m_text{} {}

		Path(cwstring value) : m_text{ value } {}

		Path(cstring value) : m_text{ util::widen((String)value) } {}

		Path(Path const & value) : m_text{ value.m_text } {}

		Path(WideString const & value) : m_text{ value } {}

		Path(String const & value) : m_text{ util::widen(value) } {}

		Path(StringView const & value) : m_text{ util::widen((String)value) } {}

	public:
		void clear() noexcept { m_text.clear(); }

		void assign(Path const & value) { m_text.assign(value.m_text); }

		void swap(Path & value) noexcept { m_text.swap(value.m_text); }

		void operator=(Path const & value) { assign(value); }

		void operator=(Path && value) noexcept { this->swap(std::move(value)); }

	public:
		NODISCARD operator void * () const noexcept { return !m_text.empty() ? (void *)1 : nullptr; }

		NODISCARD bool operator==(WideString const & value) { return m_text == value; }

		NODISCARD bool operator==(cwstring value) { return m_text == value; }

		NODISCARD bool operator!=(WideString const & value) { return m_text != value; }

		NODISCARD bool operator<(Path const & value) { return m_text < value.m_text; }

		NODISCARD bool operator>(Path const & value) { return m_text > value.m_text; }

		NODISCARD bool operator==(Path const & value) { return m_text == value.m_text; }

		NODISCARD auto native() noexcept -> WideString & { return m_text; }

		NODISCARD auto native() const noexcept -> WideString const & { return m_text; }

		NODISCARD auto c_str() const noexcept -> cwstring { return m_text.c_str(); }

		NODISCARD auto data() const noexcept -> cwstring { return m_text.data(); }

		NODISCARD auto length() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto size() const noexcept -> size_t { return m_text.size(); }

		NODISCARD auto hash_code() const noexcept -> hash_t { return hash(m_text); }

		NODISCARD bool empty() const noexcept { return m_text.empty(); }

		NODISCARD auto wstring() const noexcept -> WideString { return m_text; }

		NODISCARD auto string() const noexcept -> String { return util::narrow(m_text); }

		NODISCARD operator WideString() const noexcept { return m_text; }

		NODISCARD operator String() const noexcept { return util::narrow(m_text); }

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	template <> struct ism::Hasher<Path>
	{
		Hasher() = default;
		hash_t operator()(Path const & value) const
		{
			return value.hash_code();
		}
	};

	inline bool operator==(WideString const & lhs, Path const & rhs) { return lhs == rhs.wstring(); }

	inline bool operator==(cwstring lhs, Path const & rhs) { return lhs == rhs.wstring(); }

	inline bool operator==(String const & lhs, Path const & rhs) { return lhs == rhs.string(); }

	inline bool operator==(cstring lhs, Path const & rhs) { return lhs == rhs.string(); }

	inline bool operator!=(WideString const & lhs, Path const & rhs) { return lhs != rhs.wstring(); }

	inline bool operator!=(cwstring lhs, Path const & rhs) { return lhs != rhs.wstring(); }

	inline bool operator!=(String const & lhs, Path const & rhs) { return lhs != rhs.string(); }

	inline bool operator!=(cstring lhs, Path const & rhs) { return lhs != rhs.string(); }
}

#endif // !_ISM_PATH_HPP_
