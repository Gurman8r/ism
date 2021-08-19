#ifndef _ISM_CAPSULE_OBJECT_HPP_
#define _ISM_CAPSULE_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// capsule
namespace ism
{
	// capsule object
	class ISM_API CapsuleObject : public Object
	{
		ISM_OBJECT(CapsuleObject, Object);

	protected:
		static void _bind_class(OBJ scope);

	public:
		void * m_pointer{}, * m_context{};

		destructor m_closure{};

		String m_name{}, m_doc{};

		virtual ~CapsuleObject() override
		{
			if (m_closure) { m_closure((Object *)m_pointer); }
		}

		CapsuleObject() : Object{ get_class() } {}

		CapsuleObject(nullptr_t) : CapsuleObject{}
		{
			m_pointer = nullptr;
			m_context = nullptr;
			m_closure = nullptr;
		}

		CapsuleObject(void const * value, destructor closure = nullptr) : CapsuleObject{}
		{
			m_pointer = (void *)value;
			m_context = nullptr;
			m_closure = closure;
		}

		CapsuleObject(void const * value, void(*closure)(void *)) : CapsuleObject{}
		{
			m_pointer = (void *)value;
			m_context = nullptr;
			m_closure = (destructor)[](Object * o)
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
			m_closure = (destructor)[](Object * o)
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
#define ISM_CHECK_CAPSULE(o) (ism::isinstance<ism::CAPSULE>(o))

	// capsule handle
	template <> class Handle<CapsuleObject> : public Ref<CapsuleObject>
	{
		ISM_HANDLE(CapsuleObject, ISM_CHECK_CAPSULE);

	public:
		template <class T
		> NODISCARD operator T * () const { return get_pointer<T>(); }

		template <class T = void
		> NODISCARD auto get_pointer() const { return static_cast<T *>(m_ptr->m_pointer); }

		void set_pointer(void const * value) { m_ptr->m_pointer = (void *)value; }

		NODISCARD auto name() const { return attr("__name__"); }
	};
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
