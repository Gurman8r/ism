#include <core/io/logger.hpp>

namespace ISM
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void Logger::log_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, Err type)
	{
		cstring err_type;
		switch (type)
		{
		default: {
			err_type = "ERROR";
		} break;

		case ISM::Logger::ERR_ERROR: {
			err_type = "ERROR";
		} break;

		case ISM::Logger::ERR_WARNING: {
			err_type = "WARNING";
		} break;
		}

		logf("%s: %s\n", err_type, (message && *message) ? message : desc);

		logf("\tat: %s (%s:%i) - %s\n", func, file, line, type);
	}

	void Logger::logf(cstring fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		logv(fmt, args, false);
		va_end(args);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	void StdLogger::logv(cstring fmt, va_list args, bool is_error)
	{
		std::vfprintf(is_error ? stderr : stdout, fmt, args);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	CompositeLogger::CompositeLogger(Vector<Logger *> loggers) : m_loggers{ std::move(loggers) }
	{
	}

	void CompositeLogger::logv(cstring fmt, va_list args, bool is_error)
	{
		for (size_t i = 0; i < m_loggers.size(); ++i)
		{
			va_list args_copy;
			va_copy(args_copy, args);
			m_loggers[i]->logv(fmt, args_copy, is_error);
			va_end(args_copy);
		}
	}

	void CompositeLogger::add_logger(Logger * value)
	{
		if (value)
		{
			m_loggers.push_back(value);
		}
	}

	CompositeLogger::~CompositeLogger()
	{
		for (size_t i = 0; i < m_loggers.size(); ++i)
		{
			memdelete(m_loggers[i]);
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}