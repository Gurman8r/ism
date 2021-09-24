#ifndef _ISM_LIST_OBJECT_HPP_
#define _ISM_LIST_OBJECT_HPP_

#include <core/object/type_object.hpp>

// list
namespace ism
{
	// list object
	class ISM_API ListObject : public Object
	{
		OBJECT_CLASS(ListObject, Object);

	public:
		Vector<OBJ> m_list{};

		using storage_type = decltype(m_list);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_list); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_list); }

		NODISCARD operator storage_type & () const { return const_cast<storage_type &>(m_list); }

		template <class Iter
		> ListObject(Iter first, Iter last) : ListObject{} { m_list = { first, last }; }

		ListObject() noexcept : Object{} {}

		ListObject(storage_type const & value) : ListObject{} { m_list = value; }

		ListObject(storage_type && value) noexcept : ListObject{} { m_list = std::move(value); }

		ListObject(std::initializer_list<OBJ> init) : ListObject{}
		{
			m_list.reserve(init.size());

			for (auto const & e : init) { m_list.push_back(e); }
		}

		void clear() { m_list.clear(); }

		void reserve(size_t count) { m_list.reserve(count); }

		void resize(size_t count) { m_list.resize(count); }

		template <class Value = OBJ
		> void resize(size_t count, Value && value) { m_list.resize(count, FWD_OBJ(value)); }

		template <class Index = OBJ
		> auto del(Index && i) -> Error
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return m_list.erase(begin() + static_cast<size_t>(i)), Error_None;
			}
			else
			{
				return m_list.erase(begin() + FWD_OBJ(i).cast<size_t>()), Error_None;
			}
		}

		template <class Value = OBJ
		> auto append(Value && v) -> OBJ & { return m_list.emplace_back(FWD_OBJ(v)); }

		template <class Index = OBJ, class Value = OBJ
		> void insert(Index && i, Value && v)
		{
			if constexpr (std::is_integral_v<Index>)
			{
				m_list.insert(begin() + i, FWD_OBJ(v));
			}
			else
			{
				m_list.insert(begin() + i.cast<size_t>(), FWD_OBJ(v));
			}
		}

		template <class Value = OBJ
		> NODISCARD bool contains(Value && v) const { return ism::has(m_list, FWD_OBJ(v)); }

		template <class Value = OBJ
		> NODISCARD auto find(Value && v) -> iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJ
		> NODISCARD auto find(Value && v) const -> const_iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJ
		> NODISCARD auto lookup(Value && v) const -> OBJ { return this->lookup(FWD_OBJ(v), OBJ{}); }

		template <class Value = OBJ, class Defval = OBJ
		> NODISCARD auto lookup(Value && v, Defval && dv) const -> OBJ
		{
			if (auto const ptr{ ism::getptr(m_list, FWD_OBJ(v)) }) { return *ptr; }
			else { return FWD_OBJ(dv); }
		}

		template <class Index = OBJ
		> NODISCARD auto operator[](Index && i) const -> OBJ
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return m_list[static_cast<size_t>(i)];
			}
			else
			{
				return m_list[FWD_OBJ(i).cast<size_t>()];
			}
		}

		NODISCARD auto data() const { return m_list.data(); }

		NODISCARD bool empty() const { return m_list.empty(); }

		NODISCARD auto size() const { return m_list.size(); }

		NODISCARD auto begin() -> iterator { return m_list.begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_list.begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return m_list.cbegin(); }

		NODISCARD auto end() -> iterator { return m_list.end(); }

		NODISCARD auto end() const -> const_iterator { return m_list.end(); }

		NODISCARD auto cend() const -> const_iterator { return m_list.cend(); }
	};

	// list delete
	template <> struct DefaultDelete<ListObject> : DefaultDelete<Object> {};

	// list check
#define OBJECT_CHECK_LIST(o) (ism::typeof(o).has_feature(ism::TypeFlags_List_Subclass))

	// list handle
	CUSTOM_HANDLE(ListObject)
	{
		HANDLE_CLASS(ListObject, OBJECT_CHECK_LIST);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		NODISCARD operator storage_type & () const { return m_ptr->operator storage_type & (); }

		template <class Index = OBJ
		> auto del(Index && i) const -> Error { return m_ptr->del(FWD(i)); }

		void clear() const { m_ptr->clear(); }

		void reserve(size_t count) const { m_ptr->reserve(count); }

		void resize(size_t count) const { m_ptr->resize(count); }

		template <class Value = OBJ
		> void resize(size_t count, Value && value) const { m_ptr->resize(count, FWD(value)); }

		template <class Value = OBJ
		> auto append(Value && v) const -> OBJ & { return m_ptr->append(FWD(v)); }

		template <class Index = OBJ, class Value = OBJ
		> void insert(Index && i, Value && v) { return m_ptr->insert(FWD(i), FWD(v)); }

		template <class Value = OBJ
		> NODISCARD bool contains(Value && v) const { return m_ptr->contains(FWD(v)); }

		template <class Value = OBJ
		> NODISCARD auto find(Value && v) -> iterator { return m_ptr->find(FWD(v)); }

		template <class Value = OBJ
		> NODISCARD auto find(Value && v) const -> const_iterator { return m_ptr->find(FWD(v)); }

		template <class Value = OBJ
		> NODISCARD auto lookup(Value && v) const -> OBJ { return m_ptr->lookup(FWD(v)); }

		template <class Value = OBJ, class Defval = OBJ
		> NODISCARD auto lookup(Value && v, Defval && dv) const -> OBJ { return m_ptr->lookup(FWD(v), FWD(dv)); }

		template <class Index = OBJ
		> NODISCARD auto operator[](Index && i) const -> OBJ { return m_ptr->operator[](FWD(i)); }

		NODISCARD auto data() const { return m_ptr->data(); }

		NODISCARD bool empty() const { return m_ptr->empty(); }

		NODISCARD auto size() const { return m_ptr->size(); }

		NODISCARD auto begin() -> iterator { return m_ptr->begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_ptr->begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return m_ptr->cbegin(); }

		NODISCARD auto end() -> iterator { return m_ptr->end(); }

		NODISCARD auto end() const -> const_iterator { return m_ptr->end(); }

		NODISCARD auto cend() const -> const_iterator { return m_ptr->cend(); }
	};
}

#endif // !_ISM_LIST_OBJECT_HPP_
