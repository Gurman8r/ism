#ifndef _ISM_REFERENCE_HPP_
#define _ISM_REFERENCE_HPP_

#include <core/api/super.hpp>

// reference
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Reference : public Super
	{
		ISM_SUPER(Reference, Super);

	private:
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

	template <class T, class =  std::enable_if_t<std::is_base_of_v<Reference, T>>
	> void unref(T * value)
	{
		if (value && value->dec_ref())
		{
			memdelete(value);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

// ref<T>
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	struct _Ref_Tag {};

	template <class T> constexpr bool is_ref_v{ std::is_base_of_v<_Ref_Tag, T> };

	template <class T> struct Ref : _Ref_Tag
	{
	protected:
		T * m_ref{};

		NODISCARD bool dec_ref() noexcept // return true if the object should be destroyed
		{
			return m_ref && m_ref->dec_ref();
		}

		void ref(Ref const & value)
		{
			if (value.m_ref == m_ref) { return; }
			unref();
			m_ref = value.m_ref;
			if (m_ref) { m_ref->inc_ref(); }
		}

		void ref_pointer(T * value)
		{
			if (CHECK(value)->init_ref())
			{
				m_ref = value;
			}
		}

	public:
		using value_type = typename T;

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
			if (dec_ref()) { DefaultDelete<T>{}(m_ref); }
			m_ref = nullptr;
		}

		template <class ... Args
		> void revalue(Args && ... args)
		{
			ref(construct_or_initialize<T>(FWD(args)...));
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

		T * release()
		{
			T * temp{ m_ref };
			m_ref = nullptr;
			return temp;
		}

	public:
		NODISCARD operator bool() const noexcept { return m_ref != nullptr; }

		NODISCARD auto ptr() const noexcept { return const_cast<T *>(m_ref); }

		NODISCARD auto operator*() const noexcept { return const_cast<T *>(m_ref); }

		NODISCARD auto operator->() const noexcept { return const_cast<T *>(m_ref); }

		NODISCARD bool operator==(T const * value) const noexcept { return m_ref == value; }
		
		NODISCARD bool operator!=(T const * value) const noexcept { return m_ref != value; }
		
		NODISCARD bool operator<(Ref const & value) const noexcept { return m_ref < value.m_ref; }
		
		NODISCARD bool operator==(Ref const & value) const noexcept { return m_ref == value.m_ref; }
		
		NODISCARD bool operator!=(Ref const & value) const noexcept { return m_ref != value.m_ref; }
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
