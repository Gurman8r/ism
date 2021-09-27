#ifndef _ISM_INT_OBJECT_HPP_
#define _ISM_INT_OBJECT_HPP_

#include <core/object/type_object.hpp>

// bool
#define OBJ_TRUE		(ism::INT(&ism::IntObject::g_true))
#define OBJ_FALSE		(ism::INT(&ism::IntObject::g_false))
#define OBJ_BOOL(b)		((b) ? OBJ_TRUE : OBJ_FALSE)

// int
namespace ism
{
	// int object
	class ISM_API IntObject : public Object
	{
		OBJECT_COMMON(IntObject, Object);

	public:
		int64_t m_int{};

		using storage_type = decltype(m_int);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_int); }

		IntObject() noexcept : Object{} {}

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> explicit IntObject(T const value) : IntObject{} { m_int = static_cast<storage_type>(value); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> IntObject & operator=(T const value) { m_int = static_cast<storage_type>(value); return (*this); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_int); }

	public:
		static IntObject g_true; // global true

		static IntObject g_false; // global false
	};

	// int delete
	template <> struct DefaultDelete<IntObject> : DefaultDelete<Object> {};

	// int check
#define OBJECT_CHECK_INT(o) (ism::typeof(o).has_feature(ism::TypeFlags_Int_Subclass))

	// int handle
	DECL_CUSTOM_REF(IntObject)
	{
		REF_COMMON(IntObject, OBJECT_CHECK_INT);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> CustomRef(T const value) { instance(value); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> operator T () const { return (T)(**m_ptr); }
	};
}

#endif // !_ISM_INT_OBJECT_HPP_
