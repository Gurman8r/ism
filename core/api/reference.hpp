#ifndef _ISM_REFERENCE_HPP_
#define _ISM_REFERENCE_HPP_

#include <core/api/super.hpp>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Reference : public Super
	{
		RefCount m_refcount, m_refcount_init;

	public:
		NODISCARD int32_t get_reference_count() const { return m_refcount.get(); }

		NODISCARD bool has_references() const { return m_refcount_init.get() != 1; }

		bool init_ref();

		bool inc_ref(); // returns false if refcount is at zero and didn't get increased

		bool dec_ref();

		Reference();

		virtual ~Reference() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class> constexpr bool is_ref_v{ false };

	template <class T> constexpr bool is_ref_v<Ref<T>>{ true };

	template <template <class> class R, class T> constexpr bool is_ref_v<R<T>>
	{
		std::is_base_of_v<Ref<T>, R<T>>
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> class Ref
	{
	public:
		using self_type		= Ref<T>;
		using element_type	= typename T;
		using pointer		= typename element_type *;

	private:
		pointer m_ref{};

		auto ref(self_type const & value) -> self_type &
		{
			if (value.m_ref == m_ref) { return (*this); }
			unref();
			m_ref = value.m_ref;
			if (m_ref) { m_ref->inc_ref(); }
			return (*this);
		}

		auto ref_pointer(pointer value) -> self_type &
		{
			if (CHECK(value)->init_ref()) { m_ref = value; }
			return (*this);
		}

	public:
		virtual ~Ref() { unref(); }

		Ref() {}

		Ref(nullptr_t) {}

		Ref(pointer value) { if (value) { ref_pointer(value); } }

		Ref(self_type const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref(Any const & value)
		{
			Super * object{ ISM::any_cast<Super *>(value) };
			if (!object) { return; }
			
			pointer r{ ISM::super_cast<element_type>(object) };
			if (r && r->inc_ref()) { m_ref = r; }
		}

	public:
		auto operator=(nullptr_t) -> self_type &
		{
			return unref();
		}

		auto operator=(self_type const & value) -> self_type &
		{
			return reset(value);
		}

		template <class U
		> auto operator=(Ref<U> const & value) -> self_type &
		{
			return reset(value);
		}

		auto operator=(Any const & value) -> self_type &
		{
			Super * object{ ISM::any_cast<Super *>(value) };
			if (object == m_ref) { return (*this); }
			
			unref();
			if (!object) { return (*this); }
			
			pointer r{ ISM::super_cast<element_type>(object) };
			if (r && r->int_ref()) { m_ref = r; }
			return (*this);
		}

	public:
		template <class ... Args
		> auto instance(Args && ... args) -> self_type &
		{
			return ref(ISM::construct_or_initialize<element_type>(FWD(args)...));
		}

		auto unref() -> self_type &
		{
			if (m_ref && m_ref->dec_ref()) { ISM::memdelete(m_ref); }
			m_ref = nullptr;
			return (*this);
		}

		template <class U
		> auto reset(U * value) -> self_type &
		{
			if (m_ref == value) { return (*this); }
			unref();
			pointer r{ ISM::super_cast<element_type>(value) };
			return r ? ref_pointer(r) : (*this);
		}

		auto reset(self_type const & value) -> self_type &
		{
			return ref(value);
		}

		template <class U
		> auto reset(Ref<U> const & value) -> self_type &
		{
			Reference * other{ const_cast<Reference *>(static_cast<Reference const *>(value.ptr())) };
			if (!other) { return unref(); }
			
			self_type r;
			r.m_ref = ISM::super_cast<element_type>(other);
			ref(r);
			r.m_ref = nullptr;
			return (*this);
		}

	public:
		NODISCARD operator bool() const { return m_ref != nullptr; }

		NODISCARD auto ptr() const -> element_type const * { return m_ref; }
		NODISCARD auto ptr() -> pointer { return m_ref; }

		NODISCARD auto operator*() const -> element_type const * { return m_ref; }
		NODISCARD auto operator*() -> pointer { return m_ref; }

		NODISCARD auto operator->() const -> element_type const * { return m_ref; }
		NODISCARD auto operator->() -> pointer { return m_ref; }

		NODISCARD bool operator==(element_type const * value) const { return m_ref == value; }
		NODISCARD bool operator!=(element_type const * value) const { return m_ref != value; }
		NODISCARD bool operator<(self_type const & value) const { return m_ref < value.m_ref; }
		NODISCARD bool operator==(self_type const & value) const { return m_ref == value.m_ref; }
		NODISCARD bool operator!=(self_type const & value) const { return m_ref != value.m_ref; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ISM::Hash<Ref<T>>
	{
		hash_t operator()(Ref<T> const & v) const
		{
			return ISM::Hash<void const *>()(v.ptr());
		}
	};

	template <class T> struct ISM::EqualTo<Ref<T>>
	{
		bool operator()(Ref<T> const & a, Ref<T> const & b) const
		{
			return a.ptr() == b.ptr();
		}
	};

	template <class T> struct ISM::Less<Ref<T>>
	{
		bool operator()(Ref<T> const & a, Ref<T> const & b) const
		{
			return a.ptr() < b.ptr();
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WeakRef : public Reference
	{
		SuperID m_ref;

	public:
		NODISCARD Any get_ref() const;

		void set_obj(Super * value);

		void set_ref(REF const & value);

		WeakRef();

		virtual ~WeakRef() override;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_REFERENCE_HPP_
