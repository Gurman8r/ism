#ifndef _ISM_FLOAT_OBJECT_HPP_
#define _ISM_FLOAT_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// float
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreFloat : public CoreObject
	{
		ISM_OBJECT(CoreFloat, CoreObject);

	protected:
		static void _bind_methods(CoreType & t);

	public:
		double_t m_float{};

		using storage_type = decltype(m_float);

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> explicit CoreFloat(CoreType const * t, T v) : base_type{ t }, m_float{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat(T v) : base_type{ &ob_type_static }, m_float{ static_cast<storage_type>(v) } {}

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> CoreFloat & operator=(T const v) { m_float = static_cast<storage_type>(v); return (*this); }

		template <class T = storage_type, class = std::enable_if_t<std::is_floating_point_v<T>>
		> NODISCARD operator T() const & { return static_cast<T>(m_float); }

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_float); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> struct DefaultDelete<CoreFloat> : DefaultDelete<CoreObject> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreFloat> : public BaseHandle<CoreFloat>
	{
		ISM_HANDLE(CoreFloat);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CoreFloat::storage_type;

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> Handle(T v) { revalue(v); }

		template <class T, class = std::enable_if_t<std::is_floating_point_v<T>>
		> operator T () const { return (T)(**m_ref); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_FLOAT_OBJECT_HPP_
