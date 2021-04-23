#ifndef _ISM_REFERENCE_HPP_
#define _ISM_REFERENCE_HPP_

#include <core/api/super.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Reference : public Super
	{
		RefCount m_refcount, m_refcount_init;

	public:
		NODISCARD int32_t ref_count() const { return m_refcount.get(); }

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
	protected:
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
			if (CHECK(value)->init_ref()) { m_ref = value; }
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
			if (m_ref && m_ref->dec_ref()) { ism::memdelete(m_ref); }
			m_ref = nullptr;
		}

		template <class ... Args
		> void revalue(Args && ... args)
		{
			ref(ism::construct_or_initialize<T>(FWD(args)...));
		}

		template <class U
		> void reset(U * value)
		{
			if (m_ref == value) { return; }
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
			r.m_ref = super_cast<T>(other);
			ref(r);
			r.m_ref = nullptr;
		}

	public:
		NODISCARD operator bool() const { return m_ref != nullptr; }

		NODISCARD auto ptr() const { return const_cast<T *>(m_ref); }

		NODISCARD auto operator*() const { return const_cast<T *>(m_ref); }

		NODISCARD auto operator->() const { return const_cast<T *>(m_ref); }

		NODISCARD bool operator==(T const * value) const { return m_ref == value; }
		
		NODISCARD bool operator!=(T const * value) const { return m_ref != value; }
		
		NODISCARD bool operator<(Ref const & value) const { return m_ref < value.m_ref; }
		
		NODISCARD bool operator==(Ref const & value) const { return m_ref == value.m_ref; }
		
		NODISCARD bool operator!=(Ref const & value) const { return m_ref != value.m_ref; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ism::Hash<Ref<T>>
	{
		NODISCARD hash_t operator()(Ref<T> const & v) const { return ism::Hash<void const *>()(v.ptr()); }
	};

	template <class T> struct ism::EqualTo<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() == b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() == b.ptr(); }
	};

	template <class T> struct ism::NotEqualTo<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() != b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() != b.ptr(); }
	};

	template <class T> struct ism::Less<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() < b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() < b.ptr(); }
	};

	template <class T> struct ism::Greater<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() ? b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() > b.ptr(); }
	};

	template <class T> struct ism::LessEqual<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() <= b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() <= b.ptr(); }
	};

	template <class T> struct ism::GreaterEqual<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() >= b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() >= b.ptr(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API WeakRef : public Reference
	{
		InstanceID m_ref;

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

#endif // !_ISM_REFERENCE_HPP_
