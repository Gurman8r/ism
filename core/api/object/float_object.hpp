#ifndef _ISM_FLOAT_OBJECT_HPP_
#define _ISM_FLOAT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// float
namespace ism
{
	// float object
	class ISM_API FloatObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(FloatObject, BaseObject);

	protected:
		static void _bind_class(OBJECT scope);

	public:
		double_t m_float{};

		using storage_type = decltype(m_float);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_float); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit FloatObject(TYPE const & t, T const v) : base_type{ t }, m_float{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatObject(T const v) : base_type{ get_type_static() }, m_float{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatObject & operator=(T const v) { m_float = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_float); }
	};

	// float delete
	template <> struct DefaultDelete<FloatObject> : DefaultDelete<BaseObject> {};

	// float check
#define ISM_FLOAT_CHECK(o) (ism::typeof(o).has_feature(TypeFlags_Float_Subclass))

	// float handle
	template <> class Handle<FloatObject> : public BaseHandle<FloatObject>
	{
		ISM_HANDLE_DEFAULT(FloatObject, ISM_FLOAT_CHECK);

	public:
		using storage_type = value_type::storage_type;

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> Handle(T const v) { instance(v); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> operator T () const { return (T)(**m_ptr); }
	};
}

#endif // !_ISM_FLOAT_OBJECT_HPP_
