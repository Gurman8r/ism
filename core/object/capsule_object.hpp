#ifndef _ISM_CAPSULE_OBJECT_HPP_
#define _ISM_CAPSULE_OBJECT_HPP_

#include <core/object/type_object.hpp>

// capsule
namespace ism
{
	// capsule object
	class ISM_API CapsuleObject : public BaseObject
	{
		OBJECT_COMMON(CapsuleObject, BaseObject);

		friend class CAPSULE;

	public:
		void const * m_pointer{}, * m_context{};

		delfunc m_closure{};

		virtual ~CapsuleObject() override { if (m_closure) { m_closure(this); } }

		CapsuleObject() noexcept {}

		CapsuleObject(nullptr_t) noexcept {}

		CapsuleObject(void const * value, delfunc closure = nullptr)
		{
			m_pointer = value;
			m_context = nullptr;
			m_closure = closure;
		}

		CapsuleObject(void const * value, void(*closure)(void *))
		{
			m_pointer = value;
			m_context = closure;
			m_closure = [](BaseObject * obj)
			{
				if (auto self{ dynamic_cast<CapsuleObject *>(obj) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->m_context) };

					closure((void *)self->m_pointer);
				}
			};
		}

		CapsuleObject(void(*closure)())
		{
			m_pointer = closure;
			m_context = nullptr;
			m_closure = (delfunc)[](BaseObject * obj)
			{
				if (auto self{ dynamic_cast<CapsuleObject *>(obj) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->m_pointer) };

					closure();
				}
			};
		}
	};

	// capsule delete
	template <> struct DefaultDelete<CapsuleObject> : DefaultDelete<BaseObject> {};

	// capsule check
#define OBJECT_CHECK_CAPSULE(o) (isinstance<CAPSULE>(o))

	// capsule ref
	class CAPSULE : public Ref<CapsuleObject>
	{
		REF_COMMON(CAPSULE, OBJECT_CHECK_CAPSULE);

	public:
		template <class T
		> operator T * () const { return get_pointer<T>(); }

		template <class T = void
		> auto get_pointer() const { return static_cast<T *>((void *)VALIDATE(m_ptr)->m_pointer); }

		void set_pointer(void const * value) const { VALIDATE(m_ptr)->m_pointer = value; }
	};
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
