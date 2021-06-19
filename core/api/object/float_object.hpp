#ifndef _ISM_FLOAT_OBJECT_HPP_
#define _ISM_FLOAT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// float object
namespace ism::api
{
	class ISM_API FloatObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(FloatObject, BaseObject);

	protected:
		static void _bind_methods(TypeObject & t);

	public:
		double_t m_float{};

		using storage_type = decltype(m_float);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_float); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit FloatObject(TypeObject const * t, T v) : base_type{ t }, m_float{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatObject(T const v) : base_type{ get_type_static() }, m_float{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> FloatObject & operator=(T const v) { m_float = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_float); }
	};
}

// float delete
namespace ism { template <> struct DefaultDelete<api::FloatObject> : DefaultDelete<api::BaseObject> {}; }

// float handle
namespace ism::api
{
	template <> class Handle<FloatObject> : public BaseHandle<FloatObject>
	{
		ISM_HANDLE(FloatObject);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = FloatObject::storage_type;

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> Handle(T const v) { instance(v); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> operator T () const { return (T)(**m_ref); }
	};
}

#endif // !_ISM_FLOAT_OBJECT_HPP_
