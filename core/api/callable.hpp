#ifndef _ISM_CALLABLE_HPP_
#define _ISM_CALLABLE_HPP_

#include <core/api/runtime.hpp>

namespace ISM
{
	template <class T
	> struct PtrToArg {};

	template <int32_t N> struct ISM::PtrToArg<std::_Ph<N>>
	{
		static std::_Ph<N> convert(void const * p)
		{
			return *reinterpret_cast<std::_Ph<N> const *>(p);
		}
		static void encode(std::_Ph<N> v, void * p)
		{
			*((std::_Ph<N> *)p) = v;
		}
	};
}

#define MAKE_PTRARG(T)							\
template <> struct ISM::PtrToArg<T>				\
{												\
	static bool convert(void const * p)			\
	{											\
		return *reinterpret_cast<T const *>(p);	\
	}											\
	static void encode(T v, void * p)			\
	{											\
		*((T *)p) = v;							\
	}											\
}

#define MAKE_PTRARG_CONVT(T, C)									\
template <> struct ISM::PtrToArg<T>								\
{																\
	static T convert(void const * p)							\
	{															\
		return static_cast<T>(*reinterpret_cast<C const *>(p));	\
	}															\
	static void encode(T v, void * p)							\
	{															\
		*((C *)p) = static_cast<C>(v);							\
	}															\
};																\
template <> struct ISM::PtrToArg<T const &>						\
{																\
	static T convert(void const * p)							\
	{															\
		return static_cast<T>(*reinterpret_cast<C const *>(p));	\
	}															\
																\
	static void encode(T v, void * p)							\
	{															\
		*((C *)p) = static_cast<C>(v);							\
	}															\
}

#define MAKE_PTRARG_REFERENCE(T)					\
template <> struct ISM::PtrToArg<T>					\
{													\
	static T convert(void const * p)				\
	{												\
		return *reinterpret_cast<T const *>(p);		\
	}												\
	static void encode(T const & v, void * p)		\
	{												\
		*((T *)p) = v;								\
	}												\
};													\
template <> struct ISM::PtrToArg<T const &>			\
{													\
	static T convert(void const * p)				\
	{												\
		return *reinterpret_cast<T const *>(p);		\
	}												\
	static void encode(T const & v, void * p)		\
	{												\
		*((T *)p) = v;								\
	}												\
}

namespace ISM
{
	MAKE_PTRARG(bool);
	MAKE_PTRARG(char);

	MAKE_PTRARG_CONVT(int8_t, int64_t);
	MAKE_PTRARG_CONVT(int16_t, int64_t);
	MAKE_PTRARG_CONVT(int32_t, int64_t);
	MAKE_PTRARG(int64_t);

	MAKE_PTRARG_CONVT(uint8_t, uint64_t);
	MAKE_PTRARG_CONVT(uint16_t, uint64_t);
	MAKE_PTRARG_CONVT(uint32_t, uint64_t);
	MAKE_PTRARG(uint64_t);
	
	MAKE_PTRARG_CONVT(float32_t, float64_t);
	MAKE_PTRARG_CONVT(float80_t, float64_t);
	MAKE_PTRARG(float64_t);
	
	MAKE_PTRARG(cstring);
	MAKE_PTRARG_REFERENCE(String);
}

#endif // !_ISM_CALLABLE_HPP_
