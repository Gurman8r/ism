#ifndef _SYSTEM_HPP_
#define _SYSTEM_HPP_

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
#define OS_SPECIAL(user, decl)	\
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

	struct VideoMode;

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

		OS();

	public:
		virtual ~OS();

		FORCE_INLINE static OS * get_singleton() noexcept { return singleton; }

	public:
		void pause();
		
		void print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, Logger::Error type = Logger::ERR_ERROR);
		
		void print(cstring fmt, ...);
		
		void printerr(cstring fmt, ...);
		
		virtual String get_stdin_string(bool block = true) = 0;

	public:
		NODISCARD virtual String get_executable_path() const { return m_exepath; }
		
		NODISCARD virtual Vector<String> get_cmdline_args() const { return m_cmdline; }

		NODISCARD virtual String get_environment(String const & key) const = 0;

		NODISCARD virtual bool has_environment(String const & key) const = 0;

		virtual bool set_environment(String const & key, String const & value) const = 0;

		virtual String get_cwd() const = 0;

		virtual Error set_cwd(String const & path) { return Error_Unknown; }

		NODISCARD virtual String get_name() const = 0;

		NODISCARD virtual Ref<MainLoop> get_main_loop() const = 0;

	public:
		virtual Error open_dynamic_library(String const & path, void *& instance) { return Error_Unknown; }
		
		virtual Error close_dynamic_library(void * instance) { return Error_Unknown; }
		
		virtual Error get_dynamic_library_symbol_handle(void * instance, String const & name, void *& symbol, bool is_optional = false) { return Error_Unknown; }

	public:
		NODISCARD virtual VideoMode const & get_desktop_video_mode() const = 0;

		NODISCARD virtual Vector<VideoMode> const & get_fullscreen_video_modes() const = 0;
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_SYSTEM_HPP_
