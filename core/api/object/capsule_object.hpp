#ifndef _ISM_CAPSULE_OBJECT_HPP_
#define _ISM_CAPSULE_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// capsule
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// capsule object
	class ISM_API CapsuleObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(CapsuleObject, BaseObject);

	protected:
		static void _bind_class(TypeObject & t);

	public:
		void * m_pointer{}, * m_context{};

		destructor m_closure{};

		String m_name{}, m_doc{};

		virtual ~CapsuleObject() override
		{
			if (m_closure) { m_closure((BaseObject *)m_pointer); }
		}

		CapsuleObject(nullptr_t) : self_type{}
		{
			m_pointer = nullptr;
			m_context = nullptr;
			m_closure = nullptr;
		}

		CapsuleObject(void const * value, destructor closure = nullptr) : self_type{}
		{
			m_pointer = (void *)value;
			m_context = nullptr;
			m_closure = closure;
		}

		CapsuleObject(void const * value, void(*closure)(void *)) : self_type{}
		{
			m_pointer = (void *)value;
			m_context = nullptr;
			m_closure = (destructor)[](BaseObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->m_context) };

					closure(self->m_pointer);
				}
			};
		}

		CapsuleObject(void(*closure)()) : self_type{}
		{
			m_pointer = (void *)closure;
			m_context = nullptr;
			m_closure = (destructor)[](BaseObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->m_pointer) };

					closure();
				}
			};
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// capsule delete
	template <> struct DefaultDelete<CapsuleObject> : DefaultDelete<BaseObject> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// capsule check
#define ISM_CAPSULE_CHECK(o) (isinstance<CAPSULE>(o))

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// capsule handle
	template <> class Handle<CapsuleObject> : public BaseHandle<CapsuleObject>
	{
		ISM_HANDLE_DEFAULT(CapsuleObject, ISM_CAPSULE_CHECK);

	public:
		template <class T
		> NODISCARD operator T * () const { return get_pointer<T>(); }

		template <class T = void
		> NODISCARD auto get_pointer() const { return static_cast<T *>(m_ptr->m_pointer); }

		void set_pointer(void const * value) { m_ptr->m_pointer = (void *)value; }

		NODISCARD auto name() const { return attr("__name__"); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
