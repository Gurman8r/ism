#ifndef _ISM_INT_OBJECT_HPP_
#define _ISM_INT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// bool
#define ISM_True		(ism::INT(&ism::IntObject::g_true))
#define ISM_False		(ism::INT(&ism::IntObject::g_false))
#define ISM_Bool(b)		((b) ? ISM_True : ISM_False)

// int
namespace ism
{
	// int object
	class ISM_API IntObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(IntObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		int64_t m_int{};

		using storage_type = decltype(m_int);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_int); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> explicit IntObject(TYPE const & t, T const v) : base_type{ t }, m_int{ static_cast<storage_type>(v) } {}

		explicit IntObject(bool value) : self_type{ get_type_static(), value } {}
		explicit IntObject(char value) : self_type{ get_type_static(), value } {}
		explicit IntObject(wchar_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(char16_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(char32_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(int8_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(int16_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(int32_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(int64_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(uint8_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(uint16_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(uint32_t value) : self_type{ get_type_static(), value } {}
		explicit IntObject(uint64_t value) : self_type{ get_type_static(), value } {}

		IntObject(float32_t) = delete;
		IntObject(float64_t) = delete;
		IntObject(float80_t) = delete;

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> IntObject & operator=(T const v) { m_int = static_cast<storage_type>(v); return (*this); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_int); }

	public:
		static IntObject g_true; // global true

		static IntObject g_false; // global false
	};

	// int delete
	template <> struct DefaultDelete<IntObject> : DefaultDelete<BaseObject> {};

	// int check
#define ISM_INT_CHECK(o) (ism::typeof(o).has_feature(ism::TypeFlags_Int_Subclass))

	// int handle
	template <> class Handle<IntObject> : public BaseHandle<IntObject>
	{
		ISM_HANDLE_DEFAULT(IntObject, ISM_INT_CHECK);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> Handle(T const v) { instance(v); }

		template <class T, class = std::enable_if_t<std::is_integral_v<T>>
		> operator T () const { return (T)(**m_ptr); }
	};
}

#endif // !_ISM_INT_OBJECT_HPP_
