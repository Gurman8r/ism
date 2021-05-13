#ifndef _ISM_LIST_OBJECT_HPP_
#define _ISM_LIST_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// list
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreList : public CoreObject
	{
		ISM_OBJECT(CoreList, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	protected:
		Vector<OBJECT> m_list{};

	public:
		using storage_type = decltype(m_list);
		
		using iterator = storage_type::iterator;
		
		using const_iterator = storage_type::const_iterator;

		CoreList(storage_type const & v) : base_type{ &ob_type_static }, m_list{ v } {}

		CoreList(storage_type && v) noexcept : base_type{ &ob_type_static }, m_list{ std::move(v) } {}

		CoreList(std::initializer_list<OBJECT> init) : self_type{}
		{
			m_list.reserve(init.size());

			for (auto const & e : init) { m_list.push_back(e); }
		}

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_list); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_list); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// LIST
	template <> class Handle<CoreList> : public BaseHandle<CoreList>
	{
		ISM_HANDLE(CoreList);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CoreList::storage_type;

		using iterator = CoreList::iterator;
		
		using const_iterator = CoreList::const_iterator;

		NODISCARD auto data() const noexcept { return (*m_ref)->data(); }

		NODISCARD bool empty() const noexcept { return (*m_ref)->empty(); }

		NODISCARD auto size() const noexcept { return (*m_ref)->size(); }

		NODISCARD auto front() const { return (*m_ref)->front(); }

		NODISCARD auto back() const { return (*m_ref)->back(); }

		void reserve(size_t count) { (*m_ref)->reserve(count); }

		void resize(size_t count) { (*m_ref)->reserve(count); }

		void shrink_to_fit() { (*m_ref)->shrink_to_fit(); }

		void erase(size_t i) { (*m_ref)->erase(begin() + i); }

		void erase(OBJECT const & i) { (*m_ref)->erase(begin() + i.cast<size_t>()); }

		template <class Value = OBJECT
		> void append(Value && v) { (*m_ref)->emplace_back(object_forward(FWD(v))); }

		template <class Value = OBJECT
		> bool contains(Value && v) const { return end() != std::find(begin(), end(), object_forward(FWD(v))); }

		template <class Value = OBJECT
		> auto find(Value && v) { return std::find(begin(), end(), object_forward(FWD(v))); }

		template <class Value = OBJECT
		> void insert(size_t i, Value && v) { (*m_ref)->insert(begin() + i, object_forward(FWD(v))); }

		template <class Value = OBJECT
		> void insert(OBJECT const & i, Value && v) { (*m_ref)->insert(begin() + i.cast<size_t>(), object_forward(FWD(v))); }

		auto get(size_t i) const { return (*m_ref)[i]; }

		auto get(OBJECT const & i) const { return (*m_ref)[i.cast<size_t>()]; }

		template <class Value = OBJECT
		> void set(size_t i, Value && v) { m_list[i] = object_forward(FWD(v)); }

		template <class Value = OBJECT
		> void set(OBJECT const & i, Value && v) { m_list[i.cast<size_t>()] = object_forward(FWD(v)); }

		NODISCARD auto begin() noexcept -> iterator { return (*m_ref)->begin(); }

		NODISCARD auto begin() const noexcept -> const_iterator { return (*m_ref)->begin(); }

		NODISCARD auto cbegin() const noexcept -> const_iterator { return (*m_ref)->cbegin(); }

		NODISCARD auto end() noexcept -> iterator { return (*m_ref)->end(); }

		NODISCARD auto end() const noexcept -> const_iterator { return (*m_ref)->end(); }

		NODISCARD auto cend() const noexcept -> const_iterator { return (*m_ref)->cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LIST_OBJECT_HPP_
