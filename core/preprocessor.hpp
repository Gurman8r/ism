#ifndef _ISM_PREPROCESSOR_HPP_
#define _ISM_PREPROCESSOR_HPP_

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// concat implementation
#define _IMPL_CAT(a, b) \
		a##b

// concat
#define CAT(a, b) \
		_IMPL_CAT(a, b)

// compare
#define CMP(a, b) \
		(((a) != (b)) ? (((a) < (b)) ? -1 : 1) : 0)

// minimum
#define MIN(a, b) \
		((a) < (b) ? (a) : (b))

// maximum
#define MAX(a, b) \
		((a) < (b) ? (b) : (a))

// token literal to string
#define TOSTR(expr) \
		#expr

// macro contents to string
#define XSTR(expr) \
		TOSTR(expr)

// wide string
#define WIDE(str) \
		CAT(L, str)

// size of static array
#define ARRAY_SIZE(arr) \
		(sizeof(arr) / sizeof(*arr))

// unused expression
#define UNUSED(expr) \
		((void)(expr))

// branchless if statement
#define BRANCHLESS_IF(m_condition, m_expr) \
		(UNUSED((!(m_condition)) || ((m_expr), 0)))

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// map enum to array of constant values
#define ENUM_MAPPING(m_func, m_from, m_to, ...)					\
		static constexpr m_to _MAP_##m_from##_TO_##m_to##_[] =	\
		{														\
			##__VA_ARGS__										\
		};														\
		static constexpr m_to m_func(m_from i) noexcept			\
		{														\
			return _MAP_##m_from##_TO_##m_to##_[(size_t)i];		\
		}														\

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// anonymous variable name
#if defined(__COUNTER__)
#	define ANON(m_expr) CAT(_, CAT(m_expr, CAT(_, CAT(__COUNTER__, _))))
#elif defined(__LINE__)
#	define ANON(m_expr) CAT(_, CAT(m_expr, CAT(_, CAT(__LINE__, _))))
#else
#	define ANON(m_expr) CAT(_, CAT(m_expr, _))
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ON SCOPE ENTER */

namespace ism::priv
{
	template <class F> struct OnScopeEnter final
	{
		explicit OnScopeEnter(F fn) noexcept { fn(); }
	};

	enum class OnScopeEnter_Tag {};

	template <class F
	> auto operator+(OnScopeEnter_Tag, F fn) noexcept { return OnScopeEnter<F>{ fn }; }
}

#define ON_SCOPE_ENTER(...) \
		auto ANON(temp) = (ism::priv::OnScopeEnter_Tag{}) + [##__VA_ARGS__]() noexcept -> void

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* ON SCOPE EXIT */

namespace ism::priv
{
	template <class F> struct OnScopeExit final
	{
		F const m_fn;

		explicit OnScopeExit(F fn) noexcept : m_fn{ fn } {}

		~OnScopeExit() noexcept { m_fn(); }
	};

	enum class OnScopeExit_Tag {};

	template <class F
	> auto operator+(OnScopeExit_Tag, F fn) noexcept { return OnScopeExit<F>{ fn }; }
}

#define ON_SCOPE_EXIT(...) \
		auto ANON(temp) = (ism::priv::OnScopeExit_Tag{}) + [##__VA_ARGS__]() noexcept -> void

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// non-copyable
#define NON_COPYABLE(T) \
		T(T const &) = delete; \
		T & operator=(T const &) = delete;

// non-movable
#define NON_MOVABLE(T) \
		T(T &&) noexcept = delete; \
		T & operator=(T &&) noexcept = delete;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// wrap forward iterators
#define WRAP_FORWARD_ITERATORS(m_container) \
		NODISCARD auto begin() noexcept -> decltype(m_container)::iterator { return m_container.begin(); } \
		NODISCARD auto begin() const noexcept -> decltype(m_container)::const_iterator { return m_container.begin(); } \
		NODISCARD auto cbegin() const noexcept -> decltype(m_container)::const_iterator { return m_container.cbegin(); } \
		NODISCARD auto end() noexcept -> decltype(m_container)::iterator { return m_container.end(); } \
		NODISCARD auto end() const noexcept -> decltype(m_container)::const_iterator { return m_container.end(); } \
		NODISCARD auto cend() const noexcept -> decltype(m_container)::const_iterator { return m_container.cend(); }

// wrap reverse iterators
#define WRAP_REVERSE_ITERATORS(m_container) \
		NODISCARD auto rbegin() noexcept -> decltype(m_container)::reverse_iterator { return m_container.rbegin(); } \
		NODISCARD auto rbegin() const noexcept -> decltype(m_container)::const_reverse_iterator { return m_container.rbegin(); } \
		NODISCARD auto crbegin() const noexcept -> decltype(m_container)::const_reverse_iterator { return m_container.crbegin(); } \
		NODISCARD auto rend() noexcept -> decltype(m_container)::reverse_iterator { return m_container.rend(); } \
		NODISCARD auto rend() const noexcept -> decltype(m_container)::const_reverse_iterator { return m_container.rend(); } \
		NODISCARD auto crend() const noexcept -> decltype(m_container)::const_reverse_iterator { return m_container.crend(); }

// wrap all iterators
#define WRAP_ALL_ITERATORS(m_container) \
		WRAP_FORWARD_ITERATORS(m_container) \
		WRAP_REVERSE_ITERATORS(m_container)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#endif // !_ISM_PREPROCESSOR_HPP_
