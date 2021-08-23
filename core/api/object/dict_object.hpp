#ifndef _ISM_DICT_OBJECT_HPP_
#define _ISM_DICT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// dict
namespace ism
{
	// dict object
	class ISM_API DictObject : public Object
	{
		ISM_OBJECT_DEFAULT(DictObject, Object);

	public:
		HashMap<OBJ, OBJ> m_dict{};

		using storage_type = decltype(m_dict);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_dict); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_dict); }

		DictObject() noexcept : Object{ get_class() }, m_dict{} {}

		DictObject(storage_type const & value) : DictObject{} { m_dict = value; }

		DictObject(storage_type && value) noexcept : DictObject{} { m_dict = std::move(value); }

		DictObject(std::initializer_list<std::pair<OBJ, OBJ>> init) : DictObject{}
		{
			for (auto const & e : init) { m_dict.insert(e); }
		}
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

		NODISCARD auto dict() const & noexcept -> storage_type & { return **CHECK(m_ptr); }

		void clear() const { dict().clear(); }

		void reserve(size_t count) const { dict().reserve(count); }

		template <class Index = OBJ
		> auto del(Index && i) const -> Error { return dict().erase(FWD_OBJ(i)), Error_None; }

		template <class Index = OBJ, class Value = OBJ
		> bool insert(Index && i, Value && v) const { return dict().try_emplace(FWD_OBJ(i), FWD_OBJ(v)).second; }

		template <class Index = OBJ, class Value = OBJ
		> bool insert_or_assign(Index && i, Value && v) const { return dict().insert_or_assign(FWD_OBJ(i), FWD_OBJ(v)).second; }

		template <class Index = OBJ
		> NODISCARD bool contains(Index && i) const { return find(FWD(i)) != end(); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) -> iterator { return dict().find(FWD_OBJ(i)); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) const -> const_iterator { return dict().find(FWD_OBJ(i)); }

		template <class Index = OBJ
		> NODISCARD auto lookup(Index && i) const -> OBJ { return lookup(FWD_OBJ(i), OBJ{}); }

		template <class Index = OBJ, class Defval = OBJ
		> NODISCARD auto lookup(Index && i, Defval && dv) const -> OBJ
		{
			if (auto const ptr{ ism::getptr(dict(), FWD_OBJ(i)) }) { return *ptr; }
			else { return FWD_OBJ(dv); }
		}

		template <class Index = OBJ
		> NODISCARD auto operator[](Index && i) const -> OBJ & { return (**m_ptr)[FWD_OBJ(i)]; }

		NODISCARD bool empty() const { return dict().empty(); }

		NODISCARD auto size() const { return dict().size(); }

		NODISCARD auto begin() -> iterator { return dict().begin(); }

		NODISCARD auto begin() const -> const_iterator { return dict().begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return dict().cbegin(); }

		NODISCARD auto end() -> iterator { return dict().end(); }

		NODISCARD auto end() const -> const_iterator { return dict().end(); }

		NODISCARD auto cend() const -> const_iterator { return dict().cend(); }
	};
}

#endif // !_ISM_DICT_OBJECT_HPP_
