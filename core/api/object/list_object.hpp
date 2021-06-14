#ifndef _ISM_LIST_OBJECT_HPP_
#define _ISM_LIST_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// list object
class NODISCARD ISM_API ism::api::ListObject : public BaseObject
{
	ISM_OBJECT(ListObject, BaseObject);

protected:
	static void _bind_class(TypeObject & t);

	Vector<OBJECT> m_list{};

public:
	using storage_type = decltype(m_list);

	using iterator = storage_type::iterator;

	using const_iterator = storage_type::const_iterator;

	ListObject(storage_type const & v) : base_type{ &ob_type_static }, m_list{ v } {}

	ListObject(storage_type && v) noexcept : base_type{ &ob_type_static }, m_list{ std::move(v) } {}

	ListObject(std::initializer_list<OBJECT> init) : self_type{}
	{
		m_list.reserve(init.size());

		for (auto const & e : init) { m_list.push_back(e); }
	}

	NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_list); }

	NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_list); }
};

// list deleter
template <> struct ism::DefaultDelete<ism::api::ListObject> : DefaultDelete<ism::api::BaseObject> {};

// list handle
template <> class ism::api::Handle<ism::api::ListObject> : public BaseHandle<ListObject>
{
	ISM_HANDLE(ListObject);

public:
	Handle() = default;

	~Handle() = default;

	using storage_type = ListObject::storage_type;

	using iterator = ListObject::iterator;

	using const_iterator = ListObject::const_iterator;

	NODISCARD auto data() const { return (**m_ref).data(); }

	NODISCARD bool empty() const { return (**m_ref).empty(); }

	NODISCARD auto size() const { return (**m_ref).size(); }

	NODISCARD auto front() const { return (**m_ref).front(); }

	NODISCARD auto back() const { return (**m_ref).back(); }

	void reserve(size_t count) const { (**m_ref).reserve(count); }

	void resize(size_t count) const { (**m_ref).resize(count); }

	void erase(size_t i) const { (**m_ref).erase(begin() + i); }

	void erase(OBJECT const & i) const { (**m_ref).erase(begin() + i.cast<size_t>()); }

	template <class Value = OBJECT
	> void append(Value && v) const { (**m_ref).emplace_back(FWD_OBJ(v)); }

	template <class Value = OBJECT
	> bool contains(Value && v) const { return end() != std::find(begin(), end(), FWD_OBJ(v)); }

	template <class Value = OBJECT
	> auto find(Value && v) const { return std::find(begin(), end(), FWD_OBJ(v)); }

	template <class Value = OBJECT
	> void insert(size_t i, Value && v) const { (**m_ref).insert(begin() + i, FWD_OBJ(v)); }

	template <class Value = OBJECT
	> void insert(OBJECT const & i, Value && v) { (**m_ref).insert(begin() + i.cast<size_t>(), FWD_OBJ(v)); }

	template <class Index = OBJECT
	> auto operator[](Index && i) const -> OBJECT & { return this->get(FWD(i)); }

	template <class Index = OBJECT
	> auto get(Index && i) const -> OBJECT &
	{
		if constexpr (std::is_integral_v<Index>)
		{
			return (**m_ref)[static_cast<size_t>(i)];
		}
		else
		{
			return (**m_ref)[FWD_OBJ(i).cast<size_t>()];
		}
	}

	template <class Index = OBJECT, class Value = OBJECT
	> auto set(Index && i, Value && v) const -> Error
	{
		if constexpr (std::is_integral_v<Index>)
		{
			return ((**m_ref)[static_cast<size_t>(i)] = FWD_OBJ(v)), Error_None;
		}
		else
		{
			return ((**m_ref)[FWD_OBJ(i).cast<size_t>()] = FWD_OBJ(v)), Error_None;
		}
	}

public:
	NODISCARD auto begin() -> iterator { return (**m_ref).begin(); }

	NODISCARD auto begin() const -> const_iterator { return (**m_ref).begin(); }

	NODISCARD auto cbegin() const -> const_iterator { return (**m_ref).cbegin(); }

	NODISCARD auto end() -> iterator { return (**m_ref).end(); }

	NODISCARD auto end() const -> const_iterator { return (**m_ref).end(); }

	NODISCARD auto cend() const -> const_iterator { return (**m_ref).cend(); }
};

#endif // !_ISM_LIST_OBJECT_HPP_
