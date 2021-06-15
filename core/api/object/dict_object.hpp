#ifndef _ISM_DICT_OBJECT_HPP_
#define _ISM_DICT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// dict object
class NODISCARD ISM_API ism::api::DictObject : public BaseObject
{
	ISM_OBJECT(DictObject, BaseObject);

protected:
	static void _bind_class(TypeObject & t);

	HashMap<OBJECT, OBJECT> m_dict{};

public:
	using storage_type = decltype(m_dict);

	using iterator = storage_type::iterator;

	using const_iterator = storage_type::const_iterator;

	DictObject(storage_type const & v) : base_type{ &ob_type_static }, m_dict{ v } {}

	DictObject(storage_type && v) noexcept : base_type{ &ob_type_static }, m_dict{ std::move(v) } {}

	DictObject(std::initializer_list<std::pair<OBJECT, OBJECT>> init) : self_type{}
	{
		m_dict.reserve(init.size());

		for (auto const & e : init) { m_dict.insert(e); }
	}

	NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_dict); }

	NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_dict); }
};

// dict deleter
template <> struct ism::DefaultDelete<ism::api::DictObject> : ism::DefaultDelete<ism::api::BaseObject> {};

// dict handle
template <> class ism::api::Handle<ism::api::DictObject> : public ism::api::BaseHandle<ism::api::DictObject>
{
	ISM_HANDLE(DictObject);

public:
	Handle() = default;

	~Handle() = default;

	using storage_type = DictObject::storage_type;

	using iterator = DictObject::iterator;

	using const_iterator = DictObject::const_iterator;

	void clear() const { (**m_ref).clear(); }

	void reserve(size_t count) const { (**m_ref).reserve(count); }

	template <class Index = OBJECT
	> auto find(Index && i) -> iterator { return (**m_ref).find(FWD_OBJ(i)); }

	template <class Index = OBJECT
	> auto find(Index && i) const -> const_iterator { return (**m_ref).find(FWD_OBJ(i)); }

	template <class Index = OBJECT
	> bool contains(Index && i) const { return find(FWD(i)) != end(); }

	template <class Index = OBJECT
	> auto lookup(Index && i) const -> OBJECT *
	{
		auto const it{ find(FWD(i)) };
		return (it != end()) ? const_cast<OBJECT *>(&(it->second)) : nullptr;
	}

	template <class Index = OBJECT, class Value = OBJECT
	> bool insert(Index && i, Value && v) const
	{
		return (**m_ref).try_emplace(FWD_OBJ(i), FWD_OBJ(v)).second;
	}

	template <class Index = OBJECT
	> auto operator[](Index && i) const -> OBJECT &
	{
		return (**m_ref)[FWD_OBJ(i)];
	}

	template <class Index = OBJECT, class Value = OBJECT
	> auto set(Index && i, Value && v) const -> Error
	{
		return ((**m_ref)[FWD_OBJ(i)] = FWD_OBJ(v)), Error_None;
	}

	template <class Index = OBJECT
	> auto del(Index && i) const -> Error
	{
		return (**m_ref).erase(FWD_OBJ(i)), Error_None;
	}

	NODISCARD bool empty() const { return (**m_ref).empty(); }

	NODISCARD auto size() const { return (**m_ref).size(); }

public:
	NODISCARD auto begin() -> iterator { return (**m_ref).begin(); }

	NODISCARD auto begin() const -> const_iterator { return (**m_ref).begin(); }

	NODISCARD auto cbegin() const -> const_iterator { return (**m_ref).cbegin(); }

	NODISCARD auto end() -> iterator { return (**m_ref).end(); }

	NODISCARD auto end() const -> const_iterator { return (**m_ref).end(); }

	NODISCARD auto cend() const -> const_iterator { return (**m_ref).cend(); }
};

#endif // !_ISM_DICT_OBJECT_HPP_
