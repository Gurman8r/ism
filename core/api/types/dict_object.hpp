#ifndef _ISM_API_TYPES_DICT_OBJECT_HPP_
#define _ISM_API_TYPES_DICT_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// dict
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreDict : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreDict, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	private:
		HashMap<OBJECT, OBJECT> m_data{};

		using storage_type = decltype(m_data);

	public:
		using item_type = typename storage_type::value_type;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		CoreDict(storage_type const & v) : base_type{ type_static() }, m_data{ v } {}

		CoreDict(storage_type && v) noexcept : base_type{ type_static() }, m_data{ std::move(v) } {}

		CoreDict(std::initializer_list<item_type> init) : self_type{}
		{
			m_data.reserve(init.size());
			for (item_type const & e : init) {
				m_data.insert(e);
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }
		
		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

		NODISCARD bool empty() const noexcept { return m_data.empty(); }
		
		NODISCARD auto size() const noexcept { return m_data.size(); }

		template <class I = OBJECT
		> void erase(I && i) { m_data.erase(detail::object_forward(FWD(i))); }

		template <class I = OBJECT
		> bool contains(I && i) const { return end() != m_data.find(detail::object_forward(FWD(i))); }

		template <class I = OBJECT, class V = OBJECT
		> void insert(I && i, V && v) { m_data.try_emplace(detail::object_forward(FWD(i)), detail::object_forward(FWD(v))); }

		NODISCARD auto begin() noexcept -> iterator { return m_data.begin(); }
		
		NODISCARD auto begin() const noexcept -> const_iterator { return m_data.begin(); }
		
		NODISCARD auto cbegin() const noexcept -> const_iterator { return m_data.cbegin(); }
		
		NODISCARD auto end() noexcept -> iterator { return m_data.end(); }
		
		NODISCARD auto end() const noexcept -> const_iterator { return m_data.end(); }
		
		NODISCARD auto cend() const noexcept -> const_iterator { return m_data.cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_DICT_OBJECT_HPP_
