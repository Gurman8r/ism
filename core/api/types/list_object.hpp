#ifndef _ISM_API_TYPES_LIST_OBJECT_HPP_
#define _ISM_API_TYPES_LIST_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// list
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreList : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreList, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		using storage_type = Vector<OBJECT>;
		using item_type = typename storage_type::value_type;
		using iterator = typename storage_type::iterator;
		using const_iterator = typename storage_type::const_iterator;

		CoreList(storage_type const & v) : base_type{ &ob_type_static }, m_data{ v } {}

		CoreList(storage_type && v) noexcept : base_type{ &ob_type_static }, m_data{ std::move(v) } {}

		CoreList(std::initializer_list<item_type> init) : self_type{}
		{
			m_data.reserve(init.size());
			for (item_type const & e : init) {
				m_data.push_back(e);
			}
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto operator->() const { return const_cast<storage_type *>(&m_data); }

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

		template <class V = OBJECT
		> bool contains(V && v) const { return end() != std::find(begin(), end(), detail::object_forward(FWD(v))); }

		template <class V = OBJECT
		> void append(V && v) { m_data.emplace_back(detail::object_forward(FWD(v))); }

		template <class V = OBJECT
		> void insert(size_t i, V && v) { m_data.insert(begin() + i, detail::object_forward(FWD(v))); }

		template <class V = OBJECT
		> void insert(OBJECT const & i, V && v) { m_data.insert(begin() + i.cast<size_t>(), detail::object_forward(FWD(v))); }

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

#endif // !_ISM_API_TYPES_LIST_OBJECT_HPP_
