#ifndef _ISM_OS_HPP_
#define _ISM_OS_HPP_

#include <core/io/logger.hpp>
#include <core/os/main_loop.hpp>
#include <iostream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	ALIAS(ProcessID) int64_t;

	struct Mutex { /* PLACEHOLDER */ };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// operating system
	class ISM_API OS
	{
		static OS * __singleton;

		Path				m_exepath{};
		List<String>		m_cmdline{};
		int32_t				m_exit_code{};
		CompositeLogger *	m_logger{};

	public:
		OS();
		virtual ~OS();
		FORCE_INLINE static OS * get_singleton() noexcept { return __singleton; }

	protected:
		friend class Main;

		void _add_logger(Logger * value);
		void _set_logger(CompositeLogger * value);

		virtual void initialize() = 0;
		virtual void finalize() = 0;
		virtual void finalize_core() = 0;
		virtual void set_cmdline(cstring exepath, List<String> const & args);
		virtual void set_main_loop(Ref<MainLoop> value) = 0;
		virtual void delete_main_loop() = 0;

	public:
		void printv(cstring fmt, va_list args);
		void printf(cstring fmt, ...);
		void printerr(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ type = ErrorHandlerType_Error);
		void printerrv(cstring fmt, va_list args);
		void printerrf(cstring fmt, ...);

		virtual String get_stdin_string(bool block = true) = 0;

		virtual Error_ open_dynamic_library(Path const & path, void *& instance) = 0;
		virtual Error_ close_dynamic_library(void * instance) = 0;
		virtual Error_ get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional = false) = 0;

		virtual Path get_executable_path() const;
		virtual Error_ execute(Path const & path, List<String> const & args, String * pipe = nullptr, int32_t * exitcode = nullptr, bool read_stderr = false, Mutex * pipe_mutex = nullptr) = 0;
		virtual Error_ create_process(Path const & path, List<String> const & args, ProcessID * child_id = nullptr) = 0;
		virtual Error_ kill(ProcessID const & pid) = 0;
		virtual int32_t get_process_id() const = 0;

		virtual Path get_cwd() const;
		virtual Error_ set_cwd(Path const & path);
		virtual Error_ shell_open(Path const & path);

		virtual Path get_env(String const & key) const = 0;
		virtual bool has_env(String const & key) const = 0;
		virtual bool set_env(String const & key, Path const & value) const = 0;

		virtual String get_name() const = 0;
		virtual List<String> get_cmdline_args() const;
		virtual String get_model_name() const;

		virtual Ref<MainLoop> get_main_loop() const = 0;

		enum Weekday
		{
			Weekday_Sunday,
			Weekday_Monday,
			Weekday_Tuesday,
			Weekday_Wednesday,
			Weekday_Thursday,
			Weekday_Friday,
			Weekday_Saturday,
		};

		enum Month
		{
			Month_January,
			Month_February,
			Month_March,
			Month_April,
			Month_May,
			Month_June,
			Month_July,
			Month_August,
			Month_September,
			Month_October,
			Month_November,
			Month_December,
		};

		struct Date
		{
			int32_t	year;
			Month	month;
			int32_t	day;
			Weekday	weekday;
			bool	dst;
		};

		struct Time
		{
			int32_t hour;
			int32_t min;
			int32_t sec;
		};

		struct TimeZoneInfo
		{
			int32_t bias;
			String name;
		};

		virtual Date get_date(bool local = false) const = 0;
		virtual Time get_time(bool local = false) const = 0;
		virtual TimeZoneInfo get_time_zone() const = 0;
		virtual String get_iso_date_time(bool local = false) const;
		virtual double_t get_unix_time() const;

		virtual uint64_t get_static_memory_usage() const;
		virtual uint64_t get_static_memory_peak_usage() const;
		virtual uint64_t get_free_static_memory() const;

		virtual String get_locale() const;

		Path get_safe_dir_name(Path const & path, bool allow_dir_separator = false) const;
		virtual Path get_ism_dir_name() const;

		virtual Path get_data_path() const;
		virtual Path get_config_path() const;
		virtual Path get_cache_path() const;
		virtual Path get_bundle_resource_dir() const;

		virtual Path get_user_data_dir() const;
		virtual Path get_resource_dir() const;

		enum SystemDir
		{
			SystemDir_DCIM,
			SystemDir_Desktop,
			SystemDir_Documents,
			SystemDir_Downloads,
			SystemDir_Movies,
			SystemDir_Music,
			SystemDir_Pictures,
			SystemDir_Ringtones,
		};

		virtual Path get_system_dir(SystemDir value) const;

		virtual Error_ move_to_trash(Path const & path);

		virtual void debug_break();

		virtual int32_t get_exit_code() const;
		virtual void set_exit_code(int32_t value);

		virtual int32_t get_processor_count() const;

		virtual String get_unique_id() const;

		bool has_feature(String const & value);
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#define SYSTEM (ism::OS::get_singleton())

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_OS_HPP_
