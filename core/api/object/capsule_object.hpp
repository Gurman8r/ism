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
			void * pointer{}, * context{};

			destructor closure{};

			String name{}, doc{};
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCapsule : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreCapsule, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	protected:
		detail::capsule_record m_capsule{};

	public:
		using storage_type = decltype(m_capsule);

		virtual ~CoreCapsule() override
		{
			if (m_capsule.closure) { m_capsule.closure((CoreObject *)m_capsule.pointer); }
		}

		CoreCapsule(nullptr_t) : self_type{}
		{
			m_capsule.pointer = nullptr;
			m_capsule.context = nullptr;
			m_capsule.closure = nullptr;
		}

		CoreCapsule(void const * value, destructor closure = nullptr) : self_type{}
		{
			m_capsule.pointer = (void *)value;
			m_capsule.context = nullptr;
			m_capsule.closure = closure;
		}

		CoreCapsule(void const * value, void(*closure)(void *)) : self_type{}
		{
			m_capsule.pointer = (void *)value;
			m_capsule.context = nullptr;
			m_capsule.closure = (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->get_context()) };

					closure(self->get_pointer());
				}
			};
		}

		CoreCapsule(void(*closure)()) : self_type{}
		{
			m_capsule.pointer = (void *)closure;
			m_capsule.context = nullptr;
			m_capsule.closure = (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->get_pointer()) };

					closure();
				}
			};
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_capsule); }

		NODISCARD auto get_closure() const -> destructor { return m_capsule.closure; }
		
		NODISCARD auto get_context() const -> void * { return m_capsule.context; }
		
		NODISCARD auto get_pointer() const -> void * { return m_capsule.pointer; }
		
		NODISCARD auto get_doc() const -> String const & { return m_capsule.doc; }
		
		NODISCARD auto get_name() const -> String const & { return m_capsule.name; }

		void set_closure(destructor value) { m_capsule.closure = value; }
		
		void set_context(void const * value) { m_capsule.context = (void *)value; }
		
		void set_pointer(void const * value) { m_capsule.pointer = (void *)value; }
		
		void set_doc(String const & value) { m_capsule.doc = value; }
		
		void set_name(String const & value) { m_capsule.name = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
