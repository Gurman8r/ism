#ifndef _ISM_FLOAT_OBJECT_HPP_
#define _ISM_FLOAT_OBJECT_HPP_

#include <core/object/type_object.hpp>

// float
namespace ism
{
	// float object
	API_CLASS(FloatObject) : public Object
	{
		OBJ_COMMON(FloatObject, Object);

	public:
		double_t m_float{};

		using storage_type = decltype(m_float);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_float); }

		FloatObject() noexcept : Object{} {}

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit FloatObject(T const value) : FloatObject{} { m_float = static_cast<storage_type>(value); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatObject & operator=(T const value) { m_float = static_cast<storage_type>(value); return (*this); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_float); }
	};

	// float delete
	template <> struct DefaultDelete<FloatObject> : DefaultDelete<Object> {};

	// float check
#define OBJ_CHECK_FLOAT(o) (ism::typeof(o).has_feature(ism::TypeFlags_Float_Subclass))

	// float handle
	HANDLE_CLASS(FloatObject)
	{
		HANDLE_COMMON(FloatObject, OBJ_CHECK_FLOAT);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> Handle(T const value) { instance(value); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> operator T () const { return (T)(**m_ptr); }
	};
}

#endif // !_ISM_FLOAT_OBJECT_HPP_
