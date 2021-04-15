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
		template <class ... Args
		> void revalue(Args && ... args)
		{
			ref(ism::construct_or_initialize<T>(FWD(args)...));
		}

		void unref()
		{
			if (m_ref && m_ref->dec_ref()) { ism::memdelete(m_ref); }
			m_ref = nullptr;
		}

		template <class U
		> void reset(U * value)
		{
			if (m_ref == value) { return; }
			unref();
			T * r{ ism::super_cast<T>(value) };
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
			r.m_ref = ism::super_cast<T>(other);
			ref(r);
			r.m_ref = nullptr;
		}

	public:
		NODISCARD operator bool() const { return m_ref != nullptr; }

		NODISCARD auto ptr() const -> T const * { return m_ref; }
		NODISCARD auto ptr() -> T * { return m_ref; }

		NODISCARD auto operator*() const -> T const * { return m_ref; }
		NODISCARD auto operator*() -> T * { return m_ref; }

		NODISCARD auto operator->() const -> T const * { return m_ref; }
		NODISCARD auto operator->() -> T * { return m_ref; }

		NODISCARD bool operator==(T const * value) const { return m_ref == value; }
		NODISCARD bool operator!=(T const * value) const { return m_ref != value; }
		NODISCARD bool operator<(Ref const & value) const { return m_ref < value.m_ref; }
		NODISCARD bool operator==(Ref const & value) const { return m_ref == value.m_ref; }
		NODISCARD bool operator!=(Ref const & value) const { return m_ref != value.m_ref; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct ism::Hash<Ref<T>>
	{
		hash_t operator()(Ref<T> const & v) const
		{
			return ism::Hash<void const *>()(v.ptr());
		}
	};

	template <class T> struct ism::EqualTo<Ref<T>>
	{
		bool operator()(Ref<T> const & a, Ref<T> const & b) const
		{
			return a.ptr() == b.ptr();
		}
	};

	template <class T> struct ism::Less<Ref<T>>
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
