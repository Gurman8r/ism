#ifndef _ISM_LOGGER_HPP_
#define _ISM_LOGGER_HPP_

#include <core/string/string_utility.hpp>
#include <core/templates/vector.hpp>

namespace ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API Logger
	{
	public:
		enum Error
		{
			ERR_ERROR,
			ERR_WARNING,
		};

		virtual void logv(cstring fmt, va_list args, bool is_error = false) = 0;

		virtual void log_error(cstring func, cstring file, uint32_t line, cstring desc, cstring message, Error type = ERR_ERROR);

		void logf(cstring fmt, ...);

		virtual ~Logger() {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API StdLogger : public Logger
	{
	public:
		virtual void logv(cstring fmt, va_list args, bool is_error = false) override;

		virtual ~StdLogger() override {}
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	class ISM_API CompositeLogger : public Logger
	{
		Vector<Logger *> m_loggers;

	public:
		CompositeLogger(Vector<Logger *> loggers);

		virtual void logv(cstring fmt, va_list args, bool is_error = false) override;

		void add_logger(Logger * value);

		virtual ~CompositeLogger();
	};

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}

#endif // !_ISM_LOGGER_HPP_
