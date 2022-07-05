#ifndef _ISM_LANGUAGE_FEATURES_HPP_
#define _ISM_LANGUAGE_FEATURES_HPP_

#include <cstddef>

#include <core/preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	CONFIGURATION
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (_DEBUG && _DEBUG)
//	Debug
#	define ISM_DEBUG true
#else
//	Release
#	define ISM_DEBUG false
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	LANGUAGE
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if __cplusplus
#	if _MSVC_LANG
#		define ISM_CXX _MSVC_LANG
#	else
#		define ISM_CXX __cplusplus
#	endif
#else
#	error "this system does not support C++"
#endif

#if (ISM_CXX >= 201907L)
//	C++20
#	define ISM_CXX_20 1
#	define ISM_CXX_17 1
#	define ISM_CXX_14 1
#	define ISM_CXX_11 1

#elif (ISM_CXX >= 201703L)
//	C++17
#	define ISM_CXX_20 0
#	define ISM_CXX_17 1
#	define ISM_CXX_14 1
#	define ISM_CXX_11 1

#elif (ISM_CXX >= 201402L)
//	C++14
#	define ISM_CXX_20 0
#	define ISM_CXX_17 0
#	define ISM_CXX_14 1
#	define ISM_CXX_11 1

#elif (ISM_CXX >= 201103L)
//	C++11
#	define ISM_CXX_20 0
#	define ISM_CXX_17 0
#	define ISM_CXX_14 0
#	define ISM_CXX_11 1

#else
#	error "this version of C++ is not supported"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	OPERATING SYSTEM
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _WIN32 || _WIN64 || WIN32 || WIN64 || __MINGW32__ || __MINGW64__
//	Windows
#define ISM_OS_WINDOWS true

#elif __APPLE__ && __MACH__
//	Apple
#define ISM_OS_APPLE true

#elif __unix__
//	Unix
#	define ISM_OS_UNIX true

#	if __ANDROID__
//	Android
#	define ISM_OS_ANDROID true

#	elif __linux__
//	Linux
#	define ISM_OS_LINUX true

#	elif __FreeBSD__ || __FreeBSD_kernel__
//	FreeBSD
#	define ISM_OS_FREEBSD true

#	else
#	error "unsupported unix operating system"
#	endif

#else
#error "unsupported operating system"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	PROCESSOR
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if __x86_64__ || _M_X64 || _x64
//	x64
#define ISM_CPU_X64 true
#define ISM_ARCH 64

#elif __i386__ || _M_IX86
//	x86
#define ISM_CPU_X86 true
#define ISM_ARCH 32

#elif __arm__ || _M_ARM || __aarch64__
//	ARM
#	if __aarch64__
#		define ISM_CPU_ARM64 true
#		define ISM_ARCH 64
#	else
#		define ISM_CPU_ARM32 true
#		define ISM_ARCH 32
#	endif

#elif ppc || _M_PPC || __ppc64__
//	PowerPC
#	if __ppc64__
#		define ISM_CPU_PPC64 true
#		define ISM_ARCH 64
#	else
#		define ISM_CPU_PPC32 true
#		define ISM_ARCH 32
#	endif

#else
#error "unsupported cpu"
#endif

#if !(ISM_ARCH == 32 || ISM_ARCH == 64)
#error "unsupported architecture"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	COMPILER
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _MSC_VER
//	Visual Studio
#	define ISM_CC_MSVC _MSC_VER

#elif __clang__
//	Clang / LLVM
#	define ISM_CC_CLANG __clang__

#elif (__GNUC__ || __GNUG__) && !__clang__
//	GCC
#	ifdef __GNUC__
#		define ISM_CC_GCC __GNUC__
#	else
#		define ISM_CC_GCC __GNUG__
#	endif

#elif __ICC || __INTEL_ISM_CC
//	Intel
#	ifdef __ICC
#		define ISM_CC_INTEL __ICC
#	else
#		define ISM_CC_INTEL __INTEL_ISM_CC
#	endif

#elif __MINGW32__ || __MINGW64__
//	MinGW
#	ifdef __MINGW64__
#		define ISM_CC_MINGW __MINGW64__
#	else
#		define ISM_CC_MINGW __MINGW32__
#	endif

#elif __EMSCRIPTEN__
//	Emscripten
#define ISM_CC_EMSCRIPTEN __EMSCRIPTEN__

#elif __asmjs__
//	asm.js
#define ISM_CC_ASMJS __asmjs__

#elif __wasm__
//	WebAssembly
#define ISM_CC_WASM __wasm__

#else
#error "unsupported compiler"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	ATTRIBUTES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

//	nodiscard
#if __has_cpp_attribute(nodiscard) >= 201603L
#	define NODISCARD [[nodiscard]]
#else
#	define NODISCARD
#endif

//	novtable
#if ISM_CC_MSVC
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

//	inlining
#if ISM_CC_MSVC
#	define FORCE_INLINE __forceinline
#	define NEVER_INLINE __declspec(noinline)

#elif ISM_CC_CLANG || ISM_CC_GCC
#	define FORCE_INLINE __attribute__((always_inline))
#	define NEVER_INLINE __attribute__((noinline))

#else
#	define FORCE_INLINE
#	define NEVER_INLINE
#endif

//	likely
#if __has_cpp_attribute(likely) >= 201907L
#	define LIKELY(expr) ((expr)) [[likely]]
#else
#	define LIKELY(expr) ((expr))
#endif

//	unlikely
#if __has_cpp_attribute(unlikely) >= 201907L
#	define UNLIKELY(expr) ((expr)) [[unlikely]]
#else
#	define UNLIKELY(expr) ((expr))
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	API
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if ISM_STATIC
#	define ISM_API_EXPORT
#	define ISM_API_IMPORT

#elif ISM_OS_WINDOWS
#	define ISM_API_EXPORT __declspec(dllexport)
#	define ISM_API_IMPORT __declspec(dllimport)

#elif ISM_CC_CLANG || (__GNUC__ && (__GNUC__ >= 4))
#	define ISM_API_EXPORT __attribute__((visibility("default")))
#	define ISM_API_IMPORT __attribute__((visibility("hidden")))

#else
#	define ISM_API_EXPORT
#	define ISM_API_IMPORT
#endif

#ifndef ISM_API
#	ifdef ISM_EXPORTS
#		define ISM_API ISM_API_EXPORT
#	else
#		define ISM_API ISM_API_IMPORT
#	endif
#endif

#define ISM_API_FUNC(rtype) ISM_API rtype

#define ISM_API_DATA(rtype) ISM_API extern rtype


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	STL FEATURES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _HAS_FUNCTION_ALLOCATOR_SUPPORT
#define _HAS_FUNCTION_ALLOCATOR_SUPPORT true
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	DISABLE ANNOYING ISM_CC WARNINGS
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if ISM_OS_WINDOWS
#	pragma warning(disable: 4031)	//	second formal parameter list longer than the first list
#	pragma warning(disable: 4067)	//	unexpected tokens following preprocessor directive - expected a newline
#	pragma warning(disable: 4251)	//	type1 needs to have dll-interface to be used by type2
#	pragma warning(disable: 4307)	//	integral constant overflow
#	pragma warning(disable: 4308)	//	negative integral constant converted to unsigned t
#	pragma warning(disable: 4309)	//	truncation of constant value
#	pragma warning(disable: 4312)	//	conversion to greater size
#	pragma warning(disable: 4723)	//	potential divide by zero
#	pragma warning(disable: 6011)	//	dereferencing NULL pointer
#	pragma warning(disable: 6282)	//	incorrect operator
#	pragma warning(disable: 26437)	//	do not slice
#	pragma warning(disable: 26444)	//	avoid unnamed objecs with custom construction and destruction
#	pragma warning(disable: 26451)	//	arithmetic overflow
#	pragma warning(disable: 26495)	//	value may be uninitialized
#	pragma warning(disable: 26498)	//	mark as constexpr if desired
#	pragma warning(disable: 26812)	//	unscoped enum
#	pragma warning(disable: 28251)	//	inconsistent annotations
#	pragma warning(disable: 33101)	//	unchecked lower bound for enum type used as index
#endif

#endif //	!_ISM_LANGUAGE_FEATURES_HPP_
