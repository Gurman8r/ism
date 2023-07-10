#ifndef _ISM_REF_HPP_
#define _ISM_REF_HPP_

#include <core/object/object_api.hpp>

// ref
namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// Ref<Object> 
	template <class T
	> class NOVTABLE Ref : public _Ref_Tag, public ObjectAPI<Ref<T>>
	{
	public:
		using self_type		= typename Ref<T>;
		using value_type	= typename T;
		using pointer		= typename value_type *;
		using const_pointer	= typename value_type const *;

		~Ref() noexcept { unref(); }

		Ref() noexcept = default;

		Ref(nullptr_t) noexcept {}

		Ref(pointer value) { if (value) { ref_pointer(value); } }

		Ref(self_type const & value) { ref(value); }

		template <class U
		> Ref(Ref<U> const & value) { reset(value); }

		Ref(value_type && value) noexcept { instance(std::move(value)); }

		self_type & operator=(nullptr_t) { return unref(), (*this); }

		self_type & operator=(self_type const & value) { return reset(value), (*this); }

		template <class U
		> self_type & operator=(Ref<U> const & value) { return reset(value), (*this); }

		self_type & operator=(value_type && value) noexcept { return instance(std::move(value)), (*this); }

	public:
		template <class ... Args
		> NODISCARD static auto new_(Args && ... args) { return self_type{}.instance(FWD(args)...); }

		NODISCARD static constexpr auto get_class_name_static() noexcept { return value_type::get_class_name_static(); }

		NODISCARD static auto get_type_static() noexcept { return value_type::get_type_static(); }

		template <class U> NODISCARD U cast() const &;

		template <class U> NODISCARD U cast() &&;

	public:
		template <class U = value_type, class ... Args, std::enable_if_t<std::is_base_of_v<value_type, U>, int> = 0
		> self_type & instance(Args && ... args)
		{
			if constexpr (std::is_constructible_v<U, Args...>) {
				ref(memnew(U(FWD(args)...)));
			}
			else {
				ref(memnew(U{ FWD(args)... }));
			}
			return (*this);
		}

		void unref()
		{
			if (m_ptr && m_ptr->dec_ref()) {
				Ism::call_default_delete(m_ptr);
			}
			m_ptr = nullptr;
		}

		void reset(self_type const & value)
		{
			ref(value);
		}

		template <class U
		> void reset(U * value)
		{
			if (m_ptr == value) {
				return;
			}
			unref();
			if (pointer r{ dynamic_cast<pointer>(value) }) {
				ref_pointer(r);
			}
		}

		template <class U
		> void reset(Ref<U> const & value)
		{
			Object * other{ static_cast<Object *>(value.ptr()) };
			if (!other) {
				unref();
				return;
			}
			self_type r;
			r.m_ptr = dynamic_cast<pointer>(other);
			ref(r);
			r.m_ptr = nullptr;
		}

		NODISCARD pointer release() noexcept
		{
			return std::exchange(m_ptr, nullptr);
		}

	public:
		NODISCARD operator bool() const noexcept { return m_ptr != nullptr; }
		
		NODISCARD bool is_null() const noexcept { return m_ptr == nullptr; }
		
		NODISCARD bool is_valid() const noexcept { return m_ptr != nullptr; }
		
		NODISCARD auto ptr() const noexcept { return (pointer)m_ptr; }

		NODISCARD auto operator*() noexcept { return m_ptr; }
		
		NODISCARD auto operator*() const noexcept { return m_ptr; }

		NODISCARD auto operator->() noexcept { return m_ptr; }
		
		NODISCARD auto operator->() const noexcept { return m_ptr; }

		template <class U> NODISCARD friend bool operator==(self_type const & a, U const * b) noexcept { return (void *)a.m_ptr == (void *)b; }
		
		template <class U> NODISCARD friend bool operator!=(self_type const & a, U const * b) noexcept { return (void *)a.m_ptr != (void *)b; }

		template <class U> NODISCARD friend bool operator==(U const * a, self_type const & b) noexcept { return (void *)a == (void *)b.m_ptr; }
		
		template <class U> NODISCARD friend bool operator!=(U const * a, self_type const & b) noexcept { return (void *)a != (void *)b.m_ptr; }

	protected:
		pointer m_ptr{};

		void ref(self_type const & value)
		{
			if (value.m_ptr == m_ptr) {
				return;
			}
			unref();
			m_ptr = value.m_ptr;
			if (m_ptr) {
				m_ptr->inc_ref();
			}
		}

		void ref_pointer(pointer value)
		{
			ASSERT("invalid ref pointer" && value);

			if (value->init_ref()) {
				m_ptr = value;
			}
		}
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

	// ref class
#define CUSTOM_REF(m_class, m_check)																	\
public:																									\
	using base_type = Ism::Ref<value_type>;																\
																										\
	static bool check_(Ism::Ref<Object> const & o) { return o && (bool)(m_check(o)); }					\
																										\
	~m_class() noexcept = default;																		\
																										\
	m_class() noexcept = default;																		\
																										\
	m_class(nullptr_t) noexcept {}																		\
																										\
	m_class(pointer value) { if (value) { ref_pointer(value); } }										\
																										\
	m_class(base_type const & value) { ref(value); }													\
																										\
	template <class U> m_class(Ism::Ref<U> const & value) { reset(value); }								\
																										\
	m_class(value_type && value) noexcept { instance(std::move(value)); }								\
																										\
	m_class & operator=(nullptr_t) { return unref(), (*this); }											\
																										\
	m_class & operator=(base_type const & value) { return reset(value), (*this); }						\
																										\
	template <class U> m_class & operator=(Ism::Ref<U> const & value) { return reset(value), (*this); }	\
																										\
	m_class & operator=(value_type && value) noexcept { return instance(std::move(value)), (*this); }	\
																										\

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_REF_HPP_
