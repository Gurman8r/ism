#ifndef _ISM_CAPSULE_OBJECT_HPP_
#define _ISM_CAPSULE_OBJECT_HPP_

#include <core/object/type_object.hpp>

// capsule
namespace ism
{
	// capsule object
	class ISM_API CapsuleObject : public Object
	{
		OBJECT_CLASS(CapsuleObject, Object);

	public:
		void * m_pointer{}, * m_context{};

		delfunc m_closure{};

		virtual ~CapsuleObject() override { if (m_closure) { m_closure(this); } }

		CapsuleObject() noexcept : Object{} {}

		CapsuleObject(nullptr_t) : CapsuleObject{}
		{
			m_pointer = nullptr;
			m_context = nullptr;
			m_closure = nullptr;
		}

		CapsuleObject(void const * value, delfunc closure = nullptr) : CapsuleObject{}
		{
			m_pointer = (void *)value;
			m_context = nullptr;
			m_closure = closure;
		}

		CapsuleObject(void const * value, void(*closure)(void *)) : CapsuleObject{}
		{
			m_pointer = (void *)value;
			m_context = (void *)closure;
			m_closure = (delfunc)[](Object * o)
			{
				if (auto self{ dynamic_cast<CapsuleObject *>(o) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->m_context) };

					closure(self->m_pointer);
				}
			};
		}

		CapsuleObject(void(*closure)()) : CapsuleObject{}
		{
			m_pointer = (void *)closure;
			m_context = nullptr;
			m_closure = (delfunc)[](Object * o)
			{
				if (auto self{ dynamic_cast<CapsuleObject *>(o) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->m_pointer) };

					closure();
				}
			};
		}
	};

	// capsule delete
	template <> struct DefaultDelete<CapsuleObject> : DefaultDelete<Object> {};

	// capsule check
#define OBJECT_CHECK_CAPSULE(o) (ism::isinstance<ism::CAPSULE>(o))

	// capsule handle
	CUSTOM_HANDLE(CapsuleObject)
	{
		HANDLE_CLASS(CapsuleObject, OBJECT_CHECK_CAPSULE);

	public:
		template <class T
		> NODISCARD operator T * () const { return get_pointer<T>(); }

		template <class T = void
		> NODISCARD auto get_pointer() const { return static_cast<T *>(m_ptr->m_pointer); }

		void set_pointer(void const * value) { m_ptr->m_pointer = (void *)value; }
	};
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
