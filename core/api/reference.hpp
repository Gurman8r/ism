#ifndef _ISM_REFERENCE_HPP_
#define _ISM_REFERENCE_HPP_

#include <core/api/super.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Reference : public Super
	{
		ISM_SUPER(Reference, Super);

		RefCount m_refcount, m_refcount_init;

	protected:
		Reference() noexcept : Super{ true } { m_refcount.init(); m_refcount_init.init(); }

		virtual void on_inc_ref() {}

		virtual void on_dec_ref() {}

	public:
		virtual ~Reference() noexcept override = default;

		NODISCARD int32_t get_ref_count() const { return m_refcount.get(); }

		NODISCARD bool has_references() const { return m_refcount_init.get() != 1; }

		bool init_ref() noexcept
		{
			if (inc_ref())
			{
				if (!has_references() && m_refcount_init.unref())
				{
					dec_ref(); // first referencing is already 1, so compensate for the ref above
				}
				return true;
			}
			return false;
		}

		bool inc_ref() noexcept // returns false if refcount is at zero and didn't get increased
		{
			uint32_t rc_val{ m_refcount.refval() };
			bool success{ rc_val != 0 };
			if (success && rc_val <= 2 /* higher is not relevant */) { on_inc_ref(); }
			return success;
		}

		bool dec_ref() noexcept
		{
			uint32_t rc_val{ m_refcount.unrefval() };
			bool die{ rc_val == 0 };
			if (rc_val <= 1 /* higher is not relevant */) { on_dec_ref(); }
			return die;
		}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct NOVTABLE _Ref_Tag {};

	template <class T> constexpr bool is_ref_v{ std::is_base_of_v<_Ref_Tag, T> };

	template <class T> class NOVTABLE Ref : public _Ref_Tag
	{
	protected:
		T * m_ptr{};

		void ref(Ref const & value)
		{
			if (value.m_ptr == m_ptr) { return; }
			unref();
			m_ptr = value.m_ptr;
			if (m_ptr) { m_ptr->inc_ref(); }
		}

		void ref_pointer(T * value)
		{
			if (CHECK(value)->init_ref())
			{
				m_ptr = value;
			}
		}

	public:
		using value_type = T;

		~Ref() noexcept { unref(); }

		Ref() noexcept {}

		Ref(nullptr_t) {}

		Ref(T * value) { if (value) { ref_pointer(value); } }

		Ref(Reference * value) { if (value) { reset(value); } }

		Ref(Ref const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref & operator=(nullptr_t) { unref(); return (*this); }

		Ref & operator=(Ref const & value) { reset(value); return (*this); }

		template <class U
		> Ref & operator=(Ref<U> const & value) { reset(value); return (*this); }

	public:
		template <class ... Args
		> void instance(Args && ... args)
		{
			ref(construct_or_initialize<T>(FWD(args)...));
		}

		void unref()
		{
			if (m_ptr && m_ptr->dec_ref()) { DefaultDelete<T>{}(m_ptr); }
			m_ptr = nullptr;
		}

		void reset(Ref const & value)
		{
			ref(value);
		}

		template <class U
		> void reset(U * value)
		{
			if (m_ptr == value) { return; }
			unref();
			T * r{ super_cast<T>(value) };
			if (r) { ref_pointer(r); }
		}

		template <class U
		> void reset(Ref<U> const & value)
		{
			Reference * other{ const_cast<Reference *>(static_cast<Reference const *>(value.ptr())) };
			if (!other) { unref(); return; }
			Ref r;
			r.m_ptr = super_cast<T>(other);
			ref(r);
			r.m_ptr = nullptr;
		}

		T * release()
		{
			T * temp{ m_ptr };
			m_ptr = nullptr;
			return temp;
		}

	public:
		NODISCARD operator bool() const noexcept { return m_ptr != nullptr; }

		NODISCARD auto ptr() const noexcept { return const_cast<T *>(m_ptr); }

		NODISCARD auto operator*() const noexcept { return const_cast<T *>(m_ptr); }

		NODISCARD auto operator->() const noexcept { return const_cast<T *>(m_ptr); }

		NODISCARD bool operator==(T const * value) const noexcept { return m_ptr == value; }
		
		NODISCARD bool operator!=(T const * value) const noexcept { return m_ptr != value; }
		
		NODISCARD bool operator<(Ref const & value) const noexcept { return m_ptr < value.m_ptr; }
		
		NODISCARD bool operator==(Ref const & value) const noexcept { return m_ptr == value.m_ptr; }
		
		NODISCARD bool operator!=(Ref const & value) const noexcept { return m_ptr != value.m_ptr; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hash<Ref<T>>
	{
		NODISCARD hash_t operator()(Ref<T> const & v) const { return Hash<void const *>()(v.ptr()); }
	};

	template <class T> struct EqualTo<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() == b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() == b.ptr(); }
	};

	template <class T> struct NotEqualTo<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() != b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() != b.ptr(); }
	};

	template <class T> struct Less<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() < b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() < b.ptr(); }
	};

	template <class T> struct Greater<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() ? b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() > b.ptr(); }
	};

	template <class T> struct LessEqual<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() <= b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() <= b.ptr(); }
	};

	template <class T> struct GreaterEqual<Ref<T>>
	{
		NODISCARD bool operator()(Ref<T> const & a, T const * b) const { return a.ptr() >= b; }
		NODISCARD bool operator()(Ref<T> const & a, Ref<T> const & b) const { return a.ptr() >= b.ptr(); }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_REFERENCE_HPP_
