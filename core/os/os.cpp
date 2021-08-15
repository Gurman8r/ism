#include <core/os/os.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OS * OS::singleton{};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS::add_logger(Logger * value)
	{
		if (!value)
		{
			Vector<Logger *> temp{};
			temp.push_back(memnew(StdLogger));
			set_logger(memnew(CompositeLogger(std::move(temp))));
		}
		else
		{
			m_logger->add_logger(value);
		}
	}

	void OS::set_cmdline(cstring exepath, Vector<String> const & args)
	{
		m_exepath = exepath;
		m_cmdline = args;
	}

	void OS::set_logger(CompositeLogger * value)
	{
		memdelete_nonzero(m_logger);
		
		m_logger = value;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void OS::pause()
	{
#ifdef ISM_OS_WINDOWS
		std::system("pause");
#else
		this->print("\nPress enter to continue . . .");
		this->get_stdin_string(true);
#endif
	}

	void OS::print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, Logger::Error type)
	{
		m_logger->log_error(func, file, line, desc, message, type);
	}

	void OS::print(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		m_logger->logv(fmt, args, false);
		va_end(args);
	}

	void OS::printerr(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		m_logger->logv(fmt, args, true);
		va_end(args);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	OS::OS()
	{
		singleton = this;

		Vector<Logger *> temp{};
		temp.push_back(memnew(StdLogger));
		set_logger(memnew(CompositeLogger(std::move(temp))));
	}

	OS::~OS()
	{
		memdelete(m_logger);

		singleton = nullptr;
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}