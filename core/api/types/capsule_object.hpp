#ifndef _ISM_API_TYPES_CAPSULE_OBJECT_HPP_
#define _ISM_API_TYPES_CAPSULE_OBJECT_HPP_

#include <core/api/types/type_object.hpp>

// capsule
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	namespace detail
	{
		struct capsule_record
		{
			void * pointer{}, * context{};

			destructor closure{};

			String name{}, doc{};

			~capsule_record() { if (closure) { closure((CoreObject *)pointer); } }
		};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class NODISCARD ISM_API CoreCapsule : public CoreObject
	{
		ISM_OBJECT_DEFAULT(CoreCapsule, CoreObject);

	protected:
		static void _bind_class(CoreType & t);

	public:
		using storage_type = detail::capsule_record;

	public:
		CoreCapsule(nullptr_t) : self_type{}
		{
			m_data = { nullptr, nullptr, nullptr };
		}

		CoreCapsule(void const * value, destructor closure = nullptr) : self_type{}
		{
			m_data = { (void *)value, nullptr, closure };
		}

		CoreCapsule(void const * value, void(*closure)(void *)) : self_type{}
		{
			m_data = { (void *)value, (void *)closure, (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->get_context()) };

					closure(self->get_pointer());
				}
			} };
		}

		CoreCapsule(void(*closure)()) : self_type{}
		{
			m_data = { (void *)closure, nullptr, (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->get_pointer()) };

					closure();
				}
			} };
		}

		NODISCARD operator storage_type * () const { return const_cast<storage_type *>(&m_data); }

		NODISCARD auto get_closure() const -> destructor { return m_data.closure; }
		
		NODISCARD auto get_context() const -> void * { return m_data.context; }
		
		NODISCARD auto get_pointer() const -> void * { return m_data.pointer; }

		void set_closure(destructor value) { m_data.closure = value; }
		
		void set_context(void const * value) { m_data.context = (void *)value; }
		
		void set_pointer(void const * value) { m_data.pointer = (void *)value; }

	private:
		storage_type m_data{};
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_API_TYPES_CAPSULE_OBJECT_HPP_
