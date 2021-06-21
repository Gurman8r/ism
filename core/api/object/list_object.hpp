#ifndef _ISM_LIST_OBJECT_HPP_
#define _ISM_LIST_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// list
namespace ism
{
	// list object
	class ISM_API ListObject : public BaseObject
	{
		ISM_OBJECT_TYPED(ListObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		Vector<OBJECT> m_list{};

		using storage_type = decltype(m_list);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		using allocator_type = storage_type::allocator_type;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_list); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_list); }

		template <class Iter
		> ListObject(Iter first, Iter last, allocator_type al = {}) : base_type{ get_type_static() }, m_list{ first, last, al } {}

		ListObject(std::initializer_list<OBJECT> init, allocator_type al = {}) : self_type{ al }
		{
			m_list.reserve(init.size());

			for (auto const & e : init) { m_list.push_back(e); }
		}

		ListObject(allocator_type al = {}) noexcept : base_type{ get_type_static() }, m_list{ al } {}

		ListObject(storage_type const & v, allocator_type al = {}) : base_type{ get_type_static() }, m_list{ v, al } {}

		ListObject(storage_type && v, allocator_type al = {}) noexcept : base_type{ get_type_static() }, m_list{ std::move(v), al } {}
	};

	// list delete
	template <> struct DefaultDelete<ListObject> : DefaultDelete<BaseObject> {};

	// list check
#define ISM_LIST_CHECK(o) (ism::typeof(o).has_feature(TypeFlags_List_Subclass))

	// list handle
	template <> class Handle<ListObject> : public BaseHandle<ListObject>
	{
		ISM_HANDLE_DEFAULT(ListObject, ISM_LIST_CHECK);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		using allocator_type = value_type::allocator_type;

		void clear() const { (**m_ptr).clear(); }

		void reserve(size_t count) const { (**m_ptr).reserve(count); }

		void resize(size_t count) const { (**m_ptr).resize(count); }

		template <class Value = OBJECT
		> void resize(size_t count, Value && value) const { (**m_ptr).resize(count, FWD_OBJ(value)); }

		template <class Value = OBJECT
		> void append(Value && v) const { (**m_ptr).emplace_back(FWD_OBJ(v)); }

		template <class Value = OBJECT
		> auto find(Value && v) -> iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJECT
		> auto find(Value && v) const -> const_iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJECT
		> bool contains(Value && v) const { return ism::has((**m_ptr), FWD_OBJ(v)); }

		template <class Value = OBJECT
		> auto lookup(Value && v) const -> OBJECT * { return ism::getptr((**m_ptr), FWD_OBJ(v)); }

		template <class Index = OBJECT, class Value = OBJECT
		> void insert(Index && i, Value && v)
		{
			if constexpr (std::is_integral_v<Index>)
			{
				(**m_ptr).insert(begin() + i, FWD_OBJ(v));
			}
			else
			{
				(**m_ptr).insert(begin() + i.cast<size_t>(), FWD_OBJ(v));
			}
		}

		template <class Index = OBJECT
		> auto operator[](Index && i) const -> OBJECT &
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return (**m_ptr)[static_cast<size_t>(i)];
			}
			else
			{
				return (**m_ptr)[FWD_OBJ(i).cast<size_t>()];
			}
		}

		template <class Index = OBJECT
		> auto del(Index && i) const -> Error
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return (**m_ptr).erase(begin() + static_cast<size_t>(i)), Error_None;
			}
			else
			{
				return (**m_ptr).erase(begin() + FWD_OBJ(i).cast<size_t>()), Error_None;
			}
		}

		NODISCARD auto data() const { return (**m_ptr).data(); }

		NODISCARD bool empty() const { return (**m_ptr).empty(); }

		NODISCARD auto size() const { return (**m_ptr).size(); }

		NODISCARD auto front() const -> OBJECT & { return (**m_ptr).front(); }

		NODISCARD auto back() const -> OBJECT & { return (**m_ptr).back(); }

		NODISCARD auto begin() -> iterator { return (**m_ptr).begin(); }

		NODISCARD auto begin() const -> const_iterator { return (**m_ptr).begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return (**m_ptr).cbegin(); }

		NODISCARD auto end() -> iterator { return (**m_ptr).end(); }

		NODISCARD auto end() const -> const_iterator { return (**m_ptr).end(); }

		NODISCARD auto cend() const -> const_iterator { return (**m_ptr).cend(); }
	};
}

#endif // !_ISM_LIST_OBJECT_HPP_
