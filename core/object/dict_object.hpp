#ifndef _ISM_DICT_OBJECT_HPP_
#define _ISM_DICT_OBJECT_HPP_

#include <core/object/type_object.hpp>

// dict
namespace ism
{
	// dict object
	class ISM_API DictObject : public Object
	{
		OBJECT_COMMON(DictObject, Object);

		friend class DICT;

	public:
		HashMap<OBJ, OBJ> m_dict{};

		using storage_type		= decltype(m_dict);
		using hasher			= storage_type::hasher;
		using key_equal			= storage_type::key_equal;
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_dict); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_dict); }

		NODISCARD operator storage_type & () const { return const_cast<storage_type &>(m_dict); }

		DictObject() noexcept : m_dict{} {}

		DictObject(storage_type const & value) : m_dict{ value } {}

		DictObject(storage_type && value) noexcept : m_dict{ std::move(value) } {}

		DictObject(std::initializer_list<std::pair<OBJ, OBJ>> init) : m_dict{}
		{
			for (auto const & e : init) { m_dict.insert(e); }
		}

		void clear() { m_dict.clear(); }

		void reserve(size_t count) { m_dict.reserve(count); }

		template <class Index = OBJ
		> auto del(Index && i) const -> Error_ { return m_dict.erase(FWD_OBJ(i)), Error_None; }

		template <class Index = OBJ, class Value = OBJ
		> bool insert(Index && i, Value && v) const { return m_dict.try_emplace(FWD_OBJ(i), FWD_OBJ(v)).second; }

		template <class Index = OBJ
		> NODISCARD bool contains(Index && i) const { return find(FWD(i)) != end(); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) -> iterator { return m_dict.find(FWD_OBJ(i)); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) const -> const_iterator { return m_dict.find(FWD_OBJ(i)); }

		template <class Index = OBJ
		> NODISCARD auto lookup(Index && i) const -> OBJ { return this->lookup(FWD_OBJ(i), OBJ{}); }

		template <class Index = OBJ, class Defval = OBJ
		> NODISCARD auto lookup(Index && i, Defval && dv) const -> OBJ
		{
			if (auto const ptr{ ism::getptr(m_dict, FWD_OBJ(i)) }) { return *ptr; }
			else { return FWD_OBJ(dv); }
		}

		template <class Index = OBJ
		> NODISCARD auto operator[](Index && i) -> OBJ & { return m_dict[FWD_OBJ(i)]; }

		NODISCARD bool empty() const { return m_dict.empty(); }

		NODISCARD auto size() const { return m_dict.size(); }

		NODISCARD auto begin() -> iterator { return m_dict.begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_dict.begin(); }

		NODISCARD auto end() -> iterator { return m_dict.end(); }

		NODISCARD auto end() const -> const_iterator { return m_dict.end(); }
	};

	// dict delete
	template <> struct DefaultDelete<DictObject> : DefaultDelete<Object> {};

	// dict check
#define OBJECT_CHECK_DICT(o) (ism::typeof(o).has_feature(ism::TypeFlags_Dict_Subclass))

	// dict ref
	class DICT : public Ref<DictObject>
	{
		REF_COMMON(DICT, OBJECT_CHECK_DICT);

	public:
		using storage_type		= value_type::storage_type;
		using hasher			= value_type::hasher;
		using key_equal			= value_type::key_equal;
		using iterator			= value_type::iterator;
		using const_iterator	= value_type::const_iterator;

		NODISCARD operator storage_type & () const { return m_ptr->operator storage_type &(); }

		void clear() const { m_ptr->clear(); }

		void reserve(size_t count) const { m_ptr->reserve(count); }

		template <class Index = OBJ
		> auto del(Index && i) const -> Error_ { return m_ptr->erase(FWD(i)); }

		template <class Index = OBJ, class Value = OBJ
		> bool insert(Index && i, Value && v) const { return m_ptr->try_emplace(FWD(i), FWD(v)); }

		template <class Index = OBJ
		> NODISCARD bool contains(Index && i) const { return m_ptr->contains(FWD(i)); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) -> iterator { return m_ptr->find(FWD(i)); }

		template <class Index = OBJ
		> NODISCARD auto find(Index && i) const -> const_iterator { return m_ptr->find(FWD(i)); }

		template <class Index = OBJ
		> NODISCARD auto lookup(Index && i) const -> OBJ { return m_ptr->lookup(FWD(i)); }

		template <class Index = OBJ, class Defval = OBJ
		> NODISCARD auto lookup(Index && i, Defval && dv) const -> OBJ { return m_ptr->lookup(FWD(i), FWD(dv)); }

		template <class Index = OBJ
		> NODISCARD auto operator[](Index && i) const -> OBJ & { return m_ptr->operator[](i); }

		NODISCARD bool empty() const { return m_ptr->empty(); }

		NODISCARD auto size() const { return m_ptr->size(); }

		NODISCARD auto begin() -> iterator { return m_ptr->begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_ptr->begin(); }

		NODISCARD auto end() -> iterator { return m_ptr->end(); }

		NODISCARD auto end() const -> const_iterator { return m_ptr->end(); }
	};
}

#endif // !_ISM_DICT_OBJECT_HPP_
