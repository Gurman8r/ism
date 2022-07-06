#include <core/os/os.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	MEMBER_IMPL(OS::g_singleton) {};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OS::OS()
	{
		g_singleton = this;

		List<Logger *> temp{};
		temp.push_back(memnew(StdLogger));
		_set_logger(memnew(CompositeLogger(std::move(temp))));
	}

	OS::~OS()
	{
		memdelete(m_logger);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS::_add_logger(Logger * value)
	{
		if (!value)
		{
			List<Logger *> temp{};
			temp.push_back(memnew(StdLogger));
			_set_logger(memnew(CompositeLogger(std::move(temp))));
		}
		else
		{
			m_logger->add_logger(value);
		}
	}

	void OS::_set_logger(CompositeLogger * value)
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

	void OS::printerr(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ type)
	{
		m_logger->log_error(func, file, line, desc, message, type);
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

	Path OS::get_executable_path() const
	{
		return m_exepath;
	}

	Path OS::get_cwd() const
	{
		return {};
	}

	Error_ OS::set_cwd(Path const & path)
	{
		return Error_Unknown;
	}

	Error_ OS::shell_open(Path const & path)
	{
		return Error_Unknown;
	}

	List<String> OS::get_cmdline_args() const
	{
		return m_cmdline;
	}

	void OS::set_cmdline(cstring exepath, List<String> const & args)
	{
		m_exepath = exepath;
		m_cmdline = args;
	}

	String OS::get_model_name() const
	{
		return "GenericModel"_s;
	}

	String OS::get_iso_date_time(bool local) const
	{
		return {};
	}

	double_t OS::get_unix_time() const
	{
		return {};
	}

	uint64_t OS::get_static_memory_usage() const
	{
		return {};
	}

	uint64_t OS::get_static_memory_peak_usage() const
	{
		return {};
	}

	uint64_t OS::get_free_static_memory() const
	{
		return {};
	}

	String OS::get_locale() const
	{
		return {};
	}

	Path OS::get_data_path() const
	{
		return ".";
	}

	Path OS::get_config_path() const
	{
		return ".";
	}

	Path OS::get_cache_path() const
	{
		return ".";
	}

	Path OS::get_bundle_resource_dir() const
	{
		return ".";
	}

	Path OS::get_user_data_dir() const
	{
		return ".";
	}

	Path OS::get_resource_dir() const
	{
		return ".";
	}

	Path OS::get_safe_dir_name(Path const & p_dir_name, bool p_allow_dir_separator) const
	{
		return ".";
	}

	Path OS::get_ism_dir_name() const
	{
		return ".";
	}

	Path OS::get_system_dir(SystemDir value) const
	{
		return ".";
	}

	Error_ OS::move_to_trash(Path const & path)
	{
		return Error_Unknown;
	}

	void OS::debug_break()
	{
	}

	int32_t OS::get_exit_code() const
	{
		return m_exit_code;
	}

	void OS::set_exit_code(int32_t value)
	{
	}

	int32_t OS::get_processor_count() const
	{
		return -1;
	}

	String OS::get_unique_id() const
	{
		return {};
	}

	bool OS::has_feature(String const & value)
	{
		return false;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}