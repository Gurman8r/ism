#include <core/os/os.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(OperatingSystem::singleton) {};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

OperatingSystem::OperatingSystem()
{
	singleton = this;

	Vector<Logger *> temp{};
	temp.push_back(memnew(StdLogger));
	set_logger(memnew(CompositeLogger(std::move(temp))));
}

OperatingSystem::~OperatingSystem()
{
	memdelete(m_logger);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void OperatingSystem::add_logger(Logger * value)
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

void OperatingSystem::set_cmdline(cstring exepath, Vector<String> const & args)
{
	m_exepath = exepath;
	m_cmdline = args;
}

void OperatingSystem::set_logger(CompositeLogger * value)
{
	memdelete_nonzero(m_logger);

	m_logger = value;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void OperatingSystem::pause()
{
#if SYSTEM_WINDOWS
	std::system("pause");
#else
	this->print("\nPress enter to continue . . .");
	this->get_stdin_string(true);
#endif
}

void OperatingSystem::print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ type)
{
	m_logger->log_error(func, file, line, desc, message, type);
}

void OperatingSystem::print(String const & s)
{
	print("%s", s.c_str());
}

void OperatingSystem::print(cstring fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printv(fmt, args);
	va_end(args);
}

void OperatingSystem::printv(cstring fmt, va_list args)
{
	m_logger->logv(fmt, args, false);
}

void OperatingSystem::printerr(String const & s)
{
	printerr("%s", s.c_str());
}

void OperatingSystem::printerr(cstring fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printerrv(fmt, args);
	va_end(args);
}

void OperatingSystem::printerrv(cstring fmt, va_list args)
{
	m_logger->logv(fmt, args, true);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */