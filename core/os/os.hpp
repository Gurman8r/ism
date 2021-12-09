#ifndef _ISM_OS_HPP_
#define _ISM_OS_HPP_

#include <core/io/logger.hpp>
#include <core/os/main_loop.hpp>
#include <iostream>

// os function signature
#define OS_FUNC(user) \
	ism::OS & __os_impl__(user)

// os main
#define OS_MAIN(user) \
	static ism::OS & ANON{ __os_impl__(user) }

// os implementation
#define OS_IMPL(user, decl)		\
	OS_FUNC(user)				\
	{							\
		RETURN_STATIC(decl);	\
	}							\

// os main default
#define OS_DEFAULT()			\
	extern OS_FUNC(void *);		\
	OS_MAIN(nullptr);			\

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// operating system
	class ISM_API OS
	{
	private:
		static OS * singleton;

		String				m_exepath{};
		Vector<String>		m_cmdline{};
		int32_t				m_exit_code{};
		CompositeLogger *	m_logger{};

	public:
		OS();

		virtual ~OS();

		FORCE_INLINE static OS * get_singleton() noexcept { return singleton; }

	protected:
		friend class Main;

		void add_logger(Logger * value);

		virtual void initialize() = 0;

		virtual void finalize() = 0;

		virtual void finalize_core() = 0;

		virtual void set_cmdline(cstring exepath, Vector<String> const & args);

		virtual void set_logger(CompositeLogger * value);

		virtual void set_main_loop(Ref<MainLoop> value) = 0;

		virtual void delete_main_loop() = 0;

	public:
		void pause();
		
		void print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ type = ErrorHandlerType_Error);
		
		void print(String const & s);

		void print(cstring fmt, ...);

		void printv(cstring fmt, va_list args);

		void printerr(String const & s);
		
		void printerr(cstring fmt, ...);
		
		void printerrv(cstring fmt, va_list args);
		
		virtual String get_stdin_string(bool block = true) = 0;

	public:
		NODISCARD int32_t get_exit_code() const noexcept { return m_exit_code; }

		NODISCARD virtual String get_executable_path() const { return m_exepath; }
		
		NODISCARD virtual Vector<String> get_cmdline_args() const { return m_cmdline; }

		NODISCARD virtual String get_environment(String const & key) const = 0;
		
		NODISCARD virtual bool has_environment(String const & key) const = 0;
		
		virtual bool set_environment(String const & key, String const & value) const = 0;

		virtual Path get_cwd() const = 0;
		
		virtual Error_ set_cwd(Path const & path) { return Error_Unknown; }

		NODISCARD virtual String get_name() const = 0;

		NODISCARD virtual Ref<MainLoop> get_main_loop() const = 0;

	public:
		virtual Error_ open_dynamic_library(Path const & path, void *& instance) { return Error_Unknown; }
		
		virtual Error_ close_dynamic_library(void * instance) { return Error_Unknown; }
		
		virtual Error_ get_dynamic_library_symbol_handle(void * instance, String const & name, void *& symbol, bool is_optional = false) { return Error_Unknown; }
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_OS_HPP_
