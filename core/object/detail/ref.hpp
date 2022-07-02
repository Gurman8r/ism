#ifndef _ISM_REF_HPP_
#define _ISM_REF_HPP_

#include <core/object/detail/object_api.hpp>

// ref
namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ref<Object> 
	template <class T
	> class NOVTABLE Ref : public _Ref_Tag, public ObjectAPI<Ref<T>>
	{
	protected:
		T * m_ptr{};

		void ref(Ref const & value)
		{
			if (value.m_ptr == m_ptr) { return; }
			unref();
			m_ptr = value.m_ptr;
			if (m_ptr) { m_ptr->reference(); }
		}

		void ref_pointer(T * value)
		{
			ASSERT("INVALID REF POINTER" && value);

			if (value->init_ref()) { m_ptr = value; }
		}

	public:
		using value_type = typename T;

		~Ref() noexcept { unref(); }

		Ref() noexcept = default;

		Ref(nullptr_t) noexcept {}

		Ref(T * value) { if (value) { ref_pointer(value); } }

		Ref(Ref const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref(T && value) noexcept { instance(std::move(value)); }

		Ref & operator=(nullptr_t) { return unref(), (*this); }

		Ref & operator=(Ref const & value) { return reset(value), (*this); }

		template <class U
		> Ref & operator=(Ref<U> const & value) { return reset(value), (*this); }

		Ref & operator=(T && value) noexcept { return instance(std::move(value)), (*this); }

	public:
		template <class ... Args
		> static auto new_(Args && ... args) { return Ref<T>{ T{ FWD(args)... } }; }

		template <class U> NODISCARD U cast() const &; // cast.hpp

		template <class U> NODISCARD U cast() &&; // cast.hpp

		NODISCARD static constexpr auto get_class_static() noexcept { return T::get_class_static(); }

		NODISCARD static auto get_type_static() noexcept { return T::get_type_static(); }

	public:
		template <class U = T, class ... Args
		> void instance(Args && ... args)
		{
			static_assert(std::is_base_of_v<T, U>);

			ref(memnew(U{ FWD(args)... }));
		}

		void unref()
		{
			if (m_ptr && m_ptr->unreference()) {
				ism::call_default_delete(m_ptr);
			}
			m_ptr = nullptr;
		}

		void reset(Ref const & value)
		{
			ref(value);
		}

		template <class U> void reset(U * value)
		{
			if (m_ptr == value) { return; }
			unref();
			T * r{ dynamic_cast<T *>(value) };
			if (r) { ref_pointer(r); }
		}

		template <class U> void reset(Ref<U> const & value)
		{
			Object * other{ static_cast<Object *>(value.ptr()) };
			if (!other) { unref(); return; }
			Ref r;
			r.m_ptr = dynamic_cast<T *>(other);
			ref(r);
			r.m_ptr = nullptr;
		}

	public:
		NODISCARD operator bool() const noexcept { return m_ptr != nullptr; }
		NODISCARD bool is_null() const noexcept { return m_ptr == nullptr; }
		NODISCARD bool is_valid() const noexcept { return m_ptr != nullptr; }
		NODISCARD auto ptr() const noexcept { return const_cast<T *>(m_ptr); }

		NODISCARD auto operator*() noexcept { return m_ptr; }
		NODISCARD auto operator*() const noexcept { return m_ptr; }

		NODISCARD auto operator->() noexcept { return m_ptr; }
		NODISCARD auto operator->() const noexcept { return m_ptr; }

		template <class U> friend bool operator==(Ref const & lhs, U const * rhs) noexcept { return (void *)lhs.m_ptr == (void *)rhs; }
		template <class U> friend bool operator!=(Ref const & lhs, U const * rhs) noexcept { return (void *)lhs.m_ptr != (void *)rhs; }

		template <class U> friend bool operator==(U const * lhs, Ref const & rhs) noexcept { return (void *)lhs == (void *)rhs.m_ptr; }
		template <class U> friend bool operator!=(U const * lhs, Ref const & rhs) noexcept { return (void *)lhs != (void *)rhs.m_ptr; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	template <class T> struct Hasher<Ref<T>> : Hasher<ObjectAPI<Ref<T>>> {};
	template <class T> struct EqualTo<Ref<T>> : EqualTo<ObjectAPI<Ref<T>>> {};
	template <class T> struct NotEqualTo<Ref<T>> : NotEqualTo<ObjectAPI<Ref<T>>> {};
	template <class T> struct Less<Ref<T>> : Less<ObjectAPI<Ref<T>>> {};
	template <class T> struct Greater<Ref<T>> : Greater<ObjectAPI<Ref<T>>> {};
	template <class T> struct LessEqual<Ref<T>> : LessEqual<ObjectAPI<Ref<T>>> {};
	template <class T> struct GreaterEqual<Ref<T>> : GreaterEqual<ObjectAPI<Ref<T>>> {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ref common
#define REF_COMMON(m_class, m_check)																	\
public:																									\
	ALIAS(base_type) ism::Ref<value_type>;																\
																										\
	NODISCARD static bool check_(ism::Ref<Object> const & o) { return o && (bool)(m_check(o)); }		\
																										\
	~m_class() noexcept = default;																		\
																										\
	m_class() noexcept = default;																		\
																										\
	m_class(nullptr_t) noexcept {}																		\
																										\
	m_class(value_type * value) { if (value) { ref_pointer(value); } }									\
																										\
	m_class(base_type const & value) { ref(value); }													\
																										\
	template <class U> m_class(ism::Ref<U> const & value) { reset(value); }								\
																										\
	m_class(value_type && value) noexcept { instance(std::move(value)); }								\
																										\
	m_class & operator=(nullptr_t) { return unref(), (*this); }											\
																										\
	m_class & operator=(base_type const & value) { return reset(value), (*this); }						\
																										\
	template <class U> m_class & operator=(ism::Ref<U> const & value) { return reset(value), (*this); }	\
																										\
	m_class & operator=(value_type && value) noexcept { return instance(std::move(value)), (*this); }	\
																										\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// ref traits
#define IMPLEMENT_DEFAULT_REF_TRAITS(m_class)																\
	template <> struct ism::Hasher<m_class> : ism::Hasher<ism::Ref<m_class::value_type>> {};				\
																											\
	template <> struct ism::EqualTo<m_class> : ism::EqualTo<ism::Ref<m_class::value_type>> {};				\
																											\
	template <> struct ism::NotEqualTo<m_class> : ism::NotEqualTo<ism::Ref<m_class::value_type>> {};		\
																											\
	template <> struct ism::Less<m_class> : ism::Less<ism::Ref<m_class::value_type>> {};					\
																											\
	template <> struct ism::Greater<m_class> : ism::Greater<ism::Ref<m_class::value_type>> {};				\
																											\
	template <> struct ism::LessEqual<m_class> : ism::LessEqual<ism::Ref<m_class::value_type>> {};			\
																											\
	template <> struct ism::GreaterEqual<m_class> : ism::GreaterEqual<ism::Ref<m_class::value_type>> {};	\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_REF_HPP_
