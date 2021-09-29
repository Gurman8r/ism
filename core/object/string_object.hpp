#ifndef _ISM_STRING_OBJECT_HPP_
#define _ISM_STRING_OBJECT_HPP_

#include <core/object/type_object.hpp>

// string
namespace ism
{
	// string object
	class ISM_API StringObject : public Object
	{
		OBJECT_COMMON(StringObject, Object);

	public:
		String m_string{};

		using storage_type = decltype(m_string);

		using iterator = storage_type::iterator;

		using const_iterator = storage_type::const_iterator;

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_string); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_string); }

		NODISCARD operator storage_type & () const { return const_cast<storage_type &>(m_string); }

		StringObject() noexcept : Object{} {}

		StringObject(storage_type const & value) : StringObject{} { m_string = value; }

		StringObject(storage_type && value) noexcept : StringObject{} { m_string = std::move(value); }

		StringObject(cstring value) : StringObject{} { m_string = value; }

		StringObject(cstring value, size_t n) : StringObject{} { m_string = { value, n }; }

		StringObject(StringName const & value) : StringObject{ value.string() } {}

		StringObject(StringName && value) noexcept : StringObject{ std::move(value).string() } {}

		StringObject(std::initializer_list<char> init) : StringObject{} { m_string = init; }

		template <class T> StringObject(Ref<T> const & value) : StringObject{}
		{
			if (STR::check_(value))
			{
				m_string = (storage_type)(STR)value;
			}
			else if (TYPE t{ api::typeof(value) }; t->tp_str)
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

		NODISCARD auto cbegin() const -> const_iterator { return m_string.cbegin(); }

		NODISCARD auto end() -> iterator { return m_string.end(); }

		NODISCARD auto end() const -> const_iterator { return m_string.end(); }

		NODISCARD auto cend() const -> const_iterator { return m_string.cend(); }
	};

	// string delete
	template <> struct DefaultDelete<StringObject> : DefaultDelete<Object> {};

	// string check
#define OBJECT_CHECK_STR(o) (ism::api::typeof(o).has_feature(ism::TypeFlags_Str_Subclass))

	// string ref
	MAKE_SPECIAL_REF(StringObject)
	{
		REF_COMMON(StringObject, OBJECT_CHECK_STR);

	public:
		using storage_type = value_type::storage_type;

		using iterator = value_type::iterator;

		using const_iterator = value_type::const_iterator;

		NODISCARD operator storage_type & () const { return m_ptr->operator storage_type & (); }

		void reserve(size_t count) { m_ptr->reserve(count); }

		void resize(size_t count) { m_ptr->resize(count); }

		NODISCARD auto c_str() const { return m_ptr->c_str(); }

		NODISCARD auto data() const { return m_ptr->data(); }

		NODISCARD bool empty() const { return m_ptr->empty(); }

		NODISCARD auto size() const { return m_ptr->size(); }

		NODISCARD auto begin() -> iterator { return m_ptr->begin(); }

		NODISCARD auto begin() const -> const_iterator { return m_ptr->begin(); }

		NODISCARD auto cbegin() const -> const_iterator { return m_ptr->cbegin(); }

		NODISCARD auto end() -> iterator { return m_ptr->end(); }

		NODISCARD auto end() const -> const_iterator { return m_ptr->end(); }

		NODISCARD auto cend() const -> const_iterator { return m_ptr->cend(); }
	};
}

// functions
namespace ism::api
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
