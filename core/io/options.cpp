#include <core/io/options.hpp>

namespace Ism
{
	EMBED_CLASS(Options, t) {}

	Options::Options(String const & trigger, String const & value, String const & desc, String const & default_value, String const & category, Map<String, String> const & allowed)
		: m_trigger	{ trigger }
		, m_value	{ value }
		, m_desc	{ desc }
		, m_default	{ default_value }
		, m_category{ category }
		, m_allowed	{ allowed }
	{
	}

	Error_ Options::parse(i32 argc, cstring argv[])
	{
		return Error_OK;
	}
}