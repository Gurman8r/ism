#ifndef _ISM_CAPSULE_OBJECT_HPP_
#define _ISM_CAPSULE_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// capsule object
namespace ism::api
{
	class ISM_API CapsuleObject : public BaseObject
	{
		ISM_OBJECT_DEFAULT(CapsuleObject, BaseObject);

	protected:
		static void _bind_methods(TypeObject & t);

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
}

// capsule delete
namespace ism { template <> struct DefaultDelete<api::CapsuleObject> : DefaultDelete<api::BaseObject> {}; }

// capsule handle
namespace ism::api
{
	template <> class Handle<CapsuleObject> : public BaseHandle<CapsuleObject>
	{
		ISM_HANDLE(CapsuleObject);

	public:
		Handle() = default;

		~Handle() = default;

		template <class T
		> NODISCARD operator T * () const { return this->get_pointer<T>(); }

		template <class T = void
		> NODISCARD auto get_pointer() const { return static_cast<T *>(m_ref->m_pointer); }

		void set_pointer(void const * value) { m_ref->m_pointer = (void *)value; }

		NODISCARD OBJECT name() const { return this->attr("__name__"); }
	};
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
