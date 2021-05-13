#ifndef _ISM_CAPSULE_OBJECT_HPP_
#define _ISM_CAPSULE_OBJECT_HPP_

#include <core/api/object/type_object.hpp>

// capsule
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		struct NODISCARD capsule_record
		{
			DEFAULT_COPY_AND_MOVE_CONSTRUCTABLE(capsule_record);

			void * pointer{}, * context{};

			destructor closure{};

			String name{}, doc{};
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCapsule : public CoreObject
	{
		ISM_OBJECT(CoreCapsule, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		void * m_pointer{}, * m_context{};

		destructor m_closure{};

		String m_name{}, m_doc{};

	public:
		virtual ~CoreCapsule() override
		{
			if (m_closure) { m_closure((CoreObject *)m_pointer); }
		}

		CoreCapsule(nullptr_t) : self_type{}
		{
			m_pointer = nullptr;
			m_context = nullptr;
			m_closure = nullptr;
		}

		CoreCapsule(void const * value, destructor closure = nullptr) : self_type{}
		{
			m_pointer = (void *)value;
			m_context = nullptr;
			m_closure = closure;
		}

		CoreCapsule(void const * value, void(*closure)(void *)) : self_type{}
		{
			m_pointer = (void *)value;
			m_context = nullptr;
			m_closure = (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->m_context) };

					closure(self->m_pointer);
				}
			};
		}

		CoreCapsule(void(*closure)()) : self_type{}
		{
			m_pointer = (void *)closure;
			m_context = nullptr;
			m_closure = (destructor)[](CoreObject * o)
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

	template <> struct DefaultDelete<CoreCapsule> : DefaultDelete<CoreObject> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <> class Handle<CoreCapsule> : public BaseHandle<CoreCapsule>
	{
		ISM_HANDLE(CoreCapsule);

	public:
		Handle() = default;

		~Handle() = default;

		template <class T = void
		> NODISCARD auto get_pointer() const { return static_cast<T *>((*m_ref).m_pointer); }

		void set_pointer(void const * value) { (*m_ref).m_pointer = (void *)value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
