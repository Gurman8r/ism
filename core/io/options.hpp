#ifndef _ISM_OPTIONS_HPP_
#define _ISM_OPTIONS_HPP_

#include <core/object/class.hpp>

namespace Ism
{
	class ISM_API Options : public Object
	{
		DEFINE_CLASS(Options, Object);

		String
			m_trigger	{},
			m_value		{},
			m_desc		{},
			m_default	{},
			m_category	{};

		Map<String, String> m_allowed{};

	public:
		Options(String const & trigger, String const & value, String const & desc, String const & default_value, String const & category, Map<String, String> const & allowed = {});

		Error_ parse(i32 argc, cstring argv[]);

		NODISCARD auto get_trigger() const noexcept -> String const & { return m_trigger; }
		NODISCARD auto get_value() const noexcept -> String const & { return m_value; }
		NODISCARD auto get_desc() const noexcept -> String const & { return m_desc; }
		NODISCARD auto get_default() const noexcept -> String const & { return m_default; }
		NODISCARD auto get_category() const noexcept -> String const & { return m_category; }
		NODISCARD auto get_allowed() const noexcept -> Map<String, String> const & { return m_allowed; }
	};
}

#endif // !_ISM_OPTIONS_HPP_
