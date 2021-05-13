#ifndef _ISM_STRING_OBJECT_HPP_
#define _ISM_STRING_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// string
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreString : public CoreObject
	{
		ISM_OBJECT(CoreString, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	public:
		String m_string{};

		using storage_type = decltype(m_string);
		
		using iterator = storage_type::iterator;
		
		using const_iterator = storage_type::const_iterator;

		CoreString(storage_type const & v) : base_type{ &ob_type_static }, m_string{ v } {}

		CoreString(storage_type && v) noexcept : base_type{ &ob_type_static }, m_string{ std::move(v) } {}

		CoreString(cstring v) : base_type{ &ob_type_static }, m_string{ v } {}

		CoreString(cstring v, size_t n) : base_type{ &ob_type_static }, m_string{ v, n } {}

		CoreString(StringName const & v) : self_type{ v.string() } {}

		CoreString(StringName && v) noexcept : self_type{ std::move(v).string() } {}

		CoreString(std::initializer_list<char> init) : self_type{ storage_type{ init.begin(), init.end() } } {}

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

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_string); }
		
		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_string); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreString> : public BaseHandle<CoreString>
	{
		ISM_HANDLE(CoreString);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CoreString::storage_type;

		using iterator = CoreString::iterator;

		using const_iterator = CoreString::const_iterator;

		NODISCARD auto c_str() const noexcept { return (*m_ref)->c_str(); }

		NODISCARD auto data() const noexcept { return (*m_ref)->data(); }

		NODISCARD bool empty() const noexcept { return (*m_ref)->empty(); }

		NODISCARD auto size() const noexcept { return (*m_ref)->size(); }

		NODISCARD auto front() const { return (*m_ref)->front(); }

		NODISCARD auto back() const { return (*m_ref)->back(); }

		void reserve(size_t count) { (*m_ref)->reserve(count); }

		void resize(size_t count) { (*m_ref)->reserve(count); }

		void erase(size_t i) { (*m_ref)->erase(begin() + i); }

		void erase(OBJECT const & i) { (*m_ref)->erase(begin() + i.cast<size_t>()); }

		NODISCARD auto begin() noexcept -> iterator { return (*m_ref)->begin(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return (*m_ref)->begin(); }

		NODISCARD auto cbegin() const noexcept -> const_iterator { return (*m_ref)->cbegin(); }

		NODISCARD auto end() noexcept -> iterator { return (*m_ref)->end(); }

		NODISCARD auto end() const noexcept -> const_iterator { return (*m_ref)->end(); }

		NODISCARD auto cend() const noexcept -> const_iterator { return (*m_ref)->cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD STR repr(Handle<T> const & o) noexcept
	{
		TYPE t{ typeof(o) };
		return t && t->tp_repr ? t->tp_repr(o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_OBJECT_HPP_
