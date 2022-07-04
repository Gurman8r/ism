#ifndef _ISM_LANGUAGE_FEATURES_HPP_
#define _ISM_LANGUAGE_FEATURES_HPP_

#include <cstddef>

#include <core/preprocessor.hpp>

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	CONFIGURATION
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if (_DEBUG && _DEBUG)
//	Debug
#	define IS_DEBUG true
#else
//	Release
#	define IS_DEBUG false
#endif

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	LANGUAGE
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if __cplusplus
#	if _MSVC_LANG
#		define CXX _MSVC_LANG
#	else
#		define CXX __cplusplus
#	endif
#else
#	error "this system does not support C++"
#endif

#if (CXX >= 201907L)
//	C++20
#	define CXX_20 1
#	define CXX_17 1
#	define CXX_14 1
#	define CXX_11 1

#elif (CXX >= 201703L)
//	C++17
#	define CXX_20 0
#	define CXX_17 1
#	define CXX_14 1
#	define CXX_11 1

#elif (CXX >= 201402L)
//	C++14
#	define CXX_20 0
#	define CXX_17 0
#	define CXX_14 1
#	define CXX_11 1

#elif (CXX >= 201103L)
//	C++11
#	define CXX_20 0
#	define CXX_17 0
#	define CXX_14 0
#	define CXX_11 1

#else
#	error "this version of C++ is not supported"
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	OPERATING SYSTEM
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _WIN32 || _WIN64 || WIN32 || WIN64 || __MINGW32__ || __MINGW64__
//	Windows
#define OS_WINDOWS true

#elif __APPLE__ && __MACH__
//	Apple
#define OS_APPLE true

#elif __unix__
//	Unix
#	define OS_UNIX true

#	if __ANDROID__
//	Android
#	define OS_ANDROID true

#	elif __linux__
//	Linux
#	define OS_LINUX true

#	elif __FreeBSD__ || __FreeBSD_kernel__
//	FreeBSD
#	define OS_FREEBSD true

#	else
	static_assert(!"unsupported unix operating system");
#	endif

#else
static_assert(!"unsupported operating system");
#endif


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	PROCESSOR
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if __x86_64__ || _M_X64 || _x64
//	x64
#define CPU_X64 true
#define ARCHITECTURE 64

#elif __i386__ || _M_IX86
//	x86
#define CPU_X86 true
#define ARCHITECTURE 32

#elif __arm__ || _M_ARM || __aarch64__
//	ARM
#	if __aarch64__
#		define CPU_ARM64 true
#		define ARCHITECTURE 64
#	else
#		define CPU_ARM32 true
#		define ARCHITECTURE 32
#	endif

#elif ppc || _M_PPC || __ppc64__
//	PowerPC
#	if __ppc64__
#		define CPU_PPC64 true
#		define ARCHITECTURE 64
#	else
#		define CPU_PPC32 true
#		define ARCHITECTURE 32
#	endif

#endif

static_assert((ARCHITECTURE == 32 || ARCHITECTURE == 64), "invalid architecture");


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
//	COMPILER
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if _MSC_VER
//	Visual Studio
#	define CC_MSVC _MSC_VER

#elif __clang__
//	Clang / LLVM
#	define CC_CLANG __clang__

#elif (__GNUC__ || __GNUG__) && !__clang__
//	GCC
#	ifdef __GNUC__
#		define CC_GCC __GNUC__
#	else
#		define CC_GCC __GNUG__
#	endif

#elif __ICC || __INTEL_COMPILER
//	Intel
#	ifdef __ICC
#		define COMPILER_INTEL __ICC
#	else
#		define COMPILER_INTEL __INTEL_COMPILER
#	endif

#elif __MINGW32__ || __MINGW64__
//	MinGW
#	ifdef __MINGW64__
#		define COMPILER_MINGW __MINGW64__
#	else
#		define COMPILER_MINGW __MINGW32__
#	endif

#elif __EMSCRIPTEN__
//	Emscripten
#define COMPILER_EMSCRIPTEN __EMSCRIPTEN__

#elif __asmjs__
//	asm.js
#define COMPILER_ASMJS __asmjs__

#elif __wasm__
//	WebAssembly
#define COMPILER_WASM __wasm__

#else
static_assert(!"invalid compiler");
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
#if CC_MSVC
#define NOVTABLE __declspec(novtable)
#else
#define NOVTABLE
#endif

//	inlining
#if CC_MSVC
#	define FORCE_INLINE __forceinline
#	define NEVER_INLINE __declspec(noinline)

#elif CC_CLANG || CC_GCC
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

#elif OS_WINDOWS
#	define ISM_API_EXPORT __declspec(dllexport)
#	define ISM_API_IMPORT __declspec(dllimport)

#elif CC_CLANG || (__GNUC__ && (__GNUC__ >= 4))
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
//	DISABLE ANNOYING COMPILER WARNINGS
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#if OS_WINDOWS
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
