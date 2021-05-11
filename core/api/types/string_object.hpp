#ifndef _ISM_API_TYPES_STRING_OBJECT_HPP_
#define _ISM_API_TYPES_STRING_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// string
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreString : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreString, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		using storage_type = String;
		using item_type = typename storage_type::value_type;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		CoreString(storage_type const & v) : base_type{ &ob_type_static }, m_data{ v } {}

		CoreString(storage_type && v) noexcept : base_type{ &ob_type_static }, m_data{ std::move(v) } {}

		CoreString(cstring v) : base_type{ &ob_type_static }, m_data{ v } {}

		CoreString(cstring v, size_t n) : base_type{ &ob_type_static }, m_data{ v, n } {}

		CoreString(StringName const & v) : self_type{ v.string() } {}

		CoreString(StringName && v) noexcept : self_type{ std::move(v).string() } {}

		CoreString(std::initializer_list<item_type> init) : self_type{ storage_type{ init.begin(), init.end() } } {}

		template <class T> CoreString(Handle<T> const & o) noexcept : self_type{}
		{
			if constexpr (std::is_same_v<T, self_type>)
			{
				m_data = ***o;
			}
			else if (isinstance<STR>(o))
			{
				m_data = ***STR(o);
			}
			else if (TYPE t{ typeof(o) }; t && t->tp_str)
			{
				m_data = ***t->tp_str(o);
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto c_str() const noexcept { return m_data.c_str(); }
		
		NODISCARD auto data() const noexcept { return m_data.data(); }
		
		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		
		NODISCARD auto size() const noexcept { return m_data.size(); }
		
		NODISCARD auto front() const { return m_data.front(); }
		
		NODISCARD auto back() const { return m_data.back(); }
		
		void reserve(size_t count) { m_data.reserve(count); }
		
		void resize(size_t count) { m_data.reserve(count); }
		
		void shrink_to_fit() { m_data.shrink_to_fit(); }

		void erase(size_t i) { m_data.erase(begin() + i); }

		void erase(OBJECT const & i) { m_data.erase(begin() + i.cast<size_t>()); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }

	private:
		storage_type m_data{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_STRING_OBJECT_HPP_
