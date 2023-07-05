#include <core/os/os.hpp>

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

	void OS::printerrv(cstring fmt, va_list args)
	{
		m_logger->logv(fmt, args, true);
	}

	void OS::printerrf(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		printerrv(fmt, args);
		va_end(args);
	}

	void OS::printerror(cstring func, cstring file, u32 line, cstring desc, cstring message, ErrorHandlerType_ type)
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

	void OS::set_cmdline(cstring exepath, Vector<String> const & args)
	{
		m_exe_dir = exepath;
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
		return {};
	}

	u64 OS::get_static_memory_peak_usage() const
	{
		return {};
	}

	u64 OS::get_free_static_memory() const
	{
		return {};
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

	String OS::localize_path(String const & path) const
	{
		return {};
	}

	String OS::globalize_path(String const & path) const
	{
		if (path.has_prefix("res://")) {
			if (!m_resources_dir.empty()) { return path.replace("res:/", m_resources_dir); }
			return path.replace("res://", "");
		}
		else if (path.has_prefix("user://")) {
			if (String const data_dir{ get_os()->get_user_dir() }; !data_dir.empty()) { return path.replace("user:/", data_dir); }
			return path.replace("user://", "");
		}
		return path;
	}

	String OS::get_bin_dir() const { return m_bin_dir; }

	String OS::get_cache_dir() const { return m_cache_dir; }

	String OS::get_config_dir() const { return m_config_dir; }

	String OS::get_data_dir() const { return m_data_dir; }

	String OS::get_downloads_dir() const { return m_downloads_dir; }

	String OS::get_exe_dir() const { return m_exe_dir; }

	String OS::get_mods_dir() const { return m_mods_dir; }

	String OS::get_profiles_dir() const { return m_profiles_dir; }

	String OS::get_resource_dir() const { return m_resources_dir; }

	String OS::get_saves_dir() const { return m_saves_dir; }

	String OS::get_system_dir(SystemDir_ value) const { return "./"; }

	String OS::get_user_dir() const { return m_user_dir; }

	Error_ OS::move_to_trash(String const & path) { return Error_Failed; }

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS::debug_break()
	{
	}

	i32 OS::get_exit_code() const
	{
		return m_exit_code;
	}

	void OS::set_exit_code(i32 value)
	{
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