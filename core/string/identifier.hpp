#ifndef _ISM_IDENTIFIER_HPP_
#define _ISM_IDENTIFIER_HPP_

#include <core/string/string_view.hpp>

namespace ism
{
	// string name
	class ISM_API Identifier
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		String m_text;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public:
		NODISCARD operator void *() const noexcept { return !m_text.empty() ? (void *)1 : nullptr; }

		NODISCARD bool operator==(String const & value) { return m_text == value; }
		
		NODISCARD bool operator==(cstring value) { return m_text == value; }
		
		NODISCARD bool operator!=(String const & value) { return m_text != value; }
		
		NODISCARD bool operator<(Identifier const & value) { return m_text < value.m_text; }
		
		NODISCARD bool operator>(Identifier const & value) { return m_text > value.m_text; }
		
		NODISCARD bool operator==(Identifier const & value) { return m_text == value.m_text; }

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

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		void clear() noexcept { m_text.clear(); }

		void assign(Identifier const & value) { m_text.assign(value.m_text); }

		void swap(Identifier & value) noexcept { m_text.swap(value.m_text); }

		void operator=(Identifier const & value) { assign(value); }

		void operator=(Identifier && value) noexcept { this->swap(std::move(value)); }

		~Identifier();

		Identifier() : m_text{} {}
		
		Identifier(nullptr_t) : m_text{} {}

		Identifier(cstring value) : m_text{ value } {}

		Identifier(Identifier const & value) : m_text{ value } {}

		Identifier(String const & value) : m_text{ value } {}

		Identifier(StringView const & value) : m_text{ value } {}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};

	template <> struct ism::Hash<Identifier>
	{
		Hash() = default;
		hash_t operator()(Identifier const & value) const
		{
			return value.hash_code();
		}
	};

	inline bool operator==(String const & lhs, Identifier const & rhs)
	{
		return lhs == rhs.string();
	}

	inline bool operator!=(String const & lhs, Identifier const & rhs)
	{
		return lhs != rhs.string();
	}

	inline bool operator==(cstring lhs, Identifier const & rhs)
	{
		return lhs == rhs.string();
	}

	inline bool operator!=(cstring lhs, Identifier const & rhs)
	{
		return lhs != rhs.string();
	}
}

#endif // !_ISM_IDENTIFIER_HPP_
