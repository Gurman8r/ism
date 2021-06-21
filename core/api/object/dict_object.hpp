#ifndef _ISM_DICT_OBJECT_HPP_
#define _ISM_DICT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// dict
namespace ism
{
	// dict object
	class ISM_API DictObject : public BaseObject
	{
		ISM_OBJECT_TYPED(DictObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		HashMap<OBJECT, OBJECT> m_dict{};

		using storage_type = decltype(m_dict);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		using allocator_type = storage_type::allocator_type;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_dict); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_dict); }

		DictObject(std::initializer_list<std::pair<OBJECT, OBJECT>> init, allocator_type al = {}) : self_type{ al }
		{
			for (auto const & e : init) { m_dict.insert(e); }
		}

		DictObject(allocator_type al = {}) noexcept : base_type{ get_type_static() }, m_dict{ al } {}

		DictObject(storage_type const & v, allocator_type al = {}) : base_type{ get_type_static() }, m_dict{ v, al } {}

		DictObject(storage_type && v, allocator_type al = {}) noexcept : base_type{ get_type_static() }, m_dict{ std::move(v), al } {}
	};

	// dict delete
	template <> struct DefaultDelete<DictObject> : DefaultDelete<BaseObject> {};

	// dict check
#define ISM_DICT_CHECK(o) (ism::typeof(o).has_feature(TypeFlags_Dict_Subclass))

	// dict handle
	template <> class Handle<DictObject> : public BaseHandle<DictObject>
	{
		ISM_HANDLE_DEFAULT(DictObject, ISM_DICT_CHECK);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		using allocator_type = value_type::allocator_type;

		void clear() const { (**m_ptr).clear(); }

		void reserve(size_t count) const { (**m_ptr).reserve(count); }

		template <class Index = OBJECT
		> auto find(Index && i) -> iterator { return (**m_ptr).find(FWD_OBJ(i)); }

		template <class Index = OBJECT
		> auto find(Index && i) const -> const_iterator { return (**m_ptr).find(FWD_OBJ(i)); }

		template <class Index = OBJECT
		> bool contains(Index && i) const { return find(FWD(i)) != end(); }

		template <class Index = OBJECT
		> auto lookup(Index && i) const -> OBJECT * { return ism::getptr((**m_ptr), FWD_OBJ(i)); }

		template <class Index = OBJECT, class Value = OBJECT
		> bool insert(Index && i, Value && v) const { return (**m_ptr).try_emplace(FWD_OBJ(i), FWD_OBJ(v)).second; }

		template <class Index = OBJECT
		> auto operator[](Index && i) const -> OBJECT & { return (**m_ptr)[FWD_OBJ(i)]; }

		template <class Index = OBJECT
		> auto del(Index && i) const -> Error { return (**m_ptr).erase(FWD_OBJ(i)), Error_None; }

		NODISCARD bool empty() const { return (**m_ptr).empty(); }

		NODISCARD auto size() const { return (**m_ptr).size(); }

		NODISCARD auto begin() -> iterator { return (**m_ptr).begin(); }

		NODISCARD auto begin() const -> const_iterator { return (**m_ptr).begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return (**m_ptr).cbegin(); }

		NODISCARD auto end() -> iterator { return (**m_ptr).end(); }

		NODISCARD auto end() const -> const_iterator { return (**m_ptr).end(); }

		NODISCARD auto cend() const -> const_iterator { return (**m_ptr).cend(); }
	};
}

#endif // !_ISM_DICT_OBJECT_HPP_
