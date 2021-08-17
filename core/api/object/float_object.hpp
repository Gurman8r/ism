#ifndef _ISM_FLOAT_OBJECT_HPP_
#define _ISM_FLOAT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// float
namespace ism
{
	// float object
	class ISM_API FloatObject : public Object
	{
		ISM_OBJECT_DEFAULT(FloatObject, Object);

	protected:
		static void _bind_class(OBJ scope);

	public:
		double_t m_float{};

		using storage_type = decltype(m_float);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_float); }

		FloatObject() : base_type{ get_class() } {}

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit FloatObject(TYPE const & type, T const value) : base_type{ type }, m_float{ static_cast<storage_type>(value) } {}

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit FloatObject(T const value) : self_type{ get_class(), value } {}

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatObject & operator=(T const value) { m_float = static_cast<storage_type>(value); return (*this); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_float); }
	};

	// float delete
	template <> struct DefaultDelete<FloatObject> : DefaultDelete<Object> {};

	// float check
#define ISM_CHECK_FLOAT(o) (ism::typeof(o).has_feature(ism::TypeFlags_Float_Subclass))

	// float handle
	template <> class Handle<FloatObject> : public Ref<FloatObject>
	{
		ISM_HANDLE_DEFAULT(FloatObject, ISM_CHECK_FLOAT);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> Handle(T const value) { instance(value); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> operator T () const { return (T)(**m_ptr); }
	};
}

#endif // !_ISM_FLOAT_OBJECT_HPP_
