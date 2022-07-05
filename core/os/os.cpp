#include <core/os/os.hpp>

using namespace ism;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

MEMBER_IMPL(OS::g_singleton) {};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

static String vstring(cstring fmt, ...)
{
	
}

OS::OS()
{
	g_singleton = this;

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

void OS::set_logger(CompositeLogger * value)
{
	if (m_logger) { memdelete(m_logger); }
	m_logger = value;
}

void OS::set_cmdline(cstring exepath, Vector<String> const & args)
{
	m_exepath = exepath;
	m_cmdline = args;
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

void OS::print(cstring str, size_t size)
{
	printf("%.*s", str, size);
}

void OS::print(cstring str)
{
	printf("%s", str);
}

void OS::print(String const & str)
{
	printf("%s", str.c_str());
}

void OS::errorv(cstring fmt, va_list args)
{
	m_logger->logv(fmt, args, true);
}

void OS::errorf(cstring fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	errorv(fmt, args);
	va_end(args);
}

void OS::error(cstring str, size_t size)
{
	errorf("%.*s", str, size);
}

void OS::error(cstring str)
{
	errorf("%s", str);
}

void OS::error(String const & str)
{
	errorf("%s", str.c_str());
}

void OS::error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, ErrorHandlerType_ type)
{
	m_logger->log_error(func, file, line, desc, message, type);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */