#ifndef _ISM_OS_HPP_
#define _ISM_OS_HPP_

#include <core/io/logger.hpp>
#include <core/os/main_loop.hpp>
#include <iostream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// operating system
	class ISM_API OS
	{
		static OS * g_singleton;

		String				m_exepath{};
		Vector<String>		m_cmdline{};
		int32_t				m_exit_code{};
		CompositeLogger *	m_logger{};

	public:
		OS();

		virtual ~OS();

		FORCE_INLINE static OS * get_singleton() noexcept { return g_singleton; }

	protected:
		friend class Main;

		void add_logger(Logger * value);
		void set_logger(CompositeLogger * value);

		virtual void initialize() = 0;
		virtual void finalize() = 0;
		virtual void finalize_core() = 0;
		virtual void set_cmdline(cstring exepath, Vector<String> const & args);
		virtual void set_main_loop(Ref<MainLoop> value) = 0;
		virtual void delete_main_loop() = 0;

	public:
		void printv(cstring fmt, va_list args);
		void printf(cstring fmt, ...);
		void print(cstring str, size_t size);
		void print(cstring str);
		void print(String const & str);
		void errorv(cstring fmt, va_list args);
		void errorf(cstring fmt, ...);
		void error(cstring str, size_t size);
		void error(cstring str);
		void error(String const & str);
		void error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ type = ErrorHandlerType_Error);

	public:
		int32_t get_exit_code() const noexcept { return m_exit_code; }
		virtual String get_executable_path() const { return m_exepath; }
		virtual Vector<String> get_cmdline_args() const { return m_cmdline; }
		virtual String get_name() const = 0;
		virtual String get_stdin_string(bool block = true) = 0;
		virtual String get_env(String const & key) const = 0;
		virtual bool has_env(String const & key) const = 0;
		virtual bool set_env(String const & key, String const & value) const = 0;
		virtual Path get_cwd() const = 0;
		virtual Error_ set_cwd(Path const & path) = 0;
		virtual Ref<MainLoop> get_main_loop() const = 0;

	public:
		virtual Error_ open_dynamic_library(Path const & path, void *& instance) = 0;
		virtual Error_ close_dynamic_library(void * instance) = 0;
		virtual Error_ get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional = false) = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// os g_singleton
#define SYSTEM (ism::OS::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_OS_HPP_
