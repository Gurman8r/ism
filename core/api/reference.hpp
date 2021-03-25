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

	template <class T> class Ref
	{
	private:
		T * m_ref{};

		void ref(Ref const & value)
		{
			if (value.m_ref == m_ref) { return; }
			unref();
			m_ref = value.m_ref;
			if (m_ref) { m_ref->inc_ref(); }
		}

		void ref_pointer(T * value)
		{
			VERIFY(value);
			if (value->init_ref()) { m_ref = value; }
		}

	public:
		NODISCARD bool operator==(T const * value) const { return m_ref == value; }
		NODISCARD bool operator!=(T const * value) const { return m_ref != value; }
		NODISCARD bool operator<(Ref<T> const & value) const { return m_ref < value.m_ref; }
		NODISCARD bool operator==(Ref<T> const & value) const { return m_ref == value.m_ref; }
		NODISCARD bool operator!=(Ref<T> const & value) const { return m_ref != value.m_ref; }

		NODISCARD auto operator->() const -> T const * { return m_ref; }
		NODISCARD auto operator->() -> T * { return m_ref; }

		NODISCARD auto operator*() const -> T const * { return m_ref; }
		NODISCARD auto operator*() -> T * { return m_ref; }

		NODISCARD auto ptr() const -> T const * { return m_ref; }
		NODISCARD auto ptr() -> T * { return m_ref; }

		NODISCARD operator bool() const { return m_ref != nullptr; }

		void operator=(nullptr_t) { unref(); }
		void operator=(Ref const & value) { ref(value); }

		template <class U
		> void operator=(Ref<U> const & value)
		{
			Reference * refb{ const_cast<Reference *>(static_cast<Reference const *>(value.ptr())) };
			if (!refb) { unref(); return; }
			Ref r;
			r.m_ref = ISM::super_cast<T>(refb);
			ref(r);
			r.m_ref = nullptr;
		}

		template <class U
		> void reference_ptr(U * value)
		{
			if (m_ref == value) { return; }
			unref();
			T * r{ ISM::super_cast<T>(value) };
			if (r) { ref_pointer(r); }
		}

		void unref()
		{
			if (m_ref && m_ref->dec_ref()) { ISM::memdelete(m_ref); }
			m_ref = nullptr;
		}

		template <class ... Args
		> Ref<T> & instance(Args && ... args)
		{
			ref(ISM::construct_or_initialize<T>(FWD(args)...));
			return (*this);
		}

	public:
		Ref(nullptr_t) : Ref{} {}

		Ref(Ref const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value)
		{
			Reference * refb{ const_cast<Reference *>(static_cast<Reference const *>(value.ptr())) };
			if (!refb) { unref(); return; }
			Ref r;
			r.m_ref = ISM::super_cast<T>(refb);
			ref(r);
			r.m_ref = nullptr;
		}

		Ref(T * value) { if (value) { ref_pointer(value); } }

		Ref() {}

		~Ref() { unref(); }
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
