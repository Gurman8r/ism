#ifndef _ISM_DICT_OBJECT_HPP_
#define _ISM_DICT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// dict
namespace ism
{
	// dict object
	class ISM_API DictObject : public Object
	{
		ISM_OBJECT(DictObject, Object);

	protected:
		static void _bind_methods();

	public:
		HashMap<OBJ, OBJ> m_dict{};

		using storage_type = decltype(m_dict);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		using allocator_type = storage_type::allocator_type;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_dict); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_dict); }

		DictObject(std::initializer_list<std::pair<OBJ, OBJ>> init, allocator_type al = {}) : DictObject{ al }
		{
			for (auto const & e : init) { m_dict.insert(e); }
		}

		DictObject(allocator_type al = {}) noexcept : Object{ get_class_static() }, m_dict{ al } {}

		DictObject(storage_type const & v, allocator_type al = {}) : Object{ get_class_static() }, m_dict{ v, al } {}

		DictObject(storage_type && v, allocator_type al = {}) noexcept : Object{ get_class_static() }, m_dict{ std::move(v), al } {}
	};

	// dict delete
	template <> struct DefaultDelete<DictObject> : DefaultDelete<Object> {};

	// dict check
#define ISM_CHECK_DICT(o) (ism::typeof(o).has_feature(ism::TypeFlags_Dict_Subclass))

	// dict handle
	template <> class Handle<DictObject> : public Ref<DictObject>
	{
		ISM_HANDLE(DictObject, ISM_CHECK_DICT);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		using allocator_type = value_type::allocator_type;

		void clear() const { (**m_ptr).clear(); }

		void reserve(size_t count) const { (**m_ptr).reserve(count); }

		template <class Index = OBJ
		> auto del(Index && i) const -> Error { return (**m_ptr).erase(FWD_OBJ(i)), Error_None; }

		template <class Index = OBJ, class Value = OBJ
		> bool insert(Index && i, Value && v) const { return (**m_ptr).try_emplace(FWD_OBJ(i), FWD_OBJ(v)).second; }

		template <class Index = OBJ, class Value = OBJ
		> bool insert_or_assign(Index && i, Value && v) const { return (**m_ptr).insert_or_assign(FWD_OBJ(i), FWD_OBJ(v)).second; }

		template <class Index = OBJ
		> NODISCARD bool contains(Index && i) const { return find(FWD(i)) != end(); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) -> iterator { return (**m_ptr).find(FWD_OBJ(i)); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) const -> const_iterator { return (**m_ptr).find(FWD_OBJ(i)); }

		template <class Index = OBJ
		> NODISCARD auto lookup(Index && i) const -> OBJ { return lookup(FWD_OBJ(i), OBJ{}); }

		template <class Index = OBJ, class Defval = OBJ
		> NODISCARD auto lookup(Index && i, Defval && dv) const -> OBJ
		{
			if (auto const ptr{ ism::getptr(**m_ptr, FWD_OBJ(i)) }) { return *ptr; }
			else { return FWD_OBJ(dv); }
		}

		template <class Index = OBJ
		> NODISCARD auto operator[](Index && i) const -> OBJ & { return (**m_ptr)[FWD_OBJ(i)]; }

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
