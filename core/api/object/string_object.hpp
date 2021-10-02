#ifndef _ISM_STRING_OBJECT_HPP_
#define _ISM_STRING_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// string
namespace ism
{
	// string object
	class ISM_API StringObject : public Object
	{
		OBJECT_COMMON(StringObject, Object);

		friend class STR;

	public:
		String m_string{};

		using storage_type		= decltype(m_string);
		using iterator			= storage_type::iterator;
		using const_iterator	= storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_string); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_string); }

		NODISCARD operator storage_type & () const { return const_cast<storage_type &>(m_string); }

		StringObject() noexcept : m_string{} {}

		StringObject(storage_type const & value) : m_string{ value } {}

		StringObject(storage_type && value) noexcept : m_string{ std::move(value) } {}

		StringObject(cstring value) : m_string{ value } {}

		StringObject(cstring value, size_t n) : m_string{ value, n } {}

		StringObject(StringName const & value) : m_string{ value.string() } {}

		StringObject(Path const & value) : m_string{ value.string() } {}

		StringObject(std::initializer_list<char> init) : m_string{ init } {}

		template <class T> StringObject(Ref<T> const & value) : m_string{}
		{
			if (STR::check_(value))
			{
				m_string = (storage_type)(STR)value;
			}
			else if (TYPE t{ typeof(value) }; t->tp_str)
			{
				m_string = (storage_type)t->tp_str(value);
			}
		}

		void reserve(size_t count) { m_string.reserve(count); }

		void resize(size_t count) { m_string.resize(count); }

		NODISCARD auto c_str() const { return m_string.c_str(); }

		NODISCARD auto data() const { return m_string.data(); }

		NODISCARD bool empty() const { return m_string.empty(); }

		NODISCARD auto size() const { return m_string.size(); }

		NODISCARD auto begin() -> iterator { return m_string.begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_string.begin(); }

		NODISCARD auto end() -> iterator { return m_string.end(); }

		NODISCARD auto end() const -> const_iterator { return m_string.end(); }
	};

	// string delete
	template <> struct DefaultDelete<StringObject> : DefaultDelete<Object> {};

	// string check
#define OBJECT_CHECK_STR(o) (ism::typeof(o).has_feature(ism::TypeFlags_Str_Subclass))

	// string ref
	class STR : public Ref<StringObject>
	{
		REF_COMMON(STR, OBJECT_CHECK_STR);

	public:
		using storage_type		= value_type::storage_type;
		using iterator			= value_type::iterator;
		using const_iterator	= value_type::const_iterator;

		NODISCARD operator storage_type & () const { return m_ptr->operator storage_type & (); }

		void reserve(size_t count) { m_ptr->reserve(count); }

		void resize(size_t count) { m_ptr->resize(count); }

		NODISCARD auto c_str() const { return m_ptr->c_str(); }

		NODISCARD auto data() const { return m_ptr->data(); }

		NODISCARD bool empty() const { return m_ptr->empty(); }

		NODISCARD auto size() const { return m_ptr->size(); }

		NODISCARD auto begin() -> iterator { return m_ptr->begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_ptr->begin(); }

		NODISCARD auto end() -> iterator { return m_ptr->end(); }

		NODISCARD auto end() const -> const_iterator { return m_ptr->end(); }

		template <class T, class = std::enable_if_t<mpl::is_string_v<T>> // std::is_convertible_v<T, storage_type>
		> STR & operator=(T && other) noexcept
		{
			if (m_ptr) { m_ptr->m_string = FWD(value); }
			else { instance(FWD(value)); }
			return (*this);
		}
	};
}

// functions
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> NODISCARD STR repr(Ref<T> const & o) noexcept
	{
		if (!o) { return nullptr; }

		TYPE t{ typeof(o) };

		return t->tp_repr ? t->tp_repr(o) : nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// "__x__"
	inline bool is_dunder_name(OBJ name)
	{
		if (!STR::check_(name)) { return false; }

		String const & s{ STR(name) };

		size_t const n{ s.size() };

		return (n >= 5) && (s[0] == '_') && (s[1] == '_') && (s[n - 2] == '_') && (s[n - 1] == '_');
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_STRING_OBJECT_HPP_