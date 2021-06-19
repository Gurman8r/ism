#ifndef _ISM_LIST_OBJECT_HPP_
#define _ISM_LIST_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// list object
namespace ism::api
{
	class ISM_API ListObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(ListObject, BaseObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
		Vector<OBJECT> m_list{};

		using storage_type = decltype(m_list);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_list); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_list); }

		ListObject(storage_type const & v) : base_type{ get_type_static() }, m_list{ v } {}

		ListObject(storage_type && v) noexcept : base_type{ get_type_static() }, m_list{ std::move(v) } {}

		ListObject(std::initializer_list<OBJECT> init) : self_type{}
		{
			m_list.reserve(init.size());

			for (auto const & e : init) { m_list.push_back(e); }
		}
	};
}

// list delete
namespace ism { template <> struct DefaultDelete<api::ListObject> : DefaultDelete<api::BaseObject> {}; }

// list handle
namespace ism::api
{
	template <> class Handle<ListObject> : public BaseHandle<ListObject>
	{
		ISM_HANDLE(ListObject);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = ListObject::storage_type;

		using iterator = ListObject::iterator;

		using const_iterator = ListObject::const_iterator;

		void clear() const { (**m_ref).clear(); }

		void reserve(size_t count) const { (**m_ref).reserve(count); }

		void resize(size_t count) const { (**m_ref).resize(count); }

		template <class Value = OBJECT
		> void resize(size_t count, Value && value) const { (**m_ref).resize(count, FWD_OBJ(value)); }

		template <class Value = OBJECT
		> void append(Value && v) const { (**m_ref).emplace_back(FWD_OBJ(v)); }

		template <class Value = OBJECT
		> bool contains(Value && v) const { return end() != std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJECT
		> auto find(Value && v) -> iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJECT
		> auto find(Value && v) const -> const_iterator { return std::find(begin(), end(), FWD_OBJ(v)); }

		template <class Value = OBJECT
		> void insert(size_t i, Value && v) const { (**m_ref).insert(begin() + i, FWD_OBJ(v)); }

		template <class Value = OBJECT
		> void insert(OBJECT const & i, Value && v) { (**m_ref).insert(begin() + i.cast<size_t>(), FWD_OBJ(v)); }

		template <class Index = OBJECT
		> auto operator[](Index && i) const -> OBJECT &
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

		template <class Index = OBJECT
		> auto del(Index && i) const -> Error
		{
			if constexpr (std::is_integral_v<Index>)
			{
				return (**m_ref).erase(begin() + static_cast<size_t>(i)), Error_None;
			}
			else
			{
				return (**m_ref).erase(begin() + FWD_OBJ(i).cast<size_t>()), Error_None;
			}
		}

		NODISCARD auto data() const { return (**m_ref).data(); }

		NODISCARD bool empty() const { return (**m_ref).empty(); }

		NODISCARD auto size() const { return (**m_ref).size(); }

		NODISCARD auto front() const -> OBJECT & { return (**m_ref).front(); }

		NODISCARD auto back() const -> OBJECT & { return (**m_ref).back(); }

	public:
		NODISCARD auto begin() -> iterator { return (**m_ref).begin(); }

		NODISCARD auto begin() const -> const_iterator { return (**m_ref).begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return (**m_ref).cbegin(); }

		NODISCARD auto end() -> iterator { return (**m_ref).end(); }

		NODISCARD auto end() const -> const_iterator { return (**m_ref).end(); }

		NODISCARD auto cend() const -> const_iterator { return (**m_ref).cend(); }
	};
}

#endif // !_ISM_LIST_OBJECT_HPP_
