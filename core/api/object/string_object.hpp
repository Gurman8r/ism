#ifndef _ISM_STRING_OBJECT_HPP_
#define _ISM_STRING_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// string
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreString : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreString, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	protected:
		String m_string{};

	public:
		using storage_type = decltype(m_string);
		using item_type = typename String::value_type;
		using iterator = typename String::iterator;
		using const_iterator = typename String::const_iterator;

		CoreString(String const & v) : base_type{ &ob_type_static }, m_string{ v } {}

		CoreString(String && v) noexcept : base_type{ &ob_type_static }, m_string{ std::move(v) } {}

		CoreString(cstring v) : base_type{ &ob_type_static }, m_string{ v } {}

		CoreString(cstring v, size_t n) : base_type{ &ob_type_static }, m_string{ v, n } {}

		CoreString(StringName const & v) : self_type{ v.string() } {}

		CoreString(StringName && v) noexcept : self_type{ std::move(v).string() } {}

		CoreString(std::initializer_list<item_type> init) : self_type{ String{ init.begin(), init.end() } } {}

		template <class T> CoreString(Handle<T> const & o) noexcept : self_type{}
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				m_string = ***o;
			}
			else if (isinstance<STR>(o))
			{
				m_string = ***STR(o);
			}
			else if (TYPE t{ typeof(o) }; t && t->tp_str)
			{
				m_string = ***t->tp_str(o);
			}
		}

		NODISCARD operator String * () const { return const_cast<String *>(&m_string); }
		
		NODISCARD auto operator->() const { return const_cast<String *>(&m_string); }

		NODISCARD auto c_str() const noexcept { return m_string.c_str(); }
		
		NODISCARD auto data() const noexcept { return m_string.data(); }
		
		NODISCARD bool empty() const noexcept { return m_string.empty(); }
		
		NODISCARD auto size() const noexcept { return m_string.size(); }
		
		NODISCARD auto front() const { return m_string.front(); }
		
		NODISCARD auto back() const { return m_string.back(); }
		
		void reserve(size_t count) { m_string.reserve(count); }
		
		void resize(size_t count) { m_string.reserve(count); }
		
		void shrink_to_fit() { m_string.shrink_to_fit(); }

		void erase(size_t i) { m_string.erase(begin() + i); }

		void erase(OBJECT const & i) { m_string.erase(begin() + i.cast<size_t>()); }

		NODISCARD auto begin() noexcept -> iterator { return m_string.begin(); }
		
		NODISCARD auto begin() const noexcept -> const_iterator { return m_string.begin(); }
		
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_string.cbegin(); }
		
		NODISCARD auto end() noexcept -> iterator { return m_string.end(); }
		
		NODISCARD auto end() const noexcept -> const_iterator { return m_string.end(); }
		
		NODISCARD auto cend() const noexcept -> const_iterator { return m_string.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_OBJECT_HPP_
