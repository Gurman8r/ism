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
		ISM_OBJECT(CoreCapsule, CoreObject);

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
			m_capsule = { nullptr, nullptr, nullptr };
		}

		CoreCapsule(void const * value, destructor closure = nullptr) : self_type{}
		{
			m_capsule = { (void *)value, nullptr, closure };
		}

		CoreCapsule(void const * value, void(*closure)(void *)) : self_type{}
		{
			m_capsule = { (void *)value, nullptr, (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)(void *)>(self->m_capsule.context) };

					closure(self->m_capsule.pointer);
				}
			} };
		}

		CoreCapsule(void(*closure)()) : self_type{}
		{
			m_capsule = { (void *)closure, nullptr, (destructor)[](CoreObject * o)
			{
				if (auto self{ super_cast<self_type>(o) })
				{
					auto closure{ reinterpret_cast<void(*)()>(self->m_capsule.pointer) };

					closure();
				}
			} };
		}

		NODISCARD auto & operator*() const { return const_cast<storage_type &>(m_capsule); }

		NODISCARD auto * operator->() const { return const_cast<storage_type *>(&m_capsule); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// CAPSULE
	template <> class Handle<CoreCapsule> : public BaseHandle<CoreCapsule>
	{
		ISM_HANDLE(CoreCapsule);

	public:
		Handle() = default;

		~Handle() = default;

		using storage_type = CoreCapsule::storage_type;

		NODISCARD auto get_closure() const -> destructor { return (*m_ref)->closure; }

		NODISCARD auto get_context() const -> void * { return (*m_ref)->context; }

		NODISCARD auto get_pointer() const -> void * { return (*m_ref)->pointer; }

		NODISCARD auto get_doc() const -> String const & { return (*m_ref)->doc; }

		NODISCARD auto get_name() const -> String const & { return (*m_ref)->name; }

		void set_closure(destructor value) { (*m_ref)->closure = value; }

		void set_context(void const * value) { (*m_ref)->context = (void *)value; }

		void set_pointer(void const * value) { (*m_ref)->pointer = (void *)value; }

		void set_doc(String const & value) { (*m_ref)->doc = value; }

		void set_name(String const & value) { (*m_ref)->name = value; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_CAPSULE_OBJECT_HPP_
