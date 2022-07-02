#include <core/os/os.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(OS::singleton) {};

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
}

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
#if OS_WINDOWS
	std::system("pause");
#else
	this->print("\nPress enter to continue . . .");
	this->get_stdin_string(true);
#endif
}

void OS::print_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ type)
{
	m_logger->log_error(func, file, line, desc, message, type);
}

void OS::print(String const & s)
{
	print("%s", s.c_str());
}

void OS::print(cstring fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printv(fmt, args);
	va_end(args);
}

void OS::printv(cstring fmt, va_list args)
{
	m_logger->logv(fmt, args, false);
}

void OS::printerr(String const & s)
{
	printerr("%s", s.c_str());
}

void OS::printerr(cstring fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	printerrv(fmt, args);
	va_end(args);
}

void OS::printerrv(cstring fmt, va_list args)
{
	m_logger->logv(fmt, args, true);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */