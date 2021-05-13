#ifndef _ISM_DICT_OBJECT_HPP_
#define _ISM_DICT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// dict
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreDict : public CoreObject
	{
		ISM_OBJECT(CoreDict, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	public:
		HashMap<OBJECT, OBJECT> m_dict{};

		using storage_type = decltype(m_dict);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		CoreDict(storage_type const & v) : base_type{ &ob_type_static }, m_dict{ v } {}

		CoreDict(storage_type && v) noexcept : base_type{ &ob_type_static }, m_dict{ std::move(v) } {}

		CoreDict(std::initializer_list<std::pair<OBJECT, OBJECT>> init) : self_type{}
		{
			m_dict.reserve(init.size());

			for (auto const & e : init) { m_dict.insert(e); }
		}

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_dict); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_dict); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct DefaultDelete<CoreDict> : DefaultDelete<CoreObject> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreDict> : public BaseHandle<CoreDict>
	{
		ISM_HANDLE(CoreDict);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CoreDict::storage_type;

		using iterator = CoreDict::iterator;

		using const_iterator = CoreDict::const_iterator;

		NODISCARD bool empty() const { return (**m_ref).empty(); }

		NODISCARD auto size() const { return (**m_ref).size(); }

		void reserve(size_t count) const { (**m_ref).reserve(count); }

		template <class Index = OBJECT
		> void erase(Index && i) const { (**m_ref).erase(object_or_cast(FWD(i))); }

		template <class Index = OBJECT
		> bool contains(Index && i) const { return end() != (**m_ref).find(object_or_cast(FWD(i))); }

		template <class Index = OBJECT
		> auto find(Index && i) const { return (**m_ref).find(object_or_cast(FWD(i))); }

		template <class Index = OBJECT, class Value = OBJECT
		> void insert(Index && i, Value && v) const { (**m_ref).try_emplace(object_or_cast(FWD(i)), object_or_cast(FWD(v))); }

		template <class Index = OBJECT
		> auto get(Index && i) const -> OBJECT
		{
			return (**m_ref)[object_or_cast(FWD(i))];
		}

		template <class Index = OBJECT
		> auto get(Index && i, OBJECT const & defval) const -> OBJECT
		{
			OBJECT name{ object_or_cast(FWD(i)) };
			return name && contains(name) ? (**m_ref)[name] : defval;
		}

		template <class Index = OBJECT, class Value = OBJECT
		> auto set(Index && i, Value && v) const -> Error
		{
			return ((**m_ref)[object_or_cast(FWD(i))] = object_or_cast(FWD(v))), Error_None;
		}

	public:
		NODISCARD auto begin() -> iterator { return (**m_ref).begin(); }

		NODISCARD auto begin() const -> const_iterator { return (**m_ref).begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return (**m_ref).cbegin(); }

		NODISCARD auto end() -> iterator { return (**m_ref).end(); }

		NODISCARD auto end() const -> const_iterator { return (**m_ref).end(); }

		NODISCARD auto cend() const -> const_iterator { return (**m_ref).cend(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_DICT_OBJECT_HPP_
