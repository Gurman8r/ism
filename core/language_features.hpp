#ifndef _ISM_LANGUAGE_FEATURES_HPP_
#define _ISM_LANGUAGE_FEATURES_HPP_

#include <stddef.h>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// CONFIGURATION
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_DEBUG)
//							Debug
#	define ISM_IS_DEBUG	1
#else
//							Release
#	define ISM_IS_DEBUG	0
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// LANGUAGE
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(__cplusplus)
#	if defined(_MSVC_LANG)
#		define ISM_CXX _MSVC_LANG
#	else
#		define ISM_CXX __cplusplus
#	endif
#else
#	error "this system does not support C++"
#endif

#if (ISM_CXX >= 201907L)
//							C++20
#	define ISM_CXX_20		1
#	define ISM_CXX_17		1
#	define ISM_CXX_14		1
#	define ISM_CXX_11		1

#elif (ISM_CXX >= 201703L)
//							C++17
#	define ISM_CXX_20		0
#	define ISM_CXX_17		1
#	define ISM_CXX_14		1
#	define ISM_CXX_11		1

#elif (ISM_CXX >= 201402L)
//							C++14
#	define ISM_CXX_20		0
#	define ISM_CXX_17		0
#	define ISM_CXX_14		1
#	define ISM_CXX_11		1

#elif (ISM_CXX >= 201103L)
//							C++11
#	define ISM_CXX_20		0
#	define ISM_CXX_17		0
#	define ISM_CXX_14		0
#	define ISM_CXX_11		1

#else
#	error "this version of C++ is not supported"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// OPERATING SYSTEM
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_WIN32) || defined(_WIN64) || defined(WIN32) || defined(WIN64) || defined(__MINGW32__) || defined(__MINGW64__)
//									Windows
#   define SYSTEM_WINDOWS			true

#elif defined(__APPLE__) && defined(__MACH__)
//									Apple
#   define SYSTEM_APPLE				true

#elif defined(__unix__)
//									Unix
#   define SYSTEM_UNIX				true

#   if defined(__ANDROID__)
//									Android
#       define SYSTEM_ANDROID		true

#   elif defined(__linux__)
//									Linux
#       define SYSTEM_LINUX			true

#   elif defined(__FreeBSD__) || defined(__FreeBSD_kernel__)
//									FreeBSD
#       define SYSTEM_FREEBSD		true

#   else
#       error "this unix operating system is not supported"
#   endif

#else
#   error "this operating system is not supported"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// PROCESSOR
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(__x86_64__) || defined(_M_X64) || defined(_x64)
//									x64
#	define CPU_X64					true
#	define ARCHITECTURE				64

#elif defined(__i386__) || defined(_M_IX86)
//									x86
#	define CPU_X86					true
#	define ARCHITECTURE				32

#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__)
//									ARM
#	if defined(__aarch64__)
#		define CPU_ARM64			true
#		define ARCHITECTURE			64
#	else
#		define CPU_ARM32			true
#		define ARCHITECTURE			32
#	endif

#elif defined(ppc) || defined(_M_PPC) || defined(__ppc64__)
//									PowerPC
#	if defined(__ppc64__)
#		define CPU_PPC64			true
#		define ARCHITECTURE			64
#	else
#		define CPU_PPC32			true
#		define ARCHITECTURE			32
#	endif

#else
#   error "unable to detect platform architecture"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// COMPILER
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if defined(_MSC_VER)
//									Visual Studio
#	define COMPILER_MSVC			_MSC_VER

#elif defined(__clang__)
//									Clang / LLVM
#	define COMPILER_CLANG			__clang__

#elif (defined(__GNUC__) || defined(__GNUG__)) && !defined(__clang__)
//									GCC
#	ifdef __GNUC__
#		define COMPILER_GCC			__GNUC__
#	else
#		define COMPILER_GCC			__GNUG__
#	endif

#elif defined(__ICC) || defined(__INTEL_COMPILER)
//									Intel
#	ifdef __ICC
#		define COMPILER_INTEL		__ICC
#	else
#		define COMPILER_INTEL		__INTEL_COMPILER
#	endif

#elif defined(__MINGW32__) || defined(__MINGW64__)
//									MinGW
#	ifdef __MINGW64__
#		define COMPILER_MINGW		__MINGW64__
#	else
#		define COMPILER_MINGW		__MINGW32__
#	endif

#elif defined(__EMSCRIPTEN__)
//									Emscripten
#	define COMPILER_EMSCRIPTEN		__EMSCRIPTEN__

#elif defined(__asmjs__)
//									asm.js
#	define COMPILER_ASMJS			__asmjs__

#elif defined(__wasm__)
//									WebAssembly
#	define COMPILER_WASM			__wasm__

#else
#   error "this compiler is not supported"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// ATTRIBUTES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

// nodiscard
#if __has_cpp_attribute(nodiscard) >= 201603L
#	define NODISCARD [[nodiscard]]
#else
#	define NODISCARD
#endif

// novtable
#ifdef COMPILER_MSVC
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

// inlining
#ifdef COMPILER_MSVC
#	define FORCE_INLINE __forceinline
#	define NEVER_INLINE __declspec(noinline)

#elif defined(COMPILER_CLANG) || defined(COMPILER_GCC)
#	define FORCE_INLINE __attribute__((always_inline))
#	define NEVER_INLINE __attribute__((noinline))

#else
#	define FORCE_INLINE
#	define NEVER_INLINE
#endif

// likely
#if __has_cpp_attribute(likely) >= 201907L
#	define LIKELY(expr) ((expr)) [[likely]]
#else
#	define LIKELY(expr) ((expr))
#endif

// unlikely
#if __has_cpp_attribute(unlikely) >= 201907L
#	define UNLIKELY(expr) ((expr)) [[unlikely]]
#else
#	define UNLIKELY(expr) ((expr))
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// API
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifdef ISM_STATIC
#	define ISM_API_EXPORT
#	define ISM_API_IMPORT

#elif defined(SYSTEM_WINDOWS)
#	define ISM_API_EXPORT __declspec(dllexport)
#	define ISM_API_IMPORT __declspec(dllimport)

#elif defined(COMPILER_CLANG) || (defined(__GNUC__) && (__GNUC__ >= 4))
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
// STL FEATURES
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _HAS_FUNCTION_ALLOCATOR_SUPPORT
#define _HAS_FUNCTION_ALLOCATOR_SUPPORT true
#endif // !_HAS_FUNCTION_ALLOCATOR_SUPPORT


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
// DISABLE ANNOYING COMPILER WARNINGS
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if SYSTEM_WINDOWS
#	pragma warning(disable: 4031)	// second formal parameter list longer than the first list
#	pragma warning(disable: 4067)	// unexpected tokens following preprocessor directive - expected a newline
#	pragma warning(disable: 4251)	// type1 needs to have dll-interface to be used by type2
#	pragma warning(disable: 4307)	// integral constant overflow
#	pragma warning(disable: 4308)	// negative integral constant converted to unsigned t
#	pragma warning(disable: 4309)	// truncation of constant value
#	pragma warning(disable: 4312)	// conversion to greater size
#	pragma warning(disable: 4723)	// potential divide by zero
#	pragma warning(disable: 6011)	// dereferencing NULL pointer
#	pragma warning(disable: 6282)	// incorrect operator
#	pragma warning(disable: 26437)	// do not slice
#	pragma warning(disable: 26444)	// avoid unnamed objecs with custom construction and destruction
#	pragma warning(disable: 26451)	// arithmetic overflow
#	pragma warning(disable: 26495)	// value may be uninitialized
#	pragma warning(disable: 26498)	// mark as constexpr if desired
#	pragma warning(disable: 26812)	// unscoped enum
#	pragma warning(disable: 28251)	// inconsistent annotations
#endif

#endif // !_ISM_LANGUAGE_FEATURES_HPP_
