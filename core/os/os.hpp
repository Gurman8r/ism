#ifndef _ISM_OS_HPP_
#define _ISM_OS_HPP_

#include <core/io/logger.hpp>
#include <core/os/main_loop.hpp>
#include <iostream>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	using ProcessID = i64;

	struct Mutex { /* PLACEHOLDER */ };

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	// operating system
	class ISM_API OS
	{
		static OS * __singleton;

		Path				m_exepath{};
		Vector<String>		m_cmdline{};
		i32					m_exit_code{};
		CompositeLogger *	m_logger{};

		String m_current_rendering_driver_name{};
		String m_current_rendering_method{};
		i32 m_display_driver_id{};

	public:
		OS();
		virtual ~OS();
		FORCE_INLINE static OS * get_singleton() noexcept { return __singleton; }
#define SYSTEM (ism::OS::get_singleton())

	protected:
		friend class Main;

		void _add_logger(Logger * value);
		void _set_logger(CompositeLogger * value);

		virtual void initialize() = 0;
		virtual void initialize_joysticks() = 0;
		virtual void finalize() = 0;
		virtual void finalize_core() = 0;
		virtual void set_cmdline(cstring exepath, Vector<String> const & args);
		virtual void set_main_loop(Ref<MainLoop> value) = 0;
		virtual void delete_main_loop() = 0;

	public:
		virtual void run() = 0;

		NODISCARD String get_current_rendering_driver_name() const { return m_current_rendering_driver_name; }
		NODISCARD String get_current_rendering_method() const { return m_current_rendering_method; }
		NODISCARD i32 get_display_driver_id() const { return m_display_driver_id; }
		NODISCARD virtual Vector<String> get_video_adapter_driver_info() const = 0;

		void printv(cstring fmt, va_list args);
		void printf(cstring fmt, ...);
		void printerrv(cstring fmt, va_list args);
		void printerrf(cstring fmt, ...);
		void printerror(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ type = ErrorHandlerType_Error);

		NODISCARD virtual String get_stdin_string(bool blocking = true) = 0;

		//NODISCARD virtual Vector<String> get_connected_midi_inputs();
		//virtual void open_midi_inputs();
		//virtual void close_midi_inputs();

		virtual Error_ open_dynamic_library(Path const & path, void *& instance) = 0;
		virtual Error_ close_dynamic_library(void * instance) = 0;
		virtual Error_ get_dynamic_library_symbol(void * instance, String const & name, void *& symbol, bool is_optional = false) = 0;

		NODISCARD virtual Path get_executable_path() const;
		virtual Error_ execute(Path const & path, Vector<String> const & args, String * pipe = nullptr, i32 * exitcode = nullptr, bool read_stderr = false, Mutex * pipe_mutex = nullptr) = 0;
		virtual Error_ create_process(Path const & path, Vector<String> const & args, ProcessID * child_id = nullptr) = 0;
		virtual Error_ kill(ProcessID const & pid) = 0;
		NODISCARD virtual i32 get_pid() const = 0;

		NODISCARD virtual Path get_cwd() const;
		virtual Error_ set_cwd(Path const & path);
		virtual Error_ shell_open(Path const & path);

		NODISCARD virtual Path get_env(String const & key) const = 0;
		NODISCARD virtual bool has_env(String const & key) const = 0;
		virtual bool set_env(String const & key, Path const & value) const = 0;

		NODISCARD virtual String get_name() const = 0;
		//NODISCARD virtual String get_distro() const = 0;
		//NODISCARD virtual String get_version() const = 0;
		NODISCARD virtual Vector<String> get_cmdline_args() const;
		NODISCARD virtual String get_model_name() const;

		NODISCARD virtual Ref<MainLoop> get_main_loop() const = 0;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		enum Weekday_
		{
			Weekday_Sunday,
			Weekday_Monday,
			Weekday_Tuesday,
			Weekday_Wednesday,
			Weekday_Thursday,
			Weekday_Friday,
			Weekday_Saturday,
			Weekday_MAX
		};

		enum Month_
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
			Month_MAX
		};

		struct Date
		{
			i32			year	{};
			Month_		month	{};
			i32			day		{};
			Weekday_	weekday	{};
			bool		dst		{}; // daylight saving time
		};

		struct Time
		{
			i32 hour{};
			i32 min	{};
			i32 sec	{};
		};

		struct TimeZoneInfo
		{
			i32		bias{};
			String	name{};
		};

		NODISCARD virtual Date get_date(bool local = false) const = 0;
		NODISCARD virtual Time get_time(bool local = false) const = 0;
		NODISCARD virtual TimeZoneInfo get_time_zone() const = 0;
		NODISCARD virtual String get_iso_date_time(bool local = false) const;
		NODISCARD virtual f64 get_unix_time() const;
		//virtual void delay(Duration const & duration) = 0;
		//virtual void add_frame_delay(bool can_draw);
		//NODISCARD virtual Duration get_ticks() const = 0;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		//NODISCARD bool is_stdout_verbose() const;
		//NODISCARD bool is_stdout_debug_enabled() const;
		//NODISCARD bool is_stdout_enabled() const;
		//NODISCARD bool is_stderr_enabled() const;
		//void set_stdout_enabled(bool enabled);
		//void set_stderr_enabled(bool enabled);

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual u64 get_static_memory_usage() const;
		NODISCARD virtual u64 get_static_memory_peak_usage() const;
		NODISCARD virtual u64 get_free_static_memory() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD virtual String get_locale() const;
		NODISCARD String get_locale_language() const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		NODISCARD Path get_safe_path(Path const & path, bool allow_dir_separator = false) const;
		NODISCARD virtual Path get_binary_path() const;
		NODISCARD virtual Path get_bundle_path() const;
		NODISCARD virtual Path get_cache_path() const;
		NODISCARD virtual Path get_config_path() const;
		NODISCARD virtual Path get_data_path() const;
		NODISCARD virtual Path get_resource_path() const;
		NODISCARD virtual Path get_user_path() const;

		enum SystemDir_
		{
			SystemDir_DCIM,
			SystemDir_Desktop,
			SystemDir_Documents,
			SystemDir_Downloads,
			SystemDir_Movies,
			SystemDir_Music,
			SystemDir_Pictures,
			SystemDir_Ringtones,
			SystemDir_MAX
		};

		NODISCARD virtual Path get_system_dir(SystemDir_ value) const;

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		virtual Error_ move_to_trash(Path const & path);

		virtual void debug_break();

		NODISCARD virtual i32 get_exit_code() const;
		virtual void set_exit_code(i32 value);

		NODISCARD virtual i32 get_processor_count() const;
		//NODISCARD virtual String get_processor_name() const;
		NODISCARD virtual i32 get_default_thread_pool_size() const { return get_processor_count(); }

		NODISCARD virtual String get_unique_id() const;
		
		NODISCARD virtual bool has_feature(String const & value) const;

		//void set_restart_on_exit(bool restart, Vector<String> const & args);
		//NODISCARD bool is_restart_on_exit_set() const;
		//NODISCARD Vector<String> get_restart_on_exit_arguments() const;

		//NODISCARD virtual bool request_permission(String const & name) { return true; }
		//NODISCARD virtual bool request_permissions() { return true; }
		//NODISCARD virtual Vector<String> get_granted_permissions() const { return Vector<String>(); }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	};
}

#endif // !_ISM_OS_HPP_
