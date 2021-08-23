#ifndef _ISM_LIST_OBJECT_HPP_
#define _ISM_LIST_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// list
namespace ism
{
	// list object
	class ISM_API ListObject : public Object
	{
		ISM_OBJECT(ListObject, Object);

	public:
		Vector<OBJ> m_list{};

		using storage_type = decltype(m_list);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_list); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_list); }

		template <class Iter
		> ListObject(Iter first, Iter last) : ListObject{} { m_list = { first, last }; }

		ListObject() noexcept : Object{ get_class_static() } {}

		ListObject(storage_type const & value) : ListObject{} { m_list = value; }

		ListObject(storage_type && value) noexcept : ListObject{} { m_list = std::move(value); }

		ListObject(std::initializer_list<OBJ> init) : ListObject{}
		{
			m_list.reserve(init.size());

			for (auto const & e : init) { m_list.push_back(e); }
		}
	};

	// list delete
	template <> struct DefaultDelete<ListObject> : DefaultDelete<Object> {};

	// list check
#define ISM_CHECK_LIST(o) (ism::typeof(o).has_feature(ism::TypeFlags_List_Subclass))

	// list handle
	template <> class Handle<ListObject> : public Ref<ListObject>
	{
		ISM_HANDLE(ListObject, ISM_CHECK_LIST);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		NODISCARD auto list() const & noexcept -> storage_type & { return **CHECK(m_ptr); }

		template <class Index = OBJ
		> auto del(Index && i) const -> Error
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return list().erase(begin() + static_cast<size_t>(i)), Error_None;
			}
			else
			{
				return list().erase(begin() + FWD_OBJ(i).cast<size_t>()), Error_None;
			}
		}

		void clear() const { list().clear(); }

		void reserve(size_t count) const { list().reserve(count); }

		void resize(size_t count) const { list().resize(count); }

		template <class Value = OBJ
		> void resize(size_t count, Value && value) const { list().resize(count, FWD_OBJ(value)); }

		template <class Value = OBJ
		> void append(Value && v) const { list().emplace_back(FWD_OBJ(v)); }

		template <class Index = OBJ, class Value = OBJ
		> void insert(Index && i, Value && v)
		{
			if constexpr (std::is_integral_v<Index>)
			{
				list().insert(begin() + i, FWD_OBJ(v));
			}
			else
			{
				list().insert(begin() + i.cast<size_t>(), FWD_OBJ(v));
			}
		}

		template <class Value = OBJ
		> NODISCARD bool contains(Value && v) const { return ism::has((**m_ptr), FWD_OBJ(v)); }

		template <class Value = OBJ
		> NODISCARD auto find(Value && v) -> iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJ
		> NODISCARD auto find(Value && v) const -> const_iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJ
		> NODISCARD auto lookup(Value && v) const -> OBJ { return lookup(FWD_OBJ(v), OBJ{}); }

		template <class Value = OBJ, class Defval = OBJ
		> NODISCARD auto lookup(Value && v, Defval && dv) -> OBJ
		{
			if (auto const ptr{ ism::getptr(list(), FWD_OBJ(v)) };) { return *ptr; }
			else { return FWD_OBJ(dv); }
		}

		template <class Index = OBJ
		> NODISCARD auto operator[](Index && i) const -> OBJ
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return list()[static_cast<size_t>(i)];
			}
			else
			{
				return list()[FWD_OBJ(i).cast<size_t>()];
			}
		}

		NODISCARD auto data() const { return list().data(); }

		NODISCARD bool empty() const { return list().empty(); }

		NODISCARD auto size() const { return list().size(); }

		NODISCARD auto begin() -> iterator { return list().begin(); }

		NODISCARD auto begin() const -> const_iterator { return list().begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return list().cbegin(); }

		NODISCARD auto end() -> iterator { return list().end(); }

		NODISCARD auto end() const -> const_iterator { return list().end(); }

		NODISCARD auto cend() const -> const_iterator { return list().cend(); }
	};
}

#endif // !_ISM_LIST_OBJECT_HPP_
