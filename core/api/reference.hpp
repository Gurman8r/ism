#ifndef _ISM_REFERENCE_HPP_
#define _ISM_REFERENCE_HPP_

#include <core/api/super.hpp>

// reference
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Reference : public Super
	{
		RefCount m_refcount, m_refcount_init;

	public:
		virtual ~Reference() override;

		Reference();

		NODISCARD int32_t ref_count() const { return m_refcount.get(); }

		NODISCARD bool has_references() const { return m_refcount_init.get() != 1; }

		bool init_ref();

		bool inc_ref(); // returns false if refcount is at zero and didn't get increased

		bool dec_ref();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ref<T>
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> class Ref
	{
	public:
		using value_type = typename T;

	protected:
		T * m_reference{};

		void ref(Ref const & value)
		{
			if (value.m_reference == m_reference) { return; }
			unref();
			m_reference = value.m_reference;
			if (m_reference) { m_reference->inc_ref(); }
		}

		void ref_pointer(T * value)
		{
			if (CHECK(value)->init_ref()) { m_reference = value; }
		}

	public:
		~Ref() { unref(); }

		Ref() {}

		Ref(nullptr_t) {}

		Ref(T * value) { if (value) { ref_pointer(value); } }

		Ref(Ref const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref & operator=(nullptr_t) { unref(); return (*this); }

		Ref & operator=(Ref const & value) { reset(value); return (*this); }

		template <class U
		> Ref & operator=(Ref<U> const & value) { reset(value); return (*this); }

	public:
		void unref()
		{
			if (m_reference && m_reference->dec_ref()) { memdelete(m_reference); }
			m_reference = nullptr;
		}

		template <class ... Args
		> void revalue(Args && ... args)
		{
			ref(construct_or_initialize<T>(FWD(args)...));
		}

		template <class U
		> void reset(U * value)
		{
			if (m_reference == value) { return; }
			unref();
			T * r{ super_cast<T>(value) };
			if (r) { ref_pointer(r) }
		}

		void reset(Ref const & value)
		{
			ref(value);
		}

		template <class U
		> void reset(Ref<U> const & value)
		{
			Reference * other{ const_cast<Reference *>(static_cast<Reference const *>(value.ptr())) };
			if (!other) { unref(); return; }
			Ref r;
			r.m_reference = super_cast<T>(other);
			ref(r);
			r.m_reference = nullptr;
		}

		T * release()
		{
			T * temp{ m_reference };
			m_reference = nullptr;
			return m_reference;
		}

	public:
		NODISCARD operator bool() const { return m_reference != nullptr; }

		NODISCARD auto ptr() const { return const_cast<T *>(m_reference); }

		NODISCARD auto operator*() const { return const_cast<T *>(m_reference); }

		NODISCARD auto operator->() const { return const_cast<T *>(m_reference); }

		NODISCARD bool operator==(T const * value) const { return m_reference == value; }
		
		NODISCARD bool operator!=(T const * value) const { return m_reference != value; }
		
		NODISCARD bool operator<(Ref const & value) const { return m_reference < value.m_reference; }
		
		NODISCARD bool operator==(Ref const & value) const { return m_reference == value.m_reference; }
		
		NODISCARD bool operator!=(Ref const & value) const { return m_reference != value.m_reference; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// weak ref
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WeakRef : public Reference
	{
		InstanceID m_reference;

	public:
		WeakRef() : Reference{} {}

		WeakRef(Super * value) : WeakRef{} { set_obj(value); }

		WeakRef(REF const & value) : WeakRef{} { set_ref(value); }

		NODISCARD Any get_ref() const;

		void set_obj(Super * value);

		void set_ref(REF const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

template <class T> struct ism::Hash<ism::Ref<T>>
{
	NODISCARD hash_t operator()(Ref<T> const & v) const { return ism::Hash<void const *>()(v.ptr()); }
};

template <class T> struct ism::EqualTo<ism::Ref<T>>
{
	NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() == b; }
	NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() == b.ptr(); }
};

template <class T> struct ism::NotEqualTo<ism::Ref<T>>
{
	NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() != b; }
	NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() != b.ptr(); }
};

template <class T> struct ism::Less<ism::Ref<T>>
{
	NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() < b; }
	NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() < b.ptr(); }
};

template <class T> struct ism::Greater<ism::Ref<T>>
{
	NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() ? b; }
	NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() > b.ptr(); }
};

template <class T> struct ism::LessEqual<ism::Ref<T>>
{
	NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() <= b; }
	NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() <= b.ptr(); }
};

template <class T> struct ism::GreaterEqual<ism::Ref<T>>
{
	NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() >= b; }
	NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() >= b.ptr(); }
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_REFERENCE_HPP_
