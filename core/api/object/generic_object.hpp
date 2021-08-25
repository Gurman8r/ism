#ifndef _ISM_GENERIC_OBJECT_HPP_
#define _ISM_GENERIC_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// generic
namespace ism
{
	// generic object
	class ISM_API GenericObject : public Object
	{
		ISM_OBJECT(GenericObject, Object);

	public:
		std::any m_any{};

		using storage_type = decltype(m_any);

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_any); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_any); }

		NODISCARD operator storage_type & () const { return const_cast<storage_type &>(m_any); }

		virtual ~GenericObject() override {}

		GenericObject() noexcept : Object{} {}

		explicit GenericObject(Any const & value) : GenericObject{} { m_any = value; }

		explicit GenericObject(Any && value) noexcept : GenericObject{} { m_any = std::move(value); }

		template <class T> GenericObject(T && value) noexcept : GenericObject{} { m_any = FWD(value); }
	};

	// generic delete
	template <> struct DefaultDelete<GenericObject> : DefaultDelete<Object> {};

	// generic check
#define ISM_CHECK_GENERIC(o) (ism::isinstance<ism::GENERIC>(o))

	// generic handle
	template <> class Handle<GenericObject> : public Ref<GenericObject>
	{
		ISM_HANDLE(GenericObject, ISM_CHECK_GENERIC);

	public:
	};
}

#endif // !_ISM_GENERIC_OBJECT_HPP_
