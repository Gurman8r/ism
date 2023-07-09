#include <core/os/os.hpp>
#include <core/config/project_settings.hpp>

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OS * OS::__singleton{};

	OS::OS()
	{
		SINGLETON_CTOR();

		m_stdout_enabled = m_stderr_enabled = true;

		Vector<Logger *> temp{};
		temp.push_back(memnew(StdLogger));
		set_logger(memnew(CompositeLogger(std::move(temp))));
	}

	OS::~OS()
	{
		SINGLETON_DTOR();

		memdelete(m_logger);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS::add_logger(Logger * value)
	{
		if (!value) {
			Vector<Logger *> temp{};
			temp.push_back(memnew(StdLogger));
			set_logger(memnew(CompositeLogger(std::move(temp))));
		}
		else {
			m_logger->add_logger(value);
		}
	}

	void OS::set_logger(CompositeLogger * value)
	{
		if (m_logger) { memdelete(m_logger); }
		m_logger = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS::printv(cstring fmt, va_list args)
	{
		m_logger->logv(fmt, args, false);
	}

	void OS::printf(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		printv(fmt, args);
		va_end(args);
	}

	void OS::err_printv(cstring fmt, va_list args)
	{
		m_logger->logv(fmt, args, true);
	}

	void OS::err_printf(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		err_printv(fmt, args);
		va_end(args);
	}

	void OS::err_print(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ type)
	{
		m_logger->log_error(func, file, line, desc, message, type);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Vector<String> OS::get_connected_midi_inputs()
	{
		return Vector<String>();
	}

	void OS::open_midi_inputs()
	{
	}

	void OS::close_midi_inputs()
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS::alert(String const & message, String const & title)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String OS::get_cwd() const
	{
		return {};
	}

	Error_ OS::set_cwd(String const & path)
	{
		return Error_Failed;
	}

	Error_ OS::shell_open(String const & path)
	{
		return Error_Failed;
	}

	Vector<String> OS::get_cmdline_args() const
	{
		return m_cmdline;
	}

	void OS::set_cmdline(cstring exec_path, Vector<String> const & args)
	{
		m_exec_path = exec_path;
		m_cmdline = args;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String OS::get_model_name() const
	{
		return "GenericModel"_s;
	}

	String OS::get_iso_date_time(bool local) const
	{
		return {};
	}

	f64 OS::get_unix_time() const
	{
		return {};
	}

	void OS::add_frame_delay(bool can_draw)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	bool OS::is_stdout_verbose() const
	{
		return false;
	}

	bool OS::is_stdout_debug_enabled() const
	{
		return false;
	}

	bool OS::is_stdout_enabled() const
	{
		return false;
	}

	bool OS::is_stderr_enabled() const
	{
		return false;
	}

	void OS::set_stdout_enabled(bool enabled)
	{
	}

	void OS::set_stderr_enabled(bool enabled)
	{
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	u64 OS::get_static_memory_usage() const
	{
		return 0;
	}

	u64 OS::get_static_memory_peak_usage() const
	{
		return 0;
	}

	u64 OS::get_free_static_memory() const
	{
		return 0;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String OS::get_locale() const
	{
		return {};
	}

	String OS::get_locale_language() const
	{
		return {};
	}

	String OS::get_safe_path(String const & path, bool allow_dir_separator) const
	{
		return path;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	String OS::get_cache_path() const noexcept
	{
		return ".";
	}

	String OS::get_config_path() const noexcept
	{
		return ".";
	}

	String OS::get_data_path() const noexcept
	{
		return ".";
	}

	String OS::get_exec_path() const noexcept
	{
		return m_exec_path;
	}

	String OS::get_resource_path() const noexcept
	{
		return get_globals()->get_resource_path();
	}

	String OS::get_system_path(SystemDir_ value) const noexcept
	{
		return ".";
	}

	String OS::get_user_path() const noexcept
	{
		return ".";
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	Error_ OS::move_to_trash(String const & path)
	{
		return Error_Unavailable;
	}

	void OS::debug_break()
	{
	}

	i32 OS::get_exit_code() const
	{
		return m_exit_code;
	}

	void OS::set_exit_code(i32 value)
	{
		m_exit_code = value;
	}

	i32 OS::get_processor_count() const
	{
		return -1;
	}

	String OS::get_processor_name() const
	{
		return {};
	}

	String OS::get_unique_id() const
	{
		return {};
	}

	bool OS::has_feature(String const & value) const
	{
		return false;
	}

	void OS::set_restart_on_exit(bool restart, Vector<String> const & args)
	{
	}

	bool OS::is_restart_on_exit_set() const
	{
		return false;
	}

	Vector<String> OS::get_restart_on_exit_arguments() const
	{
		return {};
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}